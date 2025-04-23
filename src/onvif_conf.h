#ifndef __ONVIF_CONF_H__
#define __ONVIF_CONF_H__

int OnvifConfInit(const char* conf_path);

int OnvifConfUninit();

void* OnvifConfGetConfig(const char* key);

int OnvifConfSetConfig(const char* key, void* param);

#endif