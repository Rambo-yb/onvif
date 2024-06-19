#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char username[32];
    char password[64];
    int level;
}OnvifConfigUserInfo;

typedef struct {
    int num;
    OnvifConfigUserInfo* user_info;
}OnvifConfigUsersInfo;

int OnvifConfigInit();

void* OnvifConfigGet(const char* type);

int OnvifConfigSet(const char* type, void* conf);

#ifdef __cplusplus
}
#endif

#endif