/*
 *   Copyright(C) 2011-2018 Intel Corporation All Rights Reserved.
 *
 *   The source code, information  and  material ("Material") contained herein is
 *   owned  by Intel Corporation or its suppliers or licensors, and title to such
 *   Material remains  with Intel Corporation  or its suppliers or licensors. The
 *   Material  contains proprietary information  of  Intel or  its  suppliers and
 *   licensors. The  Material is protected by worldwide copyright laws and treaty
 *   provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
 *   modified, published, uploaded, posted, transmitted, distributed or disclosed
 *   in any way  without Intel's  prior  express written  permission. No  license
 *   under  any patent, copyright  or  other intellectual property rights  in the
 *   Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
 *   implication, inducement,  estoppel or  otherwise.  Any  license  under  such
 *   intellectual  property  rights must  be express  and  approved  by  Intel in
 *   writing.
 *
 *   *Third Party trademarks are the property of their respective owners.
 *
 *   Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
 *   this  notice or  any other notice embedded  in Materials by Intel or Intel's
 *   suppliers or licensors in any way.
 *
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef _MSC_VER
# include <Shlobj.h>
#else
# include <unistd.h>
# include <pwd.h>
# define MAX_PATH FILENAME_MAX
#endif

#include "sgx_urts.h"
#include "App.h"
#include "Enclave_u.h"
#include "Seal_u.h"

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

typedef struct _sgx_errlist_t {
    sgx_status_t err;
    const char *msg;
    const char *sug; /* Suggestion */
} sgx_errlist_t;

/* Error code returned by sgx_create_enclave */
static sgx_errlist_t sgx_errlist[] = {
    {
        SGX_ERROR_UNEXPECTED,
        "Unexpected error occurred.",
        NULL
    },
    {
        SGX_ERROR_INVALID_PARAMETER,
        "Invalid parameter.",
        NULL
    },
    {
        SGX_ERROR_OUT_OF_MEMORY,
        "Out of memory.",
        NULL
    },
    {
        SGX_ERROR_ENCLAVE_LOST,
        "Power transition occurred.",
        "Please refer to the sample \"PowerTransition\" for details."
    },
    {
        SGX_ERROR_INVALID_ENCLAVE,
        "Invalid enclave image.",
        NULL
    },
    {
        SGX_ERROR_INVALID_ENCLAVE_ID,
        "Invalid enclave identification.",
        NULL
    },
    {
        SGX_ERROR_INVALID_SIGNATURE,
        "Invalid enclave signature.",
        NULL
    },
    {
        SGX_ERROR_OUT_OF_EPC,
        "Out of EPC memory.",
        NULL
    },
    {
        SGX_ERROR_NO_DEVICE,
        "Invalid SGX device.",
        "Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."
    },
    {
        SGX_ERROR_MEMORY_MAP_CONFLICT,
        "Memory map conflicted.",
        NULL
    },
    {
        SGX_ERROR_INVALID_METADATA,
        "Invalid enclave metadata.",
        NULL
    },
    {
        SGX_ERROR_DEVICE_BUSY,
        "SGX device was busy.",
        NULL
    },
    {
        SGX_ERROR_INVALID_VERSION,
        "Enclave version was invalid.",
        NULL
    },
    {
        SGX_ERROR_INVALID_ATTRIBUTE,
        "Enclave was not authorized.",
        NULL
    },
    {
        SGX_ERROR_ENCLAVE_FILE_ACCESS,
        "Can't open enclave file.",
        NULL
    },
    {
        SGX_ERROR_NDEBUG_ENCLAVE,
        "The enclave is signed as product enclave, and can not be created as debuggable enclave.",
        NULL
    },
	{
		SGX_ERROR_PCL_ENCRYPTED,
		"The enclave is encrypted and loaded with wrong API or wrong parameters",
		NULL
	},
	{
		SGX_ERROR_PCL_NOT_ENCRYPTED,
		"The enclave is not encrypted and loaded with wrong API or wrong parameters",
		NULL
	},
};

/* Check error conditions for loading enclave */
void print_error_message(sgx_status_t ret)
{
    size_t idx = 0;
    size_t ttl = sizeof sgx_errlist/sizeof sgx_errlist[0];

    for (idx = 0; idx < ttl; idx++) {
        if(ret == sgx_errlist[idx].err) {
            if(NULL != sgx_errlist[idx].sug)
                printf("Info: %s\n", sgx_errlist[idx].sug);
            printf("Error: %s\n", sgx_errlist[idx].msg);
            break;
        }
    }
    
    if (idx == ttl)
    	printf("Error code is 0x%X. Please refer to the \"Intel SGX SDK Developer Reference\" for more details.\n", ret);
}

/* Initialize the enclave:
 *   Step 1: try to retrieve the launch token saved by last transaction
 *   Step 2: call sgx_create_enclave to initialize an enclave instance
 *   Step 3: save the launch token if it is updated
 */
