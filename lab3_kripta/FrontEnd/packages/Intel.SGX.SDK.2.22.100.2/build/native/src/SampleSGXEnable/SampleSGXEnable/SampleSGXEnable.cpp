/*
 *   Copyright(C) 2011-2019 Intel Corporation All Rights Reserved.
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
#include "sgx_capable.h"

/* name: query_and_enable_sgx_device()
 * Usage: 1. It detects SGX device's status inside target environment.
 *        2. And it will enable SGX device when it detects that SGX was set as Software Control inside BIOS. 
 *           So it can be used inside desktop application installer to enable SGX in target environment during installation.
 * Note: This function need be executed with Administrator privilege.
*/
int query_and_enable_sgx_device()
{
	sgx_device_status_t sgx_device_status;
	sgx_status_t sgx_ret = sgx_cap_get_status(&sgx_device_status);
	if (sgx_ret != SGX_SUCCESS) {
		printf("Failed to get SGX device status.\n");
		return -1;
	}
	else {
		switch (sgx_device_status) {
		case SGX_ENABLED:
			printf("SGX device has been enabled.\n");
			break;
		case SGX_DISABLED_SCI_AVAILABLE:
			printf("SGX device can be enabled by a Software Control Interface, so enable it\n");
			sgx_ret = sgx_cap_enable_device(&sgx_device_status);
			if (sgx_ret != SGX_SUCCESS) {
				printf("Failed to Enable SGX device.\n");
				return -1;
			}
			switch (sgx_device_status)
			{
			case SGX_ENABLED:
				printf("SGX device has been enabled by Software Control Interface.\n");
				break;
			case SGX_DISABLED_REBOOT_REQUIRED:
				printf("SGX device has been enabled by Software Control Interface. Please reboot your machine.\n");
				break;
			case SGX_DISABLED_LEGACY_OS:
				printf("SGX device can't be enabled on an OS that doesn't support EFI interface.\n");
				break;
			case SGX_DISABLED:
				printf("SGX is not enabled on this platform. More details are unavailable.\n");
				break;
			case SGX_DISABLED_MANUAL_ENABLE:
				printf("SGX device can be enabled manually in the BIOS setup.\n");
				break;
			case SGX_DISABLED_HYPERV_ENABLED:
				printf("Detected an unsupported version of Windows* 10 with Hyper-V enabled.\n");
				break;
			case SGX_DISABLED_UNSUPPORTED_CPU:
				printf("SGX is not supported by this CPU.\n");
				break;
			default:
				printf("Unexpected error.\n");
				return -1;
			}
			break;
		case SGX_DISABLED_REBOOT_REQUIRED:
			printf("SGX device has been enabled. Please reboot your machine.\n");
			break;
		case SGX_DISABLED_LEGACY_OS:
			printf("SGX device can't be enabled on an OS that doesn't support EFI interface.\n");
			break;
		case SGX_DISABLED:
			printf("SGX is not enabled on this platform. More details are unavailable.\n");
			break;
		case SGX_DISABLED_MANUAL_ENABLE:
			printf("SGX device can be enabled manually in the BIOS setup.\n");
			break;
		case SGX_DISABLED_HYPERV_ENABLED:
			printf("Detected an unsupported version of Windows* 10 with Hyper-V enabled.\n");
			break;
		case SGX_DISABLED_UNSUPPORTED_CPU:
			printf("SGX is not supported by this CPU.\n");
			break;
		default:
			printf("Unexpected error.\n");
			return -1;
		}
	}
	return 0;
}

int main()
{
	query_and_enable_sgx_device();
	printf("Enter a character before exit ...\n");
	getchar();
	return 0;
}
