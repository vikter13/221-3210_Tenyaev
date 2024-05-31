/**
*
* Copyright(c) 2011-2016 Intel Corporation All Rights Reserved.
*
* The source code contained or described herein and all documents related to
* the source code ("Material") are owned by Intel Corporation or its suppliers
* or licensors. Title to the Material remains with Intel Corporation or its
* suppliers and licensors. The Material contains trade secrets and proprietary
* and confidential information of Intel or its suppliers and licensors. The
* Material is protected by worldwide copyright and trade secret laws and treaty
* provisions. No part of the Material may be used, copied, reproduced, modified,
* published, uploaded, posted, transmitted, distributed, or disclosed in any
* way without Intel's prior express written permission.
*
* No license under any patent, copyright, trade secret or other intellectual
* property right is granted to or conferred upon you by disclosure or delivery
* of the Materials, either expressly, by implication, inducement, estoppel or
* otherwise. Any license under such intellectual property rights must be
* express and approved by Intel(R) in writing.
*
*/

#ifndef _SWITCHLESS_ITF_H_
#define _SWITCHLESS_ITF_H_

#include "sgx_eid.h"
#include "sgx_error.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef sgx_status_t(*sl_init_func_t)(sgx_enclave_id_t, const void*, void**);
typedef sgx_status_t(*sl_ecall_func_t)(void*, const unsigned int, void*, int*);
typedef sgx_status_t(*sl_destroy_func_t)(void*);
typedef void(*sl_ocall_fallback_func_t)(void*);
typedef sgx_status_t(*sl_on_first_ecall_func_t)(void*, sgx_enclave_id_t, const void*);

typedef struct
{
    sl_init_func_t sl_init_func_ptr;
    sl_ecall_func_t sl_ecall_func_ptr;
    sl_destroy_func_t sl_destroy_func_ptr;
    sl_ocall_fallback_func_t sl_ocall_fallback_func_ptr;
    sl_on_first_ecall_func_t sl_on_first_ecall_func_ptr;

} sgx_switchless_funcs_t;


typedef void(*sgx_set_switchless_itf_func_t)(const sgx_switchless_funcs_t*);

#define SL_SET_SWITCHLESS_INTERFACE_FUNC_NAME "sgx_set_switchless_itf"

#ifdef __cplusplus
}
#endif


#endif