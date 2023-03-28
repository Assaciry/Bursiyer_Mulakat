#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Send-Receive max length over TCP
#define SR_MAX_LEN 600

short socket_create(void);
int socket_connect(int h_socket);
int socket_send(int h_socket, char* rqst, short len_rqst);
int socket_receive(int h_socket, char* response, short len_response);
int start_connections();

#endif