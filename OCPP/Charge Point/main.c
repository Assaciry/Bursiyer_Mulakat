#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../inc/client_socket.h"
#include "../inc/OCPP.h"

// gcc cJSON.c client_socket.c OCPP.c main.c -luuid -o ../main.exe

void station_boot_notification(){
    int read_size = 0;
    char send_to_server[SR_MAX_LEN] = {0};
    char server_reply[SR_MAX_LEN] = {0};

    int h_socket = start_connections();

    BootNotification_req bnreq = {.chargePointVendor = "VendorITU", .chargePointModel="V-MOD2"};
    OCPP_write_CALL_message(send_to_server, (void*)&bnreq, "BootNotification");

    // Send the data
    socket_send(h_socket, send_to_server, strlen(send_to_server));
    // Receive the response
    read_size = socket_receive(h_socket, server_reply, SR_MAX_LEN);

    close(h_socket);
}

RegistrationStatus station_authorization(Authorize_req *p_auth_req){
    int read_size = 0;
    char send_to_server[SR_MAX_LEN] = {0};
    char server_reply[SR_MAX_LEN] = {0};
    int h_socket = start_connections();

    OCPP_write_CALL_message(send_to_server, (void*)p_auth_req, "Authorization");

    // Send the data
    socket_send(h_socket, send_to_server, strlen(send_to_server));
    // Receive the response
    read_size = socket_receive(h_socket, server_reply, SR_MAX_LEN);

    cJSON *root = cJSON_Parse(server_reply);
    BootNotification_conf bnconf =  parse_boot_notification_conf(root);
    cJSON_Delete(root);

    // Close
    close(h_socket);

    return bnconf.status;
}

void station_start_transaction(StartTransaction_req *p_start_trans){
    int read_size = 0;
    char send_to_server[SR_MAX_LEN] = {0};
    char server_reply[SR_MAX_LEN] = {0};
    int h_socket = start_connections();

    OCPP_write_CALL_message(send_to_server, (void*)p_start_trans, "StartTransaction");

    // Send the data
    socket_send(h_socket, send_to_server, strlen(send_to_server));
    // Receive the response
    read_size = socket_receive(h_socket, server_reply, SR_MAX_LEN);
    // Close
    close(h_socket);
}

void station_stop_transaction(StopTransaction_req *p_stop_trans){
    int read_size = 0;
    char send_to_server[SR_MAX_LEN] = {0};
    char server_reply[SR_MAX_LEN] = {0};
    int h_socket = start_connections();

    OCPP_write_CALL_message(send_to_server, (void*)p_stop_trans, "StopTransaction");

    // Send the data
    socket_send(h_socket, send_to_server, strlen(send_to_server));
    // Receive the response
    read_size = socket_receive(h_socket, server_reply, SR_MAX_LEN);
    // Close
    close(h_socket);
}

int main(int argc, char* argv[]){
    bool new_connection = true; // normally this would be set via interrupt etc.
    time_t tim = time(NULL);

    // BootNotification at start
    station_boot_notification();

    // Infinite loop of Charge Point
    while(1){
        printf("Waiting for new charging connections...\n");

        // Check for new car connection to charge point
        if(new_connection){
            // Check if the connection is valid (accepted as valid)
            // ...
            char* connector_idtag = "ITU1A2B3";
            
            printf("New charging connection, sending auhorization to Central System.\n");
            Authorize_req auth_req;
            strncpy(auth_req.idTag.idToken, connector_idtag, 10);
            RegistrationStatus status = station_authorization(&auth_req);
            
            printf("Checking authorization...\n");
            /* If authorization unsuccessful*/
            if(status != RegistrationStatus_Accepted){
                printf("Authorization failed!\n");
                break;
            }

            // Start transaction
            int connector_id = 101;
            int starWh = 2;

            StartTransaction_req start_req;
            start_req.connectorId = connector_id;
            strncpy(start_req.idTag.idToken, connector_idtag, 10);
            start_req.meterStart = starWh;
            start_req.reservationId = 42;
            start_req.timestamp = *localtime(&tim);
            station_start_transaction(&start_req);

            // Wait for EV to charge (normally this would be async)
            printf("Charging...\n");
            sleep(3);

            // Stop transaction
            int stopWh = 100;

            StopTransaction_req stop_req;
            strncpy(stop_req.idTag.idToken, connector_idtag, 10);
            stop_req.meterStop = stopWh;
            stop_req.timestamp = *localtime(&tim);
            stop_req.reason = Reason_EVDisconnected;

            stop_req.transactionData.timestamp = *localtime(&tim);
            SampledValue sval = {.value = "4325.3353"};
            sval.context = ReadingContext_Interruption_End;
            stop_req.transactionData.sampledValue = sval;
            station_stop_transaction(&stop_req);

            // Connection is dealt with, therefore, flip the bool
            new_connection = false;
        }

        sleep(10);
    }

    return 0;
}