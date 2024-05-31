/*
 * Copyright (C) 2011-2019 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "Thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <new>
#ifdef _MSC_VER
#include <windows.h>
#include <process.h>
#else
#include <sched.h>
#include <sys/sysinfo.h>
#endif
/*--------------------------------------------------------------------------------------------------*/

Thread::Thread()
        : m_shutDown(false), m_thread(0) 
{
}

Thread::~Thread()
{
}

void Thread::start()
{
#ifdef _MSC_VER
	m_thread = (HANDLE)_beginthreadex(NULL, 0, Thread::doWork, this, 0, NULL);
#else
    int rc = pthread_create(&m_thread, NULL, Thread::doWork, (void*)this);
    assert(rc == 0);
    (void)rc;
#endif
}

void Thread::stop()
{
    m_shutDown = true;
}

bool Thread::isStopped()
{
    return m_shutDown;
}


#ifdef _MSC_VER
unsigned int __stdcall Thread::doWork(void* param)
#else
void* Thread::doWork(void* param)
#endif
{
    try
    {
        Thread* thread = static_cast<Thread*>(param);
        thread->run();
    }
    catch(std::bad_alloc& allocationException)
    {
        printf("Unable to allocate memory");
		(void)allocationException;
        throw;
    }

    return NULL;
};


void Thread::join()
{
#ifdef _MSC_VER	
	if (NULL != m_thread)
	{
		if (WAIT_OBJECT_0 != WaitForSingleObject(m_thread, INFINITE))
			exit(-1);
		CloseHandle(m_thread);	
		m_thread = NULL;
	}	
#else
    void* res;
    pthread_join(m_thread, &res);
#endif
}

/*--------------------------------------------------------------------------------------------------*/