int initialize_enclave(const char* token_filename, const char* enclave_file_name, uint8_t* sealed_key_blob, sgx_enclave_id_t* enclave_id)
{
    char token_path[MAX_PATH] = {'\0'};
    sgx_launch_token_t token = {0};
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    int updated = 0;
    
    /* Step 1: try to retrieve the launch token saved by last transaction 
     *         if there is no token, then create a new one.
     */
#ifdef _MSC_VER
    /* try to get the token saved in CSIDL_LOCAL_APPDATA */
    if (S_OK != SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, token_path)) {
        strncpy_s(token_path, _countof(token_path), token_filename, strlen(token_filename));
    } else {
        strncat_s(token_path, _countof(token_path), "\\", 2); 
		strncat_s(token_path, _countof(token_path), token_filename, strlen(token_filename));
    }

    /* open the token file */
    HANDLE token_handler = CreateFileA(token_path, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
    if (token_handler == INVALID_HANDLE_VALUE) {
        printf("Warning: Failed to create/open the launch token file \"%s\".\n", token_path);
    } else {
        /* read the token from saved file */
        DWORD read_num = 0;
        ReadFile(token_handler, token, sizeof(sgx_launch_token_t), &read_num, NULL);
        if (read_num != 0 && read_num != sizeof(sgx_launch_token_t)) {
            /* if token is invalid, clear the buffer */
            memset(&token, 0x0, sizeof(sgx_launch_token_t));
            printf("Warning: Invalid launch token read from \"%s\".\n", token_path);
        }
    }
#else /* __GNUC__ */
    /* try to get the token saved in $HOME */
    const char *home_dir = getpwuid(getuid())->pw_dir;
    
    if (home_dir != NULL && 
        (strlen(home_dir)+strlen("/")+ strlen(token_filename)+1) <= MAX_PATH) {
        /* compose the token path */
        strncpy(token_path, home_dir, strlen(home_dir));
        strncat(token_path, "/", strlen("/"));
        strncat(token_path, token_filename, strlen(token_filename)+1);
    } else {
        /* if token path is too long or $HOME is NULL */
        strncpy(token_path, token_filename, strlen(token_filename));
    }

    FILE *fp = fopen(token_path, "rb");
    if (fp == NULL && (fp = fopen(token_path, "wb")) == NULL) {
        printf("Warning: Failed to create/open the launch token file \"%s\".\n", token_path);
    }

    if (fp != NULL) {
        /* read the token from saved file */
        size_t read_num = fread(token, 1, sizeof(sgx_launch_token_t), fp);
        if (read_num != 0 && read_num != sizeof(sgx_launch_token_t)) {
            /* if token is invalid, clear the buffer */
            memset(&token, 0x0, sizeof(sgx_launch_token_t));
            printf("Warning: Invalid launch token read from \"%s\".\n", token_path);
        }
    }
#endif
    /* Step 2: call sgx_create_enclave to initialize an enclave instance */
    /* Debug Support: set 2nd parameter to 1 */
	const uint32_t ex_features = sealed_key_blob ? SGX_CREATE_ENCLAVE_EX_PCL : 0;
	const void* ex_features_p[32] = {};
	ex_features_p[SGX_CREATE_ENCLAVE_EX_PCL_BIT_IDX] = sealed_key_blob;
    ret = sgx_create_enclave_ex(enclave_file_name, SGX_DEBUG_FLAG, &token, &updated, enclave_id, NULL, ex_features, ex_features_p);
    if (ret != SGX_SUCCESS) {
        print_error_message(ret);
#ifdef _MSC_VER
        if (token_handler != INVALID_HANDLE_VALUE)
            CloseHandle(token_handler);
#else
        if (fp != NULL) fclose(fp);
#endif
        return -1;
    }

    /* Step 3: save the launch token if it is updated */
#ifdef _MSC_VER
    if (updated == FALSE || token_handler == INVALID_HANDLE_VALUE) {
        /* if the token is not updated, or file handler is invalid, do not perform saving */
        if (token_handler != INVALID_HANDLE_VALUE)
            CloseHandle(token_handler);
        return 0;
    }
    
    /* flush the file cache */
    FlushFileBuffers(token_handler);
    /* set access offset to the begin of the file */
    SetFilePointer(token_handler, 0, NULL, FILE_BEGIN);

    /* write back the token */
    DWORD write_num = 0;
    WriteFile(token_handler, token, sizeof(sgx_launch_token_t), &write_num, NULL);
    if (write_num != sizeof(sgx_launch_token_t))
        printf("Warning: Failed to save launch token to \"%s\".\n", token_path);
    CloseHandle(token_handler);
#else /* __GNUC__ */
    if (updated == FALSE || fp == NULL) {
        /* if the token is not updated, or file handler is invalid, do not perform saving */
        if (fp != NULL) fclose(fp);
        return 0;
    }

    /* reopen the file with write capablity */
    fp = freopen(token_path, "wb", fp);
    if (fp == NULL) return 0;
    size_t write_num = fwrite(token, 1, sizeof(sgx_launch_token_t), fp);
    if (write_num != sizeof(sgx_launch_token_t))
        printf("Warning: Failed to save launch token to \"%s\".\n", token_path);
    fclose(fp);
#endif
    return 0;
}

