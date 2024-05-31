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

#ifndef _SGX_UAE_PLATFORM_H_
#define _SGX_UAE_PLATFORM_H_

#include <stdint.h>
#if defined(_MSC_VER)
#include <windows.h>
#endif

#include "sgx_error.h"
#include "sgx_urts.h"


#define PS_CAP_TRUSTED_TIME         0x1
#define PS_CAP_MONOTONIC_COUNTER    0x2
#define PS_CAP_MESSAGE_EXCHANGE     0x8

/**
 * Platform service capabilities
 *      ps_cap0
 *       Bit 0 : Trusted Time
 *       Bit 1 : Monotonic Counter
 *       Bit 2 : Reserved
 *       Bit 3 : Message Exchange
 *       Bit 4-31 : Reserved
 *      ps_cap1
 *       Bit 0-31 : Reserved
 */
typedef struct _sgx_ps_cap_t
{
    uint32_t ps_cap0;
    uint32_t ps_cap1;
} sgx_ps_cap_t;

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * Get the platform service capabilities
 *
 * @param sgx_ps_cap Platform capabilities reported by AESM.
 * @return if OK, return SGX_SUCCESS
 */
sgx_status_t SGXAPI sgx_get_ps_cap(sgx_ps_cap_t* p_sgx_ps_cap);

#define SGX_IS_TRUSTED_TIME_AVAILABLE(cap)           ((((uint32_t)PS_CAP_TRUSTED_TIME)&((cap).ps_cap0))!=0)
#define SGX_IS_MONOTONIC_COUNTER_AVAILABLE(cap)      ((((uint32_t)PS_CAP_MONOTONIC_COUNTER)&((cap).ps_cap0))!=0)
#define SGX_IS_MESSAGE_EXCHANGE_AVAILABLE(cap)      ((((uint32_t)PS_CAP_MESSAGE_EXCHANGE)&((cap).ps_cap0))!=0)

#ifdef  __cplusplus
}
#endif

#endif
