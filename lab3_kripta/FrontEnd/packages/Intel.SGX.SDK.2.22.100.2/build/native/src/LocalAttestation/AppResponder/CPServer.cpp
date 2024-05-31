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

#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include <winsock2.h>
#pragma warning(disable:4996) 
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#endif

#include "CPServer.h"

#define BACKLOG 5     
#define CONCURRENT_MAX 32   
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

#define UNIX_DOMAIN "/tmp/UNIX.domain"

/* Function Description:
 * This is server initialization routine, it creates TCP sockets and listen on a port.
 * In Linux, it would listen on domain socket named '/tmp/UNIX.domain'
 * In Windows, it would listen on port 8888, which is for demonstration purpose 
 * */
int CPServer::init()
{
#ifdef _MSC_VER
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;

    if (WSAStartup(sockVersion, &data) != 0)
    {
        return 0;
    }

    m_server_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_server_sock_fd == INVALID_SOCKET)
    {
        printf("socket error !");
        WSACleanup();
        return 0;
    }

    sockaddr_in sin;
    memset(&sin, 0, sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERVER_PORT);
    sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    int iResult;
    bool bReuseAddr = true;
    iResult = setsockopt(m_server_sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&bReuseAddr, sizeof(bReuseAddr));
    if (SOCKET_ERROR == iResult)
    {
        printf("Failed to set resueaddr socket!\n");
        WSACleanup();
        return -1;
    }

    if (bind(m_server_sock_fd, (sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !");
        WSACleanup();
        return -1;
    }
    
    if (listen(m_server_sock_fd, 5) != 0)
    {
        printf("listen error !");
        WSACleanup();
        return 0;
    }

#else
    struct sockaddr_un srv_addr;
    
    m_server_sock_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if (m_server_sock_fd == -1)
    {
        printf("socket initiazation error\n");
        return -1;
    }

    srv_addr.sun_family = AF_UNIX;
    strncpy(srv_addr.sun_path, UNIX_DOMAIN, sizeof(srv_addr.sun_path)-1);
    unlink(UNIX_DOMAIN);

    int bind_result = bind(m_server_sock_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if (bind_result == -1)
    {
        printf("bind error\n");
        close(m_server_sock_fd);
        return -1;
    }
    
    if (listen(m_server_sock_fd, BACKLOG) == -1)
    {
        printf("listen error");
        close(m_server_sock_fd);
        return -1;
    }
#endif

    m_shutdown = 0;

    return 0;
}

/* Function Description:
 * This function is server's major routine, it uses select() to accept new connection and receive messages from clients.
 * When it receives clients' request messages, it would put the message to task queue and wake up worker thread to process the requests.
 * */
void CPServer::doWork()
{
#ifdef _MSC_VER
    fd_set server_fd_set;   
    int max_fd = -1;
    struct timeval tv;  
    char recv_msg[BUFFER_SIZE];

    // set 2s timeout value for select()
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    while (!m_shutdown)
    {
        // listening on server socket
        FD_ZERO(&server_fd_set);
        FD_SET(m_server_sock_fd, &server_fd_set);

        int ret = select(0, &server_fd_set, NULL, NULL, &tv);
        if (ret == SOCKET_ERROR) {
            Sleep(100);
            continue;
        }

        // timeout 
        if (ret == 0)
            continue;           

        // there is new connection request or new client requests
        for (unsigned int i = 0; i < server_fd_set.fd_count; i++)
        {
            if (!FD_ISSET(server_fd_set.fd_array[i], &server_fd_set))
                continue;

            if (server_fd_set.fd_array[i] == m_server_sock_fd)
            {
                // there is new connection request
                struct sockaddr_in clt_addr;
                int len = sizeof(clt_addr);

                // accept this new connection request and add to listen to this client
                SOCKET client_sock_fd = accept(m_server_sock_fd, (struct sockaddr *)&clt_addr, &len);
                if (client_sock_fd != INVALID_SOCKET) {
                    FD_SET(client_sock_fd, &server_fd_set);

                    printf("%s:%d has connected server!\n", inet_ntoa(clt_addr.sin_addr),
                        ntohs(clt_addr.sin_port));
                }
            }
            else
            {
                // there is message request from this client connection
                FIFO_MSG * msg;

                memset(recv_msg, 0, BUFFER_SIZE);
                long byte_num = recv(server_fd_set.fd_array[i], recv_msg, BUFFER_SIZE, 0);
                if (byte_num > 0) {
                    if (byte_num > BUFFER_SIZE)
                        byte_num = BUFFER_SIZE;

                    recv_msg[byte_num] = '\0';

                    msg = (FIFO_MSG *)malloc(byte_num);
                    if (!msg) {
                        printf("memory allocation failure");
                        continue;
                    }
                    memset(msg, 0, byte_num);

                    memcpy(msg, recv_msg, byte_num);

                    msg->header.sockfd = (int)server_fd_set.fd_array[i];

                    m_cptask->puttask(msg);
                }
                else {
                    //socket error or connection is closed
                    FD_CLR(server_fd_set.fd_array[i], &server_fd_set);
                    closesocket(server_fd_set.fd_array[i]);                 
                }

            }
        }
    }
#else
    int client_fds[CONCURRENT_MAX] = {0};
    fd_set server_fd_set;
    int max_fd = -1;
    struct timeval tv;  
    char input_msg[BUFFER_SIZE];
    char recv_msg[BUFFER_SIZE];
    
    while (!m_shutdown)
    {
        // set 20s timeout for select() 
        tv.tv_sec = 20;
        tv.tv_usec = 0;
        FD_ZERO(&server_fd_set);

        FD_SET(STDIN_FILENO, &server_fd_set);
        if (max_fd <STDIN_FILENO)
            max_fd = STDIN_FILENO;
        
        // listening on server socket
        FD_SET(m_server_sock_fd, &server_fd_set);
        if (max_fd < m_server_sock_fd)
            max_fd = m_server_sock_fd;
    
        // listening on all client connections
        for(int i =0; i < CONCURRENT_MAX; i++) {
            if(client_fds[i] != 0) {
                FD_SET(client_fds[i], &server_fd_set);
                if(max_fd < client_fds[i])
                    max_fd = client_fds[i];
            }
        }
        
        int ret = select(max_fd + 1, &server_fd_set, NULL, NULL, &tv);
        if(ret < 0) {
            printf("Warning: server would shutdown\n");
            continue;
        } else if(ret == 0) {
            // timeout 
            continue;
        } 
    
        if(FD_ISSET(m_server_sock_fd, &server_fd_set)) {
            // if there is new connection request
            struct sockaddr_un clt_addr;
            socklen_t len = sizeof(clt_addr);
        
            // accept this connection request
            int client_sock_fd = accept(m_server_sock_fd, (struct sockaddr *)&clt_addr, &len);
                
            if (client_sock_fd > 0) {
                // add new connection to connection pool if it's not full
                int index = -1;
                for(int i = 0; i < CONCURRENT_MAX; i++) {
                    if(client_fds[i] == 0) {
                        index = i;
                        client_fds[i] = client_sock_fd;
                        break;
                    }
                }
                    
                if(index < 0) {
                    printf("server reach maximum connection!\n");
                    bzero(input_msg, BUFFER_SIZE);
                    strcpy(input_msg, "server reach maximum connection\n");
                    send(client_sock_fd, input_msg, BUFFER_SIZE, 0);
                }
                }else if (client_sock_fd < 0) {
                    printf("server: accept() return failure, %s, would exit.\n", strerror(errno));
                close(m_server_sock_fd);
                break;
            }
        }
            
        for(int i =0; i < CONCURRENT_MAX; i++) {
            if ((client_fds[i] !=0)
               && (FD_ISSET(client_fds[i], &server_fd_set))) 
            {
                // there is request messages from client connectsions
                FIFO_MSG * msg; 

                bzero(recv_msg, BUFFER_SIZE);
                long byte_num = recv(client_fds[i], recv_msg, BUFFER_SIZE, 0);
                if (byte_num > 0) {
                    if(byte_num > BUFFER_SIZE)
                        byte_num = BUFFER_SIZE; 
                            
                    recv_msg[byte_num] = '\0';

                    msg = (FIFO_MSG *)malloc(byte_num);
                    if (!msg) {
                        printf("memory allocation failure");
                        continue;
                    }
                    memset(msg, 0, byte_num);
                        
                    memcpy(msg, recv_msg, byte_num);

                    msg->header.sockfd = client_fds[i];
                    
                    // put request message to event queue
                    m_cptask->puttask(msg);
                }
                else if(byte_num < 0) {
                        printf("failed to receive message.\n");
                } else {
                    // client connect is closed
                    FD_CLR(client_fds[i], &server_fd_set);
                    close(client_fds[i]);
                    client_fds[i] = 0;
                }
            }
        }
    }
#endif
}

/* Function Description:
 * This function is to shutdown server. It's called when process exits.
 * */
void CPServer::shutDown()
{
    printf("Server would shutdown...\n");
    m_shutdown = 1;
    m_cptask->shutdown();
        
#ifdef _MSC_VER
    closesocket(m_server_sock_fd);  
#else
    close(m_server_sock_fd);
#endif
}
