#ifndef __ONVIF_H__
#define __ONVIF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "onvif_common.h"

int OnvifInit(char* addr, OnvifDevInfo dev_info);

int OnvifUnInit();

void OnvifRegister(OnvifOperationType type, void* cb);

void OnvifEventUplaod(OnvifEventInfo* info);

#ifdef __cplusplus
}
#endif

#endif