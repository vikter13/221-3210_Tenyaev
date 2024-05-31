/**
*
* Copyright(c) 2011-2018 Intel Corporation All Rights Reserved.
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

#ifndef _SGX_USWITCHLESS_H_
#define _SGX_USWITCHLESS_H_

/*
 * Switchless SGX
 *
 * Motivation. Enclave switches are very expensive. They are mostly triggered
 * by making ECalls or OCalls. For Intel(R)SGX applications that rely on ECalls/OCalls
 * heavily, enclave switches can degrade the performance significantly.
 *
 * Overview. Switchless SGX offers switchless features that eliminiate enclave
 * switches from SGX applications. Switchless SGX addresses this issue in two
 * approaches: 1) make ECalls/OCalls themselves switchless, which are called
 * switchless ECalls/OCalls; and 2) get rid of OCalls by providing switchless
 * implementation for common operations inside enclaves (e.g., thread-related
 * operations, file I/O operations, system clock, etc.).
 *
 * Note. This version of implementation only provides switchless ECalls/OCalls.
 *
 * switchless ECalls/OCalls. In EDL file, ECalls/OCalls that are annotated with
 * keyword transition_using_threads can be accelerated by executing them in a switchless way. SGX
 * application developers are recommended to only label ECalls/OCalls as switchless
 * on condition that they are short, non-blocking and may be called frequently.
 * To be switchless, switchless ECalls/OCalls (when appropriate) are executed by
 * worker threads.
 *
 * Worker threads. Each enclave has a set of worker threads, each of which is
 * either trusted or untrusted. The trusted worker threads are responsible for
 * handling requests made by the non-enclave code; the untrusted ones are *
 * responsible for handling requests made by the enclave code. The requests
 * and responses are passed through some shared data structures that are
 * accessible by both the non-enclave and enclave halves of a Intel(R)SGX program.
 * Thanks to these worker threads, requests can be processed without performing
 * enclave switches. While a on-going request is submitted, the
 * caller (trusted or untrusted) thread will busy wait for the completion of
 * the request by an worker. Worker threads are managed by Switchless SGX; that
 * is, Intel(R)SGX application developers do not have to start, terminate, schedule
 * these threads manually.
 */

#include "sgx_error.h"
#include "sgx_eid.h"
#include "sgx_defs.h"

/*
 * A worker can be either trusted (executed inside enclave) or untrusted
 * (executed outside enclave)
 */
typedef enum {
    SGX_USWITCHLESS_WORKER_TYPE_UNTRUSTED,
    SGX_USWITCHLESS_WORKER_TYPE_TRUSTED
} sgx_uswitchless_worker_type_t;

/*
 * There are 4 worker events that may be of interest to users.
 */
typedef enum {
    SGX_USWITCHLESS_WORKER_EVENT_START, /* a worker thread starts */
    SGX_USWITCHLESS_WORKER_EVENT_IDLE, /* a worker thread is idle */
    SGX_USWITCHLESS_WORKER_EVENT_MISS, /* a worker thread misses some tasks */
    SGX_USWITCHLESS_WORKER_EVENT_EXIT, /* a worker thread exits */
    _SGX_USWITCHLESS_WORKER_EVENT_NUM,
} sgx_uswitchless_worker_event_t;

#if defined(_MSC_VER)
#	define SE_WIN
#	ifndef _M_X64	//32bit
#		define SE_32
#		define SE_WIN32
#	else			//64bit
#		define SE_64
#		define SE_WIN64
#	endif
#elif defined(__GNUC__)
#	define SE_GNU
#	if defined(__x86_64__)
#		define SE_64
#		define SE_GNU64
#	else
#		define SE_32
#		define SE_GNU32
#	endif
#endif

/*
 * Statistics of workers
 */
#ifdef SE_64
typedef struct {
    uint64_t processed; /* # of tasks that all workers have processed */
    uint64_t missed;    /* # of tasks that all workers have missed */
} sgx_uswitchless_worker_stats_t;
#else
typedef struct {
    uint32_t processed; /* # of tasks that all workers have processed */
    uint32_t missed;    /* # of tasks that all workers have missed */
} sgx_uswitchless_worker_stats_t;
#endif //SE_64

/*
 * Worker callbacks enables user to customize some behaviours of workers.
 */
typedef void (*sgx_uswitchless_worker_callback_t)(
                sgx_uswitchless_worker_type_t /* type */,
                sgx_uswitchless_worker_event_t /* event */,
                const sgx_uswitchless_worker_stats_t* /* stats */);


/*
 * Configuration for Switchless SGX
 */

#define SL_DEFAULT_FALLBACK_RETRIES  20000
#define SL_DEFAULT_SLEEP_RETRIES     20000
#define SL_DEFUALT_MAX_TASKS_QWORDS  1   //64
#define SL_MAX_TASKS_MAX_QWORDS      8   //512

#ifdef SE_64
typedef uint64_t uint_type;
#else
typedef uint32_t uint_type;
#endif //SE_64

typedef struct 
{
	uint_type                            switchless_calls_pool_size_qwords; //number of qwords to use for outstanding calls bitmaps. (actual number is x 64)

    uint_type                            num_uworkers;  //number of untrusted (for ocalls) worker threads

    uint_type                            num_tworkers;  //number of trusted (for ecalls) worker threads

	uint_type                            retries_before_fallback; //how many times to execute assembly pause instruction 
                                                                 //while waiting for worker thread to start executing switchless call 
                                                                 //before falling back to direct ECall/OCall. 

	uint_type                            retries_before_sleep;    //how many times a worker thread executes assembly pause instruction
                                                                 //while waiting for switchless call request 
                                                                 //before going to sleep
    
    uint_type                            unused;                 // for alignment purpose

    sgx_uswitchless_worker_callback_t   callback_func[_SGX_USWITCHLESS_WORKER_EVENT_NUM]; //array of pointers to callback functions.
} sgx_uswitchless_config_t;

#define SGX_USWITCHLESS_CONFIG_INITIALIZER    {0, 1, 1, 0, 0, { 0 } }






#endif /* _SGX_USWITCHLESS_H_ */
