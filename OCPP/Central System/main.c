#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/OCPP_types.h"
#include "../inc/OCPP.h"
#include "../inc/cJSON.h"
#include "../inc/server_socket.h"

#define MAX_CLIENT_NO 3

void serve_client_msg(cJSON *root, char message[SR_MAX_LEN]){
    cJSON *msgTypeId = cJSON_GetObjectItem(root, "MessageTypeId");
    cJSON *msgId = cJSON_GetObjectItem(root, "UniqueId");
    cJSON *action = cJSON_GetObjectItem(root, "Action");

    time_t tim = time(NULL);

    if(action){
        char action_str[40];

        if(strncmp(action->valuestring, "BootNotification", 50) == 0){
            printf("BootNotification!\n");
            // Handle bootnotification
            BootNotification_req req = parse_boot_notification_req(root);
            BootNotification_conf conf;

            conf.currentTime = *localtime(&tim);
            conf.interval = 60;

            if(strlen(req.chargeBoxSerialNumber) > 0 && strlen(req.chargePointVendor)){
                // Maybe other requirement for acceptance, but here we accept it
                conf.status = RegistrationStatus_Accepted;
            } else{
                conf.status = RegistrationStatus_Rejected;
            }
            OCPP_write_CALLRESULT_message(message,msgId->valuestring,(void*)&conf,"BootNotification");
        } 
        else if(strncmp(action->valuestring, "Authorization", 50) == 0){
            printf("Authorize!\n");
            // Handle Authorization
        }
        else if(strncmp(action->valuestring, "StartTransaction", 50) == 0){
            printf("Start!\n");
            // Handle Start Transaction
        }
        else if(strncmp(action->valuestring, "StopTransaction", 50) == 0){
            printf("Stop!\n");
            // Handle Stop Transaction
        }
        else{
            printf("Action value is invalid.\n");
        }
    }
}

int main(int argc, char* argv[]){
    int socket_desc = 0, sock = 0, client_len = 0;
    struct sockaddr_in client;
    
    // Buffers to send and receive
    char client_msg[SR_MAX_LEN] = {0};
    char message[SR_MAX_LEN] = {0};

    // Create the socket
    socket_desc = socket_create();
    if(socket_desc == -1){
        printf("Could not create the socket.\n");
        return 1;
    } 
    printf("Socket is created.\n");

    // Bind the socket
    if (bind_created_socket(socket_desc) <0){
        perror("Bind failed.");
        return 1;
    }
    printf("Bind is done.\n");

    // Listen
    listen(socket_desc, MAX_CLIENT_NO);

    // Wait for connections
    while(1){
        printf("Waiting for connections.\n");
        client_len = sizeof(struct sockaddr_in);

        sock = accept(socket_desc, 
            (struct sockaddr*)&client, (socklen_t*)&client_len);
        
        if(sock < 0){
            perror("Accept failed");
            return 1;
        }
        printf("Connection accepted.\n");

        // Reset the buffers
        memset(client_msg, '\0', sizeof(client_msg));
        memset(message, '\0', sizeof(message));

        if(recv(sock, client_msg, SR_MAX_LEN, 0) < 0){
            printf("recv failed.\n");
            break;
        }
        printf("Client reply: %s \n", client_msg);

        cJSON *root = cJSON_Parse(client_msg);
        if (root == NULL) {
            printf("Error parsing JSON string: %s\n", cJSON_GetErrorPtr());
            break;
        }

        serve_client_msg(root, message);

        if(send(sock, message, strlen(message), 0) < 0){
            printf("Send failed.\n");
            return 1;
        }

        close(sock);
        sleep(1);
    }

    return 0;
}