#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "cJSON.h"
#include "check_common.h"
#include "cjson_common.h"


typedef struct {
    cJSON* all_config;
    OnvifConfigDeviceInfo dev_info;
}OnvifConfigMng;
static OnvifConfigMng kOnvifConfigMng;

static cJSON* OnvifConfigLoadFile(char* path) {
    char* file_data = NULL;
    FILE* fp = fopen(path, "r");
    CHECK_POINTER(fp, return NULL);

    CHECK_LT(fseek(fp, 0, SEEK_END), 0, goto end);
    int file_len = ftell(fp);
    CHECK_LT(fseek(fp, 0, SEEK_SET), 0, goto end);

    file_data = (char*)malloc(file_len+1);
    CHECK_POINTER(file_data, goto end);

    CHECK_EQ(fread(file_data, 1, file_len+1, fp), file_len, goto end);

    cJSON* json = cJSON_Parse(file_data);
    CHECK_POINTER(json, goto end);

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

int OnvifConfigInit() {
    kOnvifConfigMng.all_config = OnvifConfigLoadFile("/data/onvif.json");
    return 0;
}

void* OnvifGetConfig(const char* type) {
    return cJSON_GetObjectItemCaseSensitive(kOnvifConfigMng.all_config, type);
}

void OnvifConfigGetDevInfo(OnvifConfigDeviceInfo* dev_info){
    memcpy(dev_info, &kOnvifConfigMng.dev_info, sizeof(OnvifConfigDeviceInfo));
}

void OnvifConfigSetDevInfo(OnvifConfigDeviceInfo* dev_info){
    memcpy(&kOnvifConfigMng.dev_info, dev_info, sizeof(OnvifConfigDeviceInfo));
}