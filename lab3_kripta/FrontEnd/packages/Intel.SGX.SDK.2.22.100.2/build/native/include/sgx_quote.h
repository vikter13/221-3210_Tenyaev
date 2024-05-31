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


 /**
  * File: sgx_quote.h
  * Description: Definition for quote structure.
  *
  * Quote structure and all relative structure will be defined in this file.
  */

#ifndef _SGX_QUOTE_H_
#define _SGX_QUOTE_H_

#include "sgx_report.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
typedef uint8_t sgx_epid_group_id_t[4];

typedef struct _spid_t
{
    uint8_t             id[16];
} sgx_spid_t;

typedef struct _basename_t
{
    uint8_t             name[32];
} sgx_basename_t;


typedef struct _quote_nonce
{
    uint8_t             rand[16];
} sgx_quote_nonce_t;

typedef enum
{
    SGX_UNLINKABLE_SIGNATURE,
    SGX_LINKABLE_SIGNATURE
} sgx_quote_sign_type_t;

typedef struct _quote_t
{
    uint16_t            version;                /* 0   */
    uint16_t            sign_type;              /* 2   */
    sgx_epid_group_id_t epid_group_id;          /* 4   */
    sgx_isv_svn_t       qe_svn;                 /* 8   */
    sgx_isv_svn_t       pce_svn;                /* 10  */
    uint32_t            xeid;                   /* 12  */
    sgx_basename_t      basename;               /* 16  */
    sgx_report_body_t   report_body;            /* 48  */
    uint32_t            signature_len;          /* 432 */
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning ( disable:4200 )
#endif
    uint8_t             signature[];            /* 436 */
#ifdef _MSC_VER
#pragma warning(pop)
#endif
} sgx_quote_t;

#define SGX_PLATFORM_INFO_SIZE 101
typedef struct _platform_info
{
    uint8_t platform_info[SGX_PLATFORM_INFO_SIZE];
} sgx_platform_info_t;

typedef struct _update_info_bit
{
    int ucodeUpdate;
    int csmeFwUpdate;
    int pswUpdate;
} sgx_update_info_bit_t;

typedef struct _att_key_id_t {
    uint8_t     att_key_id[256];
}sgx_att_key_id_t;

/** Describes a single attestation key.  Contains both QE identity and the attestation algorithm ID. */
typedef struct _sgx_ql_att_key_id_t {
    uint16_t    id;                              ///< Structure ID
    uint16_t    version;                         ///< Structure version
    uint16_t    mrsigner_length;                 ///< Number of valid bytes in MRSIGNER.
    uint8_t     mrsigner[48];                    ///< SHA256 or SHA384 hash of the Public key that signed the QE.
                                                 ///< The lower bytes contain MRSIGNER.  Bytes beyond mrsigner_length '0'
    uint32_t    prod_id;                         ///< Legacy Product ID of the QE
    uint8_t     extended_prod_id[16];            ///< Extended Product ID or the QE. All 0's for legacy format enclaves.
    uint8_t     config_id[64];                   ///< Config ID of the QE.
    uint8_t     family_id[16];                   ///< Family ID of the QE.
    uint32_t    algorithm_id;                    ///< Identity of the attestation key algorithm.
}sgx_ql_att_key_id_t;

/** Describes an extended attestation key.  Contains sgx_ql_att_key_id_t, spid and quote_type */
typedef struct _sgx_att_key_id_ext_t {
    sgx_ql_att_key_id_t base;
    uint8_t             spid[16];                ///< Service Provider ID, should be 0s for ECDSA quote
    uint16_t            att_key_type;            ///< For non-EPID quote, it should be 0
                                                 ///< For EPID quote, it equals to sgx_quote_sign_type_t
    uint8_t             reserved[80];            ///< It should have the same size of sgx_att_key_id_t
}sgx_att_key_id_ext_t;

typedef struct _qe_report_info_t {
    sgx_quote_nonce_t nonce;
    sgx_target_info_t app_enclave_target_info;
    sgx_report_t qe_report;
}sgx_qe_report_info_t;

#pragma pack(pop)


#ifdef __cplusplus
}
#endif

#endif