/* OCall functions */
void ocall_print_string(const char *str)
{
    /* Proxy/Bridge will check the length and null-terminate 
     * the input string to prevent buffer overflow. 
     */
    printf("%s", str);
}

/* Application entry */
int SGX_CDECL main(int argc, char *argv[])
{
    (void)(argc);
    (void)(argv);

	uint8_t* sealed_key_blob = NULL;
#ifndef SGX_SIM // Currently, Intel(R) SGX PCL does not support simulation mode
	bool run_seal_enclave = true;
	char sealed_key_blob_path[MAX_PATH] = { '\0' };
	strncat_s(sealed_key_blob_path, sizeof(SEALED_KEY_FILE_NAME), SEALED_KEY_FILE_NAME, sizeof(SEALED_KEY_FILE_NAME));
	/* Try to read the sealed key from file */
    HANDLE sealed_key_handler = CreateFileA(sealed_key_blob_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (sealed_key_handler != INVALID_HANDLE_VALUE) {
		/* read the sealed key from saved file */
		DWORD read_num = 0;
		DWORD sealed_key_blob_file_size = GetFileSize(sealed_key_handler, NULL);
		if (INVALID_FILE_SIZE == sealed_key_blob_file_size || 0 == sealed_key_blob_file_size)
		{
			printf("Failed to check file size for %s\n", sealed_key_blob_path);
		}
		else
		{
            sealed_key_blob = new uint8_t[sealed_key_blob_file_size];
			ReadFile(sealed_key_handler, sealed_key_blob, sealed_key_blob_file_size, &read_num, NULL);
			if (read_num != sealed_key_blob_file_size) {
				printf("Failed to read %s\n", sealed_key_blob_path);
                delete[] sealed_key_blob;
                sealed_key_blob = NULL;
			}
			else
			{
				run_seal_enclave = false;
			}
		}
		CloseHandle(sealed_key_handler);	
	}
	sealed_key_handler = 0;

	if(true == run_seal_enclave)
	{
		// Get the sealed key from ISV server
		sgx_enclave_id_t seal_eid = 0;
		size_t sealed_blob_size = 0;
		if (initialize_enclave(SEALING_TOKEN_FILENAME, SEALING_ENCLAVE_FILENAME, NULL, &seal_eid) < 0) {
			printf("Failed to initialize %s, enter a character before exit ...\n", SEALING_ENCLAVE_FILENAME);
			getchar();
			return -1;
		}
		sgx_status_t ret = ecall_get_sealed_blob_size(seal_eid, &sealed_blob_size);
		if (ret != SGX_SUCCESS || UINT32_MAX == sealed_blob_size)
		{
			printf("ecall_get_sealed_blob_size: ret = %d, sealed_blob_size = %ld\n", ret, (long)sealed_blob_size);
			sgx_destroy_enclave(seal_eid);
			return -1;
		}
		sealed_key_blob = new uint8_t[sealed_blob_size];
		sgx_status_t gret = SGX_ERROR_UNEXPECTED;
		ret = ecall_generate_sealed_blob(seal_eid, &gret, sealed_key_blob, sealed_blob_size);
		sgx_destroy_enclave(seal_eid);
		if ((SGX_SUCCESS != ret) || (SGX_SUCCESS != gret))
		{
			printf("ecall_generate_sealed_blob: ret = %d, gret = 0x%x\n", ret, gret);
			delete[] sealed_key_blob;
			return -1;
		}

		/* write back the token */
		sealed_key_handler = CreateFileA(sealed_key_blob_path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
		if (sealed_key_handler != INVALID_HANDLE_VALUE) 
		{
			DWORD write_num = 0;
			WriteFile(sealed_key_handler, sealed_key_blob, (DWORD)sealed_blob_size, &write_num, NULL);
			if (write_num != sealed_blob_size)
			{
				printf("Warning: Failed to save sealed key blob to \"%s\".\n", sealed_key_blob_path);
			}
			CloseHandle(sealed_key_handler);
		}
		sealed_key_handler = 0;
	}

#endif // #ifdef SGX_SIM
    /* Initialize the enclave */
    if(initialize_enclave(TOKEN_FILENAME, ENCLAVE_FILENAME, sealed_key_blob, &global_eid) < 0){
        delete[] sealed_key_blob;
        printf("Enter a character before exit ...\n");
        getchar();
        return -1; 
    }
 
    /* Utilize edger8r attributes */
    edger8r_array_attributes();
    edger8r_pointer_attributes();
    edger8r_type_attributes();
    edger8r_function_attributes();
    
    /* Utilize trusted libraries */
    ecall_libc_functions();
    ecall_libcxx_functions();
    ecall_thread_functions();

    /* Destroy the enclave */
    sgx_destroy_enclave(global_eid);
    
    delete[] sealed_key_blob;
    printf("Info: SampleEnclave successfully returned.\n");

    printf("Enter a character before exit ...\n");
    getchar();
    return 0;
}

