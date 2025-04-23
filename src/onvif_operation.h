#ifndef __ONVIF_OPERATION_H__
#define __ONVIF_OPERATION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "onvif_common.h"

void OnvifOperationRegisterCb(OnvifOperationType type, void* cb);

int OnvifOperationSystemRequest(OnvifSystem* sys);

int OnvifOperationGetConfig(int type, void* st, int size);

int OnvifOperationSetConfig(int type, void* st, int size);

int OnvifOperationContorlRequest(int type, int chn, void* st, int size);

#ifdef __cplusplus
}
#endif

#endif