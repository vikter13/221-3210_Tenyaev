/*
 *   Copyright(C) 2019 Intel Corporation All Rights Reserved.
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

#ifndef _SGX_SECURE_ALIGN_API_H_
#define _SGX_SECURE_ALIGN_API_H_

#include <stdint.h>

typedef struct
{
    size_t offset;
    size_t len;
} align_req_t;

#ifdef __cplusplus
extern "C" {
#endif
    /**
     * sgx_aligned_malloc
     *
     *     Allocates memory for a structure on a specified alignment boundary
     *
     * Parameters:
     *     size - the size of the requested memory allocation in bytes.
     *     alignment - the alignment value, which must be an integer power of 2.
     *     data - (offset, length) pairs to define the fields in the structure for secrets
     *             If data is NULL and count is 0, the whole structure will be aligned.
     *     count - number of align_req_t structure in data
     *             If data is NULL and count is 0, the whole structure will be aligned.
     *
     * Return Value:
     *     A pointer to the memory block that was allocated or NULL if the operation failed.
    */
    void *sgx_aligned_malloc(size_t size, size_t alignment, align_req_t *data, size_t count);
    /**
     * sgx_aligned_free
     *
     *     Frees a block of memory that was allocated with sgx_aligned_malloc
     *
     * Parameters:
     *     ptr - a pointer to the memory block that was returned to the sgx_aligned_malloc
     *
    */
    void sgx_aligned_free(void *ptr);

    /*
     * sgx_get_aligned_ptr
     *
     *     Return a pointer from the pre-allocated memory on a specified alignment boundary
     *
     * Parameters:
     *     raw - the memory allocated by user
     *     raw_size - the size of raw memory in bytes
     *     allocate_size - the size of the requested memory allocation in bytes.
     *     alignment - the alignment value, which must be an integer power of 2.
     *     data - (offset, length) pairs to define the fields in the structure for secrets
     *             If data is NULL and count is 0, the whole structure will be aligned.
     *     count - number of align_req_t structure in data
     *             If data is NULL and count is 0, the whole structure will be aligned.
     * Return Value:
     *     A pointer to the memory block or NULL if the operation failed.
     * Note:
     *     The raw memory should be allocated by user, and it should be big enough to get aligned pointer:
     *          (size + 72)(bytes), if alignment <= 8
     *          (size + 64 + alignment)(bytes), if alignment > 8
     *
     */
    void *sgx_get_aligned_ptr(void *raw, size_t raw_size, size_t allocate_size, size_t alignment, align_req_t *data, size_t count);

#ifdef __cplusplus
}
#endif

#endif