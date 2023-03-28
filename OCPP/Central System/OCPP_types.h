#ifndef OCPP_TYPES_H
#define OCPP_TYPES_H

#include <time.h>

typedef struct tm dateTime;
#define MESSAGE_TYPE_CALL 2          // Client to server
#define MESSAGE_TYPE_CALLRESULT 3    // Server to client
#define MESSAGE_TYPE_CALLERROR 4     // Server to client

/***********************************************************************/
/**************************** OCPP Enums *******************************/ 
/***********************************************************************/
typedef enum{
    RegistrationStatus_Rejected = 1,
    RegistrationStatus_Accepted = 2,
    RegistrationStatus_Pending = 3
} RegistrationStatus;

typedef enum{
    AuthorizationStatus_Accepted = 1,
    AuthorizationStatus_Blocked = 2,
    AuthorizationStatus_Expired = 3,
    AuthorizationStatus_Invalid = 4,
    AuthorizationStatus_ConcurrentTx = 5
} AuthorizationStatus;


typedef enum {
    Reason_EmergencyStop = 1,
    Reason_EVDisconnected,
    Reason_HardReset,
    Reason_Local,
    Reason_Other,
    Reason_PowerLoss,
    Reason_Reboot,
    Reason_Remote,
    Reason_SoftReset,
    Reason_UnlockCommand,
    Reason_DeAuthorized
} Reason;

typedef enum{
    ValueFormat_Raw = 1,
    ValueFormat_SignedData
} ValueFormat;

typedef enum{
    Measurand_CurrentExport = 1,
    Measurand_CurrentImport
    // ...
} Measurand;

typedef enum{
    ReadingContext_Interruption_Begin = 1,
    ReadingContext_Interruption_End,
    ReadingContext_Other,
    ReadingContext_Sample_Clock,
    ReadingContext_Sample_Periodic,
    ReadingContext_Transaction_Begin ,
    ReadingContext_Transaction_End ,
    ReadingContext_Trigger
} ReadingContext;

typedef enum{
    Phase_L1 = 1,
    Phase_L2,
    Phase_L3,
    Phase_N
    // ...
} Phase;

typedef enum{
    Location_Body = 1,
    Location_Cable,
    Location_EV,
    Location_Inlet,
    Location_Outlet
} Location;

typedef enum{
    UnitOfMeasure_Wh = 1,
    UnitOfMeasure_kWh,
    UnitOfMeasure_varh,
    UnitOfMeasure_kvarh,
    UnitOfMeasure_W,
    UnitOfMeasure_kW
    // ...
} UnitOfMeasure;

/***********************************************************************/
/**************************** OCPP Classes *****************************/ 
/***********************************************************************/
typedef struct{
    char idToken[20];
} IdToken;

typedef struct{
    dateTime expiryDate;
    IdToken parentIdTag;
    AuthorizationStatus status;
} IdTagInfo;


typedef struct{
    IdToken idTag;
} Authorize_req;

typedef struct{
    IdTagInfo idTagInfo;
} Authorize_conf;

typedef struct{
    char chargeBoxSerialNumber[25];
    char chargePointModel[20];
    char chargePointSerialNumber[25];
    char chargePointVendor[20];
    char firmwareVersion[50];
    char iccid[20];
    char imsi[20];
    char meterSerialNumber[25];
    char meterType[25];
} BootNotification_req;

typedef struct{
    dateTime currentTime;
    int interval;
    RegistrationStatus status;
} BootNotification_conf;

typedef struct{
    int connectorId;
    IdToken idTag;
    int meterStart;
    int reservationId;
    dateTime timestamp;
} StartTransaction_req;

typedef struct{
    IdTagInfo idTagInfo;
    int transactionId;
} StartTransaction_conf;

typedef struct{
    char value[50];
    ReadingContext context;
    ValueFormat format;
    Measurand measurand;
    Phase phase;
    Location location;
    UnitOfMeasure unit;
} SampledValue;

typedef struct{
    dateTime timestamp;
    SampledValue sampledValue;
} MeterValue;

typedef struct{
    IdToken idTag;
    int meterStop;
    dateTime timestamp;
    int transactionId;
    Reason reason;
    MeterValue transactionData;
} StopTransaction_req;

typedef struct{
    IdTagInfo idTagInfo;
    int transactionId;
} StopTransaction_conf;

#endif