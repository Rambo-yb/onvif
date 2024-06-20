#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "cJSON.h"
#include "check_common.h"
#include "cjson_common.h"


typedef struct {
    cJSON* all_config;
    OnvifConfigUsersInfo users_info;
}OnvifConfigMng;
static OnvifConfigMng kOnvifConfigMng;

static int OnvifConfigModifyUsersInfo(OnvifConfigUsersInfo* old_info, OnvifConfigUsersInfo* new_info) {

    return 0;
}

typedef int (*OnvifConfigModifyCb)(void* , void*);
typedef struct {
    const char* type;
    void* config;
    OnvifConfigModifyCb modify_cb;
}OnvifConfigInfo;
static OnvifConfigInfo kOnvifConfigInfo[] = {
    {.type = "users_info", .config = &kOnvifConfigMng.users_info, .modify_cb = OnvifConfigModifyUsersInfo},
};

static cJSON* OnvifConfigLoadFile(char* path) {
    char* file_data = NULL;
    FILE* fp = fopen(path, "r");
    CHECK_POINTER(fp, NULL);

    CHECK_LT_GO(fseek(fp, 0, SEEK_END), 0, end);
    int file_len = ftell(fp);
    CHECK_LT_GO(fseek(fp, 0, SEEK_SET), 0, end);

    file_data = (char*)malloc(file_len+1);
    CHECK_POINTER_GO(file_data, end);

    CHECK_EQ_GO(fwrite(file_data, 1, file_len+1, fp), file_len, end);

    cJSON* json = cJSON_Parse(file_data);
    CHECK_POINTER_GO(json, end);

    free(file_data);
    fclose(fp);
    return json;
end:
    if (file_data != NULL) {
        free(file_data);
    }
    fclose(fp);
    return NULL;
}

static int OnvifConfigUsersInfoInit(cJSON* config) {
    CHECK_POINTER_GO(config, end_default);

    cJSON* users = cJSON_GetObjectItemCaseSensitive(config, "users_info");
    CHECK_POINTER_GO(users, end_default);
    CHECK_BOOL_GO(cJSON_IsArray(users), end_err);

    kOnvifConfigMng.users_info.num = cJSON_GetArraySize(users);
    kOnvifConfigMng.users_info.user_info = (OnvifConfigUserInfo*)malloc(sizeof(OnvifConfigUserInfo)*kOnvifConfigMng.users_info.num);
    CHECK_POINTER_GO(kOnvifConfigMng.users_info.user_info, end_err);
    for (int i = 0; i < kOnvifConfigMng.users_info.num; i++) {
        cJSON* item = cJSON_GetArrayItem(users, i);
        CHECK_POINTER_GO(users, end_err);

        CJSON_GET_STRING(item, "username", kOnvifConfigMng.users_info.user_info[i].username, sizeof(kOnvifConfigMng.users_info.user_info[i].username), end_err);
        CJSON_GET_STRING(item, "password", kOnvifConfigMng.users_info.user_info[i].password, sizeof(kOnvifConfigMng.users_info.user_info[i].password), end_err);
        CJSON_GET_NUMBER(item, "level", kOnvifConfigMng.users_info.user_info[i].level, sizeof(kOnvifConfigMng.users_info.user_info[i].level), end_err);
    }

    return 0;
end_default:
    LOG_INFO("not find users info, use memory users info!");
    kOnvifConfigMng.users_info.num = 1;
    kOnvifConfigMng.users_info.user_info = (OnvifConfigUserInfo*)malloc(sizeof(OnvifConfigUserInfo));
    kOnvifConfigMng.users_info.user_info[0].level = 0;
    snprintf(kOnvifConfigMng.users_info.user_info[0].username, sizeof(kOnvifConfigMng.users_info.user_info[0].username), "admin");
    snprintf(kOnvifConfigMng.users_info.user_info[0].password, sizeof(kOnvifConfigMng.users_info.user_info[0].password), "123456");
    return 0;
end_err:
    if (kOnvifConfigMng.users_info.user_info != NULL) {
        free(kOnvifConfigMng.users_info.user_info);
    }
    return -1;
}

int OnvifConfigInit() {
    kOnvifConfigMng.all_config = OnvifConfigLoadFile("/data/onvif.json");

    OnvifConfigUsersInfoInit(kOnvifConfigMng.all_config);
    return 0;
}

void* OnvifConfigGet(const char* type) {
    CHECK_POINTER(type, NULL);
    for(int i = 0; i < sizeof(kOnvifConfigInfo)/sizeof(OnvifConfigInfo); i++) {
        if (strcmp(kOnvifConfigInfo[i].type, type) == 0) {
            return kOnvifConfigInfo[i].config;
        }
    }

    LOG_ERR("%s not support!", type);
    return NULL;
}

int OnvifConfigSet(const char* type, void* conf) {
    CHECK_POINTER(type, -1);
    CHECK_POINTER(conf, -1);
    for(int i = 0; i < sizeof(kOnvifConfigInfo)/sizeof(OnvifConfigInfo); i++) {
        if (strcmp(kOnvifConfigInfo[i].type, type) == 0) {
            return kOnvifConfigInfo[i].modify_cb(kOnvifConfigInfo[i].config, conf);
        }
    }

    LOG_ERR("%s not support!", type);
    return -1;

}