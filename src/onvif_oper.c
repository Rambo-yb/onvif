#include <string.h>
#include "onvif_oper.h"

typedef struct {
    OnvifOperDeviceInfo dev_info;
}OnvifOperMng;
static OnvifOperMng kOnvifOperMng;

void OnvifOperGetDevInfo(OnvifOperDeviceInfo* dev_info){
    memcpy(dev_info, &kOnvifOperMng.dev_info, sizeof(OnvifOperDeviceInfo));
}

void OnvifOperSetDevInfo(OnvifOperDeviceInfo* dev_info){
    memcpy(&kOnvifOperMng.dev_info, dev_info, sizeof(OnvifOperDeviceInfo));
}