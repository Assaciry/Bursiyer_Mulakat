#include "../inc/server_socket.h"


short socket_create(void){
    short h_socket;
    printf("Create the socket.\n");
    h_socket = socket(AF_INET, SOCK_STREAM, 0);
    return h_socket;
}

int bind_created_socket(int h_socket){
    int ret_val = -1;
    int client_port = 12345;
    
    struct sockaddr_in remote = {0};

    // Internet address family
    remote.sin_family = AF_INET;

    // Any incoming interface
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(client_port);

    ret_val = bind(h_socket, 
        (struct  sockaddr*)&remote, sizeof(remote));
    return ret_val;
}

