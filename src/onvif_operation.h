#ifndef __ONVIF_OPERATION_H__
#define __ONVIF_OPERATION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "onvif_common.h"

typedef struct {
    char device_addr[16];
    int web_server_port;
    int event_message_port;

    OnvifDevInfo dev_info;
}OnvifOperationDeviceInfo;

int OnvifOperationInit(OnvifOperationDeviceInfo device_info);

void OnvifOperationUnInit();

void OnvifOperationRegister(OnvifOperationType type, void* cb);

void OnvifOperationGetDevInfo(OnvifOperationDeviceInfo* device_info);

void* OnvifOperationGetConfig(const char* type);

void OnvifOperationSetConfig(const char* type, void* arg);

void OnvifOperationPtzCtrl(OnvifPtzCtrlType type, char* arg);

void OnvifOperationEventUpload(OnvifEventInfo* info);

int OnvifOperationGetEventInfo(void** json, long long timeout);


#ifdef __cplusplus
}
#endif

#endif