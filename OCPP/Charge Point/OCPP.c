#include "../inc/OCPP.h"

/*****************************************************************************/
/************************* Convert enums to strings **************************/
/*****************************************************************************/
const char * const Reason_STR[] = {
	"", "EmergencyStop","EVDisconnected","HardReset",
    "Local","Other","PowerLoss","Reboot","Remote",
    "SoftReset","UnlockCommand", "DeAuthorized"
};
const char * const ReadingContext_STR[] = {
    "","Interruption_Begin","Interruption_End",
    "Other","Sample_Clock","Sample_Periodic",
    "Transaction_Begin","Transaction_End",
    "Trigger"
};
const char * const RegistrationStatus_STR[] = {
    "","Rejected","Accepted","Pending"
};
const char * const AuthorizationStatus_STR[] = {
    "","Accepted","Blocked","Expired",
    "Invalid","ConcurrentTx"
};
const char * const ValueFormat_STR[] = {
    "", "Raw", "SignedData"
};
const char * const Measurand_STR[] = {
    "", "CurrentExport", "CurrentImport"
};
const char * const Phase_STR[] = {
    "", "L1", "L2", "L3", "N"
};
const char * const UnitOfMeasure_STR[] = {
    "","Wh","kWh","varh","kvarh","W","kW"
};
const char * const Location_STR[] = {
    "","Body","Cable","EV","Inlet","Outlet"
};

/*****************************************************************************/
/******** Some Preprocessor to write the fields of OCPP structs **************/
/*****************************************************************************/
#define WRITE_OPTIONAL_STR(root, field, field_name)\
    if(strlen(field) != 0)\
        cJSON_AddStringToObject(root,#field_name,field);

#define WRITE_REQUIRED_STR(root, field, field_name)\
    if(strlen(field) != 0) {\
        cJSON_AddStringToObject(root,#field_name,field); }\
    else {\
        perror(#field_name " is Required."); }

#define WRITE_OPTINAL_ENUM(root, value, enum_name , field_name) \
    if(value != 0){\
        cJSON_AddStringToObject(root,#field_name, enum_name##_STR[value]); }

#define WRITE_REQUIRED_ENUM(root, value, enum_name ,field_name) \
    if(value != 0){\
        cJSON_AddStringToObject(root,#field_name, enum_name##_STR[value]); } \
    else {\
        perror(#enum_name " is required."); }

#define GET_OPTIONAL_STR(root, field, field_name)\
    cJSON *val = cJSON_GetObjectItem(root, #field_name);\
    if(strlen(val->valueString) != 0){\
        strncpy(field, val->valuestring, sizeof(field));}

#define GET_REQUIRED_STR(root, field, field_name)\
    cJSON *val = cJSON_GetObjectItem(root, #field_name);\
    if(strlen(val->valueString) != 0){\
        strncpy(field, val->valuestring, sizeof(field));}\

#define GET_ENUM_FROM_STR(ret_val, value, enum_name, num_enum)\
    for(int i = 1; i <= num_enum; ++i){\
        if (strcmp(value, enum_name##_STR[i]) == 0) { ret_val = (enum_name)i;}}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

void get_uuid_str(char* msgId){
    uuid_t binuuid;
    uuid_generate_random(binuuid);
    uuid_unparse(binuuid, msgId);
}

void get_timestamp_str(char buf[20], dateTime* dt){
    sprintf(buf, "%d-%d-%d-%d-%d-%d", dt->tm_mday,dt->tm_mon,1900+dt->tm_year,dt->tm_hour,dt->tm_min,dt->tm_sec);
}

void serialize_boot_notification_req(
        cJSON* root,
        char msg_str[SR_MAX_LEN], 
        BootNotification_req *bnreq){
    
    // BootNotification serialization
    cJSON *bnot_json = cJSON_CreateObject();
    WRITE_OPTIONAL_STR(bnot_json, bnreq->chargeBoxSerialNumber, chargeBoxSerialNumber);
    WRITE_REQUIRED_STR(bnot_json, bnreq->chargePointModel, chargePointModel);
    WRITE_OPTIONAL_STR(bnot_json, bnreq->chargePointSerialNumber, chargePointSerialNumber);
    WRITE_REQUIRED_STR(bnot_json, bnreq->chargePointVendor,chargePointVendor );
    WRITE_OPTIONAL_STR(bnot_json, bnreq->firmwareVersion, firmwareVersion);
    WRITE_OPTIONAL_STR(bnot_json, bnreq->iccid,iccid);
    WRITE_OPTIONAL_STR(bnot_json, bnreq->imsi,imsi);
    WRITE_OPTIONAL_STR(bnot_json, bnreq->meterSerialNumber,meterSerialNumber);
    WRITE_OPTIONAL_STR(bnot_json, bnreq->meterType, meterType);

    cJSON_AddItemToObject(root, "chargePoint", bnot_json);
}

void serialize_authorization_req(
        cJSON* root,
        char msg_str[SR_MAX_LEN], 
        Authorize_req *auth_req
){
    // Authorization Req serialization
    cJSON *auth_req_json = cJSON_CreateObject();
    WRITE_REQUIRED_STR(auth_req_json, auth_req->idTag.idToken, idToken);
    cJSON_AddItemToObject(root, "chargePoint", auth_req_json);
}

void serialize_start_transaction_req(
        cJSON* root,
        char msg_str[SR_MAX_LEN], 
        StartTransaction_req *start_trans_req
){
    // StartTransaction_req serialization
    cJSON *start_tra_req_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(start_tra_req_json,"connectorId",start_trans_req->connectorId);
    WRITE_REQUIRED_STR(start_tra_req_json,start_trans_req->idTag.idToken, idTag);
    cJSON_AddNumberToObject(start_tra_req_json,"meterStart",start_trans_req->meterStart);
    cJSON_AddNumberToObject(start_tra_req_json,"reservationId",start_trans_req->reservationId);
    
    char datetime_str[20];
    get_timestamp_str(datetime_str, &(start_trans_req->timestamp));
    WRITE_REQUIRED_STR(start_tra_req_json,datetime_str, timestamp);

    cJSON_AddItemToObject(root, "chargePoint", start_tra_req_json);
}

void serialize_stop_transaction_req(
        cJSON* root,
        char msg_str[SR_MAX_LEN], 
        StopTransaction_req *stop_trans_req
){
    // StartTransaction_req serialization
    cJSON *stop_tra_req_json = cJSON_CreateObject();
    WRITE_REQUIRED_STR(stop_tra_req_json,stop_trans_req->idTag.idToken, idTag);
    cJSON_AddNumberToObject(stop_tra_req_json,"meterStop",stop_trans_req->meterStop);

    char datetime_str[20];
    get_timestamp_str(datetime_str, &(stop_trans_req->timestamp));

    cJSON_AddStringToObject(stop_tra_req_json,"timestamp",datetime_str);
    cJSON_AddNumberToObject(stop_tra_req_json,"transactionId",stop_trans_req->transactionId);
    WRITE_OPTINAL_ENUM(stop_tra_req_json, stop_trans_req->reason, Reason, reason);

    cJSON *transaction_data_json = cJSON_CreateObject();
    get_timestamp_str(datetime_str, &(stop_trans_req->transactionData.timestamp));
    WRITE_REQUIRED_STR(transaction_data_json, datetime_str, timestamp);

    cJSON *sampled_value_json = cJSON_CreateObject();
    cJSON_AddStringToObject(sampled_value_json, "value", stop_trans_req->transactionData.sampledValue.value);
    WRITE_OPTINAL_ENUM(sampled_value_json, stop_trans_req->transactionData.sampledValue.context,ReadingContext,context);
    WRITE_OPTINAL_ENUM(sampled_value_json, stop_trans_req->transactionData.sampledValue.format,ValueFormat,format);
    WRITE_OPTINAL_ENUM(sampled_value_json, stop_trans_req->transactionData.sampledValue.location,Location,location);
    WRITE_OPTINAL_ENUM(sampled_value_json, stop_trans_req->transactionData.sampledValue.measurand,Measurand,measurand);
    WRITE_OPTINAL_ENUM(sampled_value_json, stop_trans_req->transactionData.sampledValue.phase,Phase,phase);
    WRITE_OPTINAL_ENUM(sampled_value_json, stop_trans_req->transactionData.sampledValue.unit,UnitOfMeasure,unit);

    cJSON_AddItemToObject(transaction_data_json, "sampledValue", sampled_value_json);
    cJSON_AddItemToObject(stop_tra_req_json, "transactionData", transaction_data_json);
    cJSON_AddItemToObject(root, "chargePoint", stop_tra_req_json);
}

void serialize_boot_notification_conf(
        cJSON* root,
        char msg_str[SR_MAX_LEN], 
        BootNotification_conf *bnconf)
{
    // BootNotificationConf serialization
    cJSON *bnconf_json = cJSON_CreateObject();
    WRITE_REQUIRED_ENUM(bnconf_json, bnconf->status, RegistrationStatus, "status");
    cJSON_AddNumberToObject(bnconf_json, "interval", bnconf->interval);
    char datetime_str[20];
    get_timestamp_str(datetime_str, &(bnconf->currentTime));
    WRITE_REQUIRED_STR(bnconf_json,datetime_str, "currentTime");

    cJSON_AddItemToObject(root, "centralSystem", bnconf_json);
}

void serialize_authorization_conf(
        cJSON* root,
        char msg_str[SR_MAX_LEN], 
        Authorize_conf *auth_conf
){
    cJSON *auth_conf_json = cJSON_CreateObject();

    char date_time[20];
    get_timestamp_str(date_time, &auth_conf->idTagInfo.expiryDate);
    WRITE_OPTIONAL_STR(auth_conf_json, date_time, expiryDate);
    WRITE_OPTIONAL_STR(auth_conf_json, auth_conf->idTagInfo.parentIdTag.idToken, parentIdTag);
    WRITE_REQUIRED_ENUM(auth_conf_json, auth_conf->idTagInfo.status, AuthorizationStatus, status);
    cJSON_AddItemToObject(root, "centralSystem", auth_conf_json);
}

void serialize_start_transaction_conf(
        cJSON* root,
        char msg_str[SR_MAX_LEN], 
        StartTransaction_conf *start_tran_conf
){

}

void serialize_stop_transaction_conf(
        cJSON* root,
        char msg_str[SR_MAX_LEN], 
        StopTransaction_conf *stop_tran_conf
){

}

BootNotification_req parse_boot_notification_req(cJSON* root){
    BootNotification_req req;

    if(root == NULL) return req;

    cJSON *cp = cJSON_GetObjectItem(root, "chargePoint");
    if(cp == NULL) return req;

    strcpy(req.chargeBoxSerialNumber, cJSON_GetObjectItem(cp, "chargePointModel")->valuestring);
    strcpy(req.chargePointVendor, cJSON_GetObjectItem(cp, "chargePointVendor")->valuestring);

    return req;
}

/*
* Parses the BootNotification_conf structure and returns it
*/
BootNotification_conf parse_boot_notification_conf(cJSON* root){
    BootNotification_conf conf;

    if(root == NULL) return conf;

    cJSON *cs = cJSON_GetObjectItem(root, "centralSystem");
    if(cs == NULL) return conf;

    conf.interval = cJSON_GetObjectItem(cs, "interval")->valueint;
    GET_ENUM_FROM_STR(conf.status, cJSON_GetObjectItem(cs, "status")->valuestring, RegistrationStatus, 3);
    strptime(cJSON_GetObjectItem(cs, "currentTime")->valuestring, "%d-%m-%Y-%H-%M-%S", &conf.currentTime);
    
    return conf;
}

void OCPP_write_CALL_message(
        char msg_str[SR_MAX_LEN], 
        void* p_structure, const char* action_name){

    // Create JSON object
    cJSON *root = cJSON_CreateObject();

    // Message type
    int msgTypeId = MESSAGE_TYPE_CALL;
    // Get unique message ID
    char *msgId = malloc(37);
    get_uuid_str(msgId);
    // CALL Message format
    cJSON_AddNumberToObject(root, "MessageTypeId", msgTypeId);
    cJSON_AddStringToObject(root, "UniqueId", msgId);
    // Write action name
    cJSON_AddStringToObject(root, "Action", action_name);

    if(strncmp(action_name, "BootNotification", 50) == 0){
        serialize_boot_notification_req(root, msg_str, (BootNotification_req*)p_structure);
    } 
    else if(strncmp(action_name, "Authorization", 50) == 0){
        serialize_authorization_req(root, msg_str, (Authorize_req*)p_structure);
    }
    else if(strncmp(action_name, "StartTransaction", 50) == 0){
        serialize_start_transaction_req(root, msg_str, (StartTransaction_req*)p_structure);
    }
    else if(strncmp(action_name, "StopTransaction", 50) == 0){
        serialize_stop_transaction_req(root, msg_str, (StopTransaction_req*)p_structure);
    }
    else{
        printf("Wrong CALL action type. Available actions: \
        {BootNotification, Authorization, StartTransaction, StopTransaction,\
        Authorization, StartTransaction, StopTransaction}\n");
    }

    strncpy(msg_str, cJSON_Print(root), SR_MAX_LEN);
    cJSON_Delete(root);
}

void OCPP_write_CALLRESULT_message(
    char msg_str[SR_MAX_LEN], char* msgId,
    void* p_structure, const char* action_name)
{
    // Create JSON object
    cJSON *root = cJSON_CreateObject();

    // Message type
    int msgTypeId = MESSAGE_TYPE_CALLRESULT;
    // CALLRESULT Message format
    cJSON_AddNumberToObject(root, "MessageTypeId", msgTypeId);
    cJSON_AddStringToObject(root, "UniqueId", msgId);
    
    if(strncmp(action_name, "BootNotification", 50) == 0){
        serialize_boot_notification_conf(root, msg_str, (BootNotification_conf*)p_structure);
    } 
    else if(strncmp(action_name, "Authorization", 50) == 0){
        serialize_authorization_conf(root, msg_str, (Authorize_conf*)p_structure);
    }
    else if(strncmp(action_name, "StartTransaction", 50) == 0){
        serialize_start_transaction_conf(root, msg_str, (StartTransaction_conf*)p_structure);
    }
    else if(strncmp(action_name, "StopTransaction", 50) == 0){
        serialize_stop_transaction_conf(root, msg_str, (StopTransaction_conf*)p_structure);
    }
    else{
        printf("Wrong CARLLRESULT action type. Available actions: \
        {BootNotification, Authorization, StartTransactionConf, StopTransaction,\
        Authorization, StartTransaction, StopTransaction}\n");
    }

    strncpy(msg_str, cJSON_Print(root), SR_MAX_LEN);
    cJSON_Delete(root);
}