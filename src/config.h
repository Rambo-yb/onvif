#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int web_server_port;
    char web_server_addr[16];

    char menu_facturer[64];
    char module[64];
    char serial_num[64];
    char firmware_ver[64];
    char hardware_ver[64];
}OnvifConfigDeviceInfo;

int OnvifConfigInit();

void* OnvifGetConfig(const char* type);

void OnvifConfigGetDevInfo(OnvifConfigDeviceInfo* dev_info);

void OnvifConfigSetDevInfo(OnvifConfigDeviceInfo* dev_info);

#ifdef __cplusplus
}
#endif

#endif