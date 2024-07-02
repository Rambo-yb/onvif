#ifndef __ONVIF_OPER_H__
#define __ONVIF_OPER_H__

typedef struct {
    int web_server_port;
    char web_server_addr[16];

    char menu_facturer[64];
    char module[64];
    char serial_num[64];
    char firmware_ver[64];
    char hardware_ver[64];
}OnvifOperDeviceInfo;

void OnvifOperGetDevInfo(OnvifOperDeviceInfo* dev_info);

void OnvifOperSetDevInfo(OnvifOperDeviceInfo* dev_info);

#endif