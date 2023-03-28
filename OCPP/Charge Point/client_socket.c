#include "../inc/client_socket.h"


short socket_create(void){
    short h_socket;
    h_socket = socket(AF_INET, SOCK_STREAM, 0);
    return h_socket;
}

int socket_connect(int h_socket){
    int ret_val = -1;
    int server_port = 12345;

    struct sockaddr_in remote = {0};

    // Internet address family
    remote.sin_family = AF_INET;

    // Any incoming interface
    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); // Local host
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(server_port);

    ret_val = connect(h_socket, 
        (struct  sockaddr*)&remote, sizeof(remote));
    return ret_val;
}

int socket_send(int h_socket, char* rqst, short len_rqst){
    int short_retval = -1;
    struct timeval tv;
    tv.tv_sec = 20; // 20 secs timeout
    tv.tv_usec = 0;

    if(setsockopt(h_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv)) < 0){
        printf("Timeout.\n");
        return -1;
    }

    short_retval = send(h_socket, rqst, len_rqst, 0);
    return short_retval;
}

int socket_receive(int h_socket, char* response, short len_response){
    int short_retval = -1;
    struct timeval tv;
    tv.tv_sec = 20; // 20 secs timeout
    tv.tv_usec = 0;

    if(setsockopt(h_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0){
        printf("Timeout.\n");
        return -1;
    }

    short_retval = recv(h_socket, response, len_response, 0);
    printf("Response: %s \n", response);
    return short_retval;
}

int start_connections(){
    int h_socket = 0;

    h_socket = socket_create();
    if(h_socket == -1){
        printf("Could not create the socket.\n");
        return 1;
    }

    // Connect to the Central System server
    if (socket_connect(h_socket) < 0){
        perror("Connection to Central System has failed.");
        return 1;
    }
    //printf("Successfully connected to the Central System.\n");

    return h_socket;
}