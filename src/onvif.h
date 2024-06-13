#ifndef __ONVIF_H__
#define __ONVIF_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OnvifDevInfo {
    char menu_facturer[64];
    char module[64];
    char serial_num[64];
    char firmware_ver[64];
    char hardware_ver[64];
}OnvifDevInfo;

int OnvifInit(char* addr, int port, OnvifDevInfo dev_info);

int OnvifUnInit();

int OnvifSetDeviceInfo(OnvifDevInfo info);

#ifdef __cplusplus
}
#endif

#endif