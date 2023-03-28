#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

short socket_create(void);
int bind_created_socket(int h_socket);


#endif