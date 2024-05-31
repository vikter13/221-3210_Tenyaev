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

#include <stdarg.h>
#include <stdio.h>      /* vsnprintf */
#include "sgx_error.h"

#include "Enclave.h"
#include "Enclave_t.h"  /* print_string */

/* 
 * printf: 
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */
void printf(const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print_string(buf);
}

#define PAGE_SIZE 0x1000
sgx_status_t malloc_rwx_page_and_free()
{
    sgx_status_t error = SGX_SUCCESS;
    int retval = 0;
    void* page_mem = 0;
    int* number = 0;

    // Allocate 2 pages from the heap
    void *mem = malloc(2*PAGE_SIZE);
    if (!mem) {
        error = SGX_ERROR_OUT_OF_MEMORY;
        goto exit;
    }

    // Find the page boundary in order to change permissions on the page
    page_mem = (void*)(((size_t)mem + PAGE_SIZE) & ~(PAGE_SIZE - 1));

    // Change page_mem from RW to RWX
    retval = sgx_virtual_protect(page_mem, PAGE_SIZE, SGX_PAGE_EXECUTE_READWRITE, 0, &error);
    if (!retval) {
        goto exit;
    }

    // Do something with the memory
    number = (int*)page_mem;
    *number = 2;

    // Change page_mem back to default heap permissions before freeing
    retval = sgx_virtual_protect(page_mem, PAGE_SIZE, SGX_PAGE_READWRITE, 0, &error);
    if (!retval) {
        goto exit;
    }

exit:
    // Free the memory
    free(mem);
    return error;
}
