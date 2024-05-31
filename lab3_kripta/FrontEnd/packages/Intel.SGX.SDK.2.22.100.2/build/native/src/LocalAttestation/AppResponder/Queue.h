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

#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <queue>
#ifdef _MSC_VER
#else
#include <pthread.h>
#endif

template <typename T>
class Queue  {
    public:
        Queue();
        ~Queue();

        void push(T*);
        T* blockingPop();
        void close();

    private:
        std::queue<T*>  m_queue;
#ifdef _MSC_VER
		HANDLE            m_queueCond;
		HANDLE            m_queueMutex;
#else
        pthread_cond_t            m_queueCond;
        pthread_mutex_t           m_queueMutex;
#endif
        volatile  uint8_t         m_events;
    private:
        Queue& operator=(const Queue&);
        Queue(const Queue&);
};

#define QUEUE_EVENT_CLOSE    1

template<typename T>
Queue<T>::Queue() :
    m_queue()
{
#ifdef _MSC_VER
	m_queueMutex = CreateMutex(NULL, FALSE, "threadmutex");
	if (m_queueMutex == NULL)
		exit(-1);

	m_queueCond = CreateSemaphore(NULL, 0, 1024, "threadSem");
	if (m_queueCond == NULL)
		exit(-1);
#else
    int rc;

    rc = pthread_mutex_init(&m_queueMutex, NULL);
    if (rc != 0)
    {
	printf("failed to call pthread_mutex_init");
        exit(-1); 
    }

    rc = pthread_cond_init(&m_queueCond, NULL);
    if (rc != 0)
    {
	printf("failed to call pthread_cond_init.\n");
        exit(-1);
    }
#endif
    m_events = 0;
}

template<typename T>
Queue<T>::~Queue()
{
#ifdef _MSC_VER
	CloseHandle(m_queueMutex);
	CloseHandle(m_queueCond);
#else
    int rc;

    rc = pthread_mutex_destroy(&m_queueMutex);
    if (rc != 0)
        printf("Failed to destroy mutex");

    rc = pthread_cond_destroy(&m_queueCond);
    if (rc != 0)
        printf("Failed to destory a condition variable");
#endif
}

template<typename T>
void Queue<T>::push(T* value)
{
#ifdef _MSC_VER
	if (WAIT_OBJECT_0 != WaitForSingleObject(m_queueMutex, INFINITE))
		exit(-1);

	m_queue.push(value);

	ReleaseSemaphore(m_queueCond, 1, NULL);

	ReleaseMutex(m_queueMutex);
#else
    int rc;

    rc = pthread_mutex_lock(&m_queueMutex);
    if (rc != 0)
    {
        printf("Failed to acquire mutex");
        exit(-1);
    }
    m_queue.push(value);

    rc = pthread_cond_signal(&m_queueCond);
    if (rc != 0)
    {
        printf("Failed to signal condition");
        exit(-1);
    }

    rc = pthread_mutex_unlock(&m_queueMutex);
    if (rc != 0)
    {
        printf("Failed to unlock mutex");
        exit(-1);
    }
#endif
}

template<typename T> 
T* Queue<T>::blockingPop()
{
    
    T* value = NULL;

#ifdef _MSC_VER
	if (WAIT_OBJECT_0 != WaitForSingleObject(m_queueMutex, INFINITE))
		exit(-1);
#else
	int rc;
    rc = pthread_mutex_lock(&m_queueMutex);
    if (rc != 0)
    {
        printf("Failed to acquire mutex");
        exit(-1);
    }
#endif

    while(1) {
        if (m_events & QUEUE_EVENT_CLOSE) {			
            //epmty the queue
            while (!m_queue.empty())
            {
                value = m_queue.front();
                m_queue.pop();
                free(value);
            }
            value = NULL;			
            break;
        }
        if (!m_queue.empty()) {
            // queue is not empty
            value = m_queue.front();
            m_queue.pop();
            break;
        }
        else {
#ifdef _MSC_VER
			ReleaseMutex(m_queueMutex);

			if (WAIT_OBJECT_0 != WaitForSingleObject(m_queueCond, INFINITE))
				exit(-1);
						
			if (WAIT_OBJECT_0 != WaitForSingleObject(m_queueMutex, INFINITE))
				exit(-1);
						
#else
            rc = pthread_cond_wait(&m_queueCond, &m_queueMutex);

            if (rc != 0)
            {
                printf("Failed wait on a condition");
                exit(-1);
            }
#endif
        }
    }

#ifdef _MSC_VER
	ReleaseMutex(m_queueMutex);
#else
    rc = pthread_mutex_unlock(&m_queueMutex);
    if (rc != 0)
    {
        printf("Failed to unlock mutex");
        exit(-1);
    }
#endif

    return value;
}

template<typename T>
void Queue<T>::close()
{
#ifdef _MSC_VER
	if (WAIT_OBJECT_0 != WaitForSingleObject(m_queueMutex, INFINITE))
		exit(-1);
#else
    int rc;
    rc = pthread_mutex_lock(&m_queueMutex);
    if (rc != 0)
    {
        printf("Failed to acquire mutex");
        exit(-1);
    }
#endif

    m_events |= QUEUE_EVENT_CLOSE;

#ifdef _MSC_VER
	ReleaseSemaphore(m_queueCond, 1, NULL);

	ReleaseMutex(m_queueMutex);
#else
    rc = pthread_cond_signal(&m_queueCond);
    if (rc != 0)
    {
        printf("Failed to signal a condition");
        exit(-1);
    }

    rc = pthread_mutex_unlock(&m_queueMutex);
    if (rc != 0)
    {
        printf("Failed to unlock mutex");
        exit(-1);
    }
#endif
}

#endif
