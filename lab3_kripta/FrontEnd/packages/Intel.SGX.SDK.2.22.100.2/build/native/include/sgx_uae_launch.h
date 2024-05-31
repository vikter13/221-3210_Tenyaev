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

#pragma once

#ifndef _SGX_UAE_LAUNCH_H_
#define _SGX_UAE_LAUNCH_H_

#include <stdint.h>
#if defined(_MSC_VER)
#include <windows.h>
#endif

#include "sgx_error.h"
#include "sgx_urts.h"
#include "sgx_status.h"
#include "sgx_capable.h"


#ifdef  __cplusplus
extern "C" {
#endif

/*
 * Function used to query and enable SGX device.
 *
 * @param sgx_device_status[out] The status of SGX device.
 * @return If the function succeeds, return SGX_SUCCESS, any other value indicates an error.
 */
sgx_status_t SGXAPI sgx_enable_device(sgx_device_status_t* sgx_device_status);

/*
 * Function used to get whitelist size.
 *
 * @param p_whitelist_size[out] whitelist size, in bytes.
 * @return If whitelist size is returned,return SGX_SUCCESS, otherwise SGX_ERROR_UNEXPECTED
*             indicates current whitelist is invalid.
 */
sgx_status_t SGXAPI sgx_get_whitelist_size(
    uint32_t* p_whitelist_size);

/*
 * Function used to get whitelist.
 *
 * @param p_whitelist[out] The whitelist buffer.
 * @param whitelist_size[in] whitelist buffer size, in bytes.
 * @return If whitelist is retrieved, return SGX_SUCCESS, any other value indicates an error.
 */
sgx_status_t SGXAPI sgx_get_whitelist(
    uint8_t* p_whitelist,
    uint32_t whitelist_size);

#ifdef  __cplusplus
}
#endif

#endif
