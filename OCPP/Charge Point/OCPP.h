#ifndef OCPP_H
#define OCPP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// For message unique id
#include <uuid/uuid.h>

#include "cJSON.h"
#include "OCPP_types.h"

// Send-Receive max length over TCP
#define SR_MAX_LEN 600

void OCPP_write_CALL_message(char msg_str[SR_MAX_LEN],  void* p_structure, const char* action_name);
void OCPP_write_CALLRESULT_message(char msg_str[SR_MAX_LEN], char* msgId, void* p_structure, const char* action_name);

BootNotification_req parse_boot_notification_req(cJSON* root);
BootNotification_conf parse_boot_notification_conf(cJSON* root);

#endif
