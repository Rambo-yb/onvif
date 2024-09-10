#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "onvif_operation.h"
#include "cJSON.h"
#include "check_common.h"
#include "cjson_common.h"
#include "struct_to_cjson.h"
#include "cjson_to_struct.h"
#include "list.h"

#define ONVIF_CONFIG_FILE "./onvif.json"

typedef struct {
    OnvifOperGetConfigCb get_config_cb;
    OnvifOperSetConfigCb set_config_cb;
    OnvifOperPtzCtrlCb ptz_ctrl_cb;
}OnvifOperCb;

typedef struct {
    cJSON* all_config;
    OnvifOperationDeviceInfo device_info;
    OnvifOperCb oper_cb;
    void* event_list;
    pthread_mutex_t list_mutex;
}OnvifOperationMng;
static OnvifOperationMng kOnvifOperationMng = {.list_mutex = PTHREAD_MUTEX_INITIALIZER};

static cJSON* OnvifOperationLoadFile(char* path) {
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

static cJSON* OnvifOperationDefaultConfigScopes() {
    cJSON* scopes_json = cJSON_CreateArray();
    CHECK_POINTER(scopes_json, return NULL);

    cJSON* scope_json = cJSON_CreateObject();
    CHECK_POINTER(scope_json, goto end);

    CJSON_SET_STRING(scope_json, "item", "onvif://www.onvif.org/Profile/Streaming", goto end);
    CJSON_SET_NUMBER(scope_json, "def", 1, goto end);
    CHECK_BOOL(cJSON_AddItemToArray(scopes_json, scope_json), goto end);

    return scopes_json;
end:
    if (scope_json) {
        cJSON_free(scope_json);
    }
    if (scopes_json) {
        cJSON_free(scopes_json);
    }
    return NULL;
}

static cJSON* OnvifOperationDefaultConfigUsers() {
    cJSON* users_json = cJSON_CreateArray();
    CHECK_POINTER(users_json, return NULL);

    cJSON* user_json = cJSON_CreateObject();
    CHECK_POINTER(users_json, goto end);

    CJSON_SET_STRING(user_json, "username", "admin", goto end);
    CJSON_SET_STRING(user_json, "password", "123456", goto end);
    CJSON_SET_NUMBER(user_json, "level", 0, goto end);
    CHECK_BOOL(cJSON_AddItemToArray(users_json, user_json), goto end);

    return users_json;
end:
    if (user_json) {
        cJSON_free(user_json);
    }
    if (users_json) {
        cJSON_free(users_json);
    }
    return NULL;
}

static cJSON* OnvifOperationDefaultConfigProfile() {
    cJSON* new_json = NULL;
    cJSON* new_json_ = NULL;
    cJSON* new_arr_json = NULL;
    cJSON* profile_json = NULL;
    cJSON* profiles_json = cJSON_CreateArray();
    CHECK_POINTER(profiles_json, return NULL);

    profile_json = cJSON_CreateObject();
    CHECK_POINTER(profile_json, goto end);

    CJSON_SET_STRING(profile_json, "rtsp_url", "rtsp://192.168.110.223:554/live/main_stream", goto end);
    CJSON_SET_STRING(profile_json, "snap_url", "", goto end);

    CJSON_SET_STRING(profile_json, "token", "media_token", goto end);
    CJSON_SET_STRING(profile_json, "name", "media_name", goto end);
    CJSON_SET_NUMBER(profile_json, "fixed", 1, goto end);

    OnvifVideoEncoder video_enc = {.encoding = ONVIF_VIDEO_ENCODING_H264, .resolution_width = 640, .resolution_height = 512, .gov_length = 25, .profile = ONVIF_H264_MAIN};
    CHECK_LT(StructToCjsonVideoEncoder(&video_enc, &new_json, &new_json_), 0, goto end);

    CJSON_SET_NUMBER(profile_json, "use_video_encoder_index", 0, goto end);
    CHECK_BOOL(cJSON_AddItemToObject(profile_json, "video_source", new_json_), goto end);
    new_arr_json = cJSON_CreateArray();
    CHECK_POINTER(new_arr_json, goto end);
    CHECK_BOOL(cJSON_AddItemToArray(new_arr_json, new_json), goto end);
    CHECK_BOOL(cJSON_AddItemToObject(profile_json, "compatible_video_encoder", new_arr_json), goto end);
    new_arr_json = NULL;
    new_json_ = NULL;
    new_json = NULL;

    OnvifAudioEncoder audio_enc = {.channels = 1, .encoding = ONVIF_AUDIO_ENCODING_G711, .sample_rate = 16000, .bitrate = 1024};
    CHECK_LT(StructToCjsonAudioEncoder(&audio_enc, &new_json, &new_json_), 0, goto end);

    CJSON_SET_NUMBER(profile_json, "use_audio_encoder_index", 0, goto end);
    CHECK_BOOL(cJSON_AddItemToObject(profile_json, "audio_source", new_json_), goto end);
    new_arr_json = cJSON_CreateArray();
    CHECK_POINTER(new_arr_json, goto end);
    CHECK_BOOL(cJSON_AddItemToArray(new_arr_json, new_json), goto end);
    CHECK_BOOL(cJSON_AddItemToObject(profile_json, "compatible_audio_encoder", new_arr_json), goto end);
    new_arr_json = NULL;
    new_json_ = NULL;
    new_json = NULL;

    // ptz
    new_arr_json = cJSON_CreateArray();
    CHECK_POINTER(new_arr_json, goto end);
    for(int j = 0; j < 1; j++) {
        new_json = cJSON_CreateObject();
        CHECK_POINTER(new_json, goto end);

        CJSON_SET_STRING(new_json, "token", "ptz_token", goto end);
        CJSON_SET_STRING(new_json, "name", "ptz_name", goto end);
        CJSON_SET_STRING(new_json, "node_token", "ptz_node", goto end);

        CJSON_SET_NUMBER(new_json, "use_count", 1, goto end);          
        CJSON_SET_NUMBER(new_json, "ptz_timeout", 10*1000, goto end);

        CHECK_BOOL(cJSON_AddItemToArray(new_arr_json, new_json), goto end);
        new_json = NULL;
    }
    CJSON_SET_NUMBER(profile_json, "use_ptz_index", 0, goto end);
    CHECK_BOOL(cJSON_AddItemToObject(profile_json, "compatible_ptz", new_arr_json), goto end);
    new_arr_json = NULL;

    CHECK_BOOL(cJSON_AddItemToArray(profiles_json, profile_json), goto end);

    return profiles_json;
end:
    if (new_json_ != NULL) {
        cJSON_free(new_json_);
    }
    if (new_json != NULL) {
        cJSON_free(new_json);
    }
    if (new_arr_json != NULL) {
        cJSON_free(new_arr_json);
    }
    if (profile_json != NULL) {
        cJSON_free(profile_json);
    }
    if (profiles_json != NULL) {
        cJSON_free(profiles_json);
    }
    return NULL;
}

static cJSON* OnvifOperationDefaultConfigNetwork() {
    cJSON* new_json = NULL;
    cJSON* new_arr_json = NULL;
    cJSON* network_json = cJSON_CreateObject();
    CHECK_POINTER(network_json, goto end);

    new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, goto end);

    CJSON_SET_NUMBER(new_json, "ipv4_enable", 1, goto end);
    CJSON_SET_NUMBER(new_json, "ipv4_dhcp_enable", 0, goto end);
    CJSON_SET_STRING(new_json, "ipv4_addr", kOnvifOperationMng.device_info.device_addr, goto end);
    CJSON_SET_STRING(new_json, "ipv4_mask", "255.255.255.0", goto end);
    int num[4] = {0};
    sscanf(kOnvifOperationMng.device_info.device_addr, "%d.%d.%d.%d", &num[0], &num[1], &num[2], &num[3]);
    char gateway[16] = {0};
    snprintf(gateway, sizeof(gateway), "%d.%d.%d.1", num[0], num[1], num[2]);
    CJSON_SET_STRING(new_json, "ipv4_gateway", gateway, goto end);
    CJSON_SET_NUMBER(new_json, "ipv6_enable", 0, goto end);
    CJSON_SET_NUMBER(new_json, "ipv6_dhcp_enable", 0, goto end);
    CJSON_SET_STRING(new_json, "ipv6_addr", "", goto end);
    CJSON_SET_STRING(new_json, "ipv6_mask", "", goto end);
    CJSON_SET_STRING(new_json, "ipv6_gateway", "", goto end);
    CJSON_SET_STRING(new_json, "dns", "8.8.8.8", goto end);
    CHECK_BOOL(cJSON_AddItemToObject(network_json, "network_addr", new_json), goto end);
    new_json = NULL;

    new_arr_json = cJSON_CreateArray();
    CHECK_POINTER(new_arr_json, goto end);

    new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, goto end);

    CJSON_SET_NUMBER(new_json, "enable", 1, goto end);
    CJSON_SET_NUMBER(new_json, "name", 0, goto end);
    int port[1] = {8080};
    CJSON_SET_NUMBER_LIST(new_json, "port", port, 1, goto end);
    CHECK_BOOL(cJSON_AddItemToArray(new_arr_json, new_json), goto end);
    new_json = NULL;

    new_json = cJSON_CreateObject();
    CHECK_POINTER(new_json, goto end);

    CJSON_SET_NUMBER(new_json, "enable", 1, goto end);
    CJSON_SET_NUMBER(new_json, "name", 2, goto end);
    port[1] = 554;
    CJSON_SET_NUMBER_LIST(new_json, "port", port, 1, goto end);
    CHECK_BOOL(cJSON_AddItemToArray(new_arr_json, new_json), goto end);
    CHECK_BOOL(cJSON_AddItemToObject(network_json, "network_protocols", new_arr_json), goto end);


    return network_json;
end:
    if (new_json != NULL) {
        cJSON_free(new_json);
    }
    if (new_arr_json != NULL) {
        cJSON_free(new_arr_json);
    }
    if (network_json) {
        cJSON_free(network_json);
    }
    return NULL;
}

static int OnvifOperationDefaultConfig() {
    kOnvifOperationMng.all_config = cJSON_CreateObject();
    CHECK_POINTER(kOnvifOperationMng.all_config, return -1);

    CHECK_BOOL(cJSON_AddItemToObject(kOnvifOperationMng.all_config, "scopes", OnvifOperationDefaultConfigScopes()), ERR_OPT_NULL);
    CHECK_BOOL(cJSON_AddItemToObject(kOnvifOperationMng.all_config, "users", OnvifOperationDefaultConfigUsers()), ERR_OPT_NULL);
    CHECK_BOOL(cJSON_AddItemToObject(kOnvifOperationMng.all_config, "profiles", OnvifOperationDefaultConfigProfile()), ERR_OPT_NULL);
    CHECK_BOOL(cJSON_AddItemToObject(kOnvifOperationMng.all_config, "network", OnvifOperationDefaultConfigNetwork()), ERR_OPT_NULL);

    return 0;
}

int OnvifOperationInit(OnvifOperationDeviceInfo device_info) {
    memcpy(&kOnvifOperationMng.device_info, &device_info, sizeof(OnvifOperationDeviceInfo));

    kOnvifOperationMng.event_list = ListCreate();
    CHECK_POINTER(kOnvifOperationMng.event_list, return -1;)

    kOnvifOperationMng.all_config = OnvifOperationLoadFile(ONVIF_CONFIG_FILE);
    if (kOnvifOperationMng.all_config == NULL) {
        LOG_WRN("load onvif.json fail, use memory config !");
        OnvifOperationDefaultConfig();
    }

    return 0;
}

void OnvifOperationUnInit() {
    if (kOnvifOperationMng.all_config != NULL) {
        cJSON_free(kOnvifOperationMng.all_config);
    }

    ListDestory(kOnvifOperationMng.event_list);
}

void OnvifOperationRegister(OnvifOperationType type, void* cb) {
    switch(type) {
    case ONVIF_OPERATION_GET_CONFIG:
        kOnvifOperationMng.oper_cb.get_config_cb = (OnvifOperGetConfigCb)cb;
        break;
    case ONVIF_OPERATION_SET_CONFIG:
        kOnvifOperationMng.oper_cb.set_config_cb = (OnvifOperSetConfigCb)cb;
        break;
    case ONVIF_OPERATION_PTZ_CTRL:
        kOnvifOperationMng.oper_cb.ptz_ctrl_cb = (OnvifOperPtzCtrlCb)cb;
        break;
    default:
        LOG_ERR("unknown type:%d", type);
        break;
    }
}

void OnvifOperationGetDevInfo(OnvifOperationDeviceInfo* device_info){
    memcpy(device_info, &kOnvifOperationMng.device_info, sizeof(OnvifOperationDeviceInfo));
}

static int OnvifOperationGetProfiles(cJSON** json) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(kOnvifOperationMng.oper_cb.get_config_cb, return -1);

    cJSON* new_json = NULL;
    cJSON* new_json_ = NULL;
    cJSON* new_arr_json = NULL;
    cJSON* profile_json = NULL;
    cJSON* profiles_json = cJSON_CreateArray();
    CHECK_POINTER(profiles_json, return -1);

    for (int i = 0; i < 1; i++) {
        profile_json = cJSON_CreateObject();
        CHECK_POINTER(profile_json, goto end);

        CJSON_SET_STRING(profile_json, "rtsp_url", kOnvifOperationMng.device_info.dev_info.rtsp_url, goto end);
        CJSON_SET_STRING(profile_json, "snap_url", kOnvifOperationMng.device_info.dev_info.snap_url, goto end);

        CJSON_SET_STRING(profile_json, "token", "media_token", goto end);
        CJSON_SET_STRING(profile_json, "name", "media_name", goto end);
        CJSON_SET_NUMBER(profile_json, "fixed", 1, goto end);

        OnvifVideoEncoder video_enc;
        memset(&video_enc, 0, sizeof(OnvifVideoEncoder));
        CHECK_LT(kOnvifOperationMng.oper_cb.get_config_cb(ONVIF_CONFIG_VIDEO_ENCODER, &video_enc, sizeof(OnvifVideoEncoder)), 0, goto end);
        CHECK_LT(StructToCjsonVideoEncoder(&video_enc, &new_json, &new_json_), 0, goto end);

        CJSON_SET_NUMBER(profile_json, "use_video_encoder_index", 0, goto end);
        CHECK_BOOL(cJSON_AddItemToObject(profile_json, "video_source", new_json_), goto end);
        new_arr_json = cJSON_CreateArray();
        CHECK_POINTER(new_arr_json, goto end);
        CHECK_BOOL(cJSON_AddItemToArray(new_arr_json, new_json), goto end);
        CHECK_BOOL(cJSON_AddItemToObject(profile_json, "compatible_video_encoder", new_arr_json), goto end);
        new_arr_json = NULL;
        new_json_ = NULL;
        new_json = NULL;

        OnvifAudioEncoder audio_enc;
        memset(&audio_enc, 0, sizeof(OnvifAudioEncoder));
        CHECK_LT(kOnvifOperationMng.oper_cb.get_config_cb(ONVIF_CONFIG_AUDIO_ENCODER, &audio_enc, sizeof(OnvifAudioEncoder)), 0, goto end);
        CHECK_LT(StructToCjsonAudioEncoder(&audio_enc, &new_json, &new_json_), 0, goto end);

        CJSON_SET_NUMBER(profile_json, "use_audio_encoder_index", 0, goto end);
        CHECK_BOOL(cJSON_AddItemToObject(profile_json, "audio_source", new_json_), goto end);
        new_arr_json = cJSON_CreateArray();
        CHECK_POINTER(new_arr_json, goto end);
        CHECK_BOOL(cJSON_AddItemToArray(new_arr_json, new_json), goto end);
        CHECK_BOOL(cJSON_AddItemToObject(profile_json, "compatible_audio_encoder", new_arr_json), goto end);
        new_arr_json = NULL;
        new_json_ = NULL;
        new_json = NULL;

        // ptz
        new_arr_json = cJSON_CreateArray();
        CHECK_POINTER(new_arr_json, goto end);
        for(int j = 0; j < 1; j++) {
            new_json = cJSON_CreateObject();
            CHECK_POINTER(new_json, goto end);

            CJSON_SET_STRING(new_json, "token", "ptz_token", goto end);
            CJSON_SET_STRING(new_json, "name", "ptz_name", goto end);
            CJSON_SET_STRING(new_json, "node_token", "ptz_node", goto end);

            CJSON_SET_NUMBER(new_json, "use_count", 1, goto end);          
            CJSON_SET_NUMBER(new_json, "ptz_timeout", 10*1000, goto end);

            CHECK_BOOL(cJSON_AddItemToArray(new_arr_json, new_json), goto end);
            new_json = NULL;
        }
        CJSON_SET_NUMBER(profile_json, "use_ptz_index", 0, goto end);
        CHECK_BOOL(cJSON_AddItemToObject(profile_json, "compatible_ptz", new_arr_json), goto end);
        new_arr_json = NULL;

        // presets
        OnvifPresets presets;
        memset(&presets, 0, sizeof(OnvifPresets));
        CHECK_LT(kOnvifOperationMng.oper_cb.get_config_cb(ONVIF_CONFIG_PRESETS, &presets, sizeof(OnvifPresets)), 0, goto end);
        CHECK_LT(StructToCjsonPresets(&presets, &new_json), 0, goto end);
        CHECK_BOOL(cJSON_AddItemToObject(profile_json, "presets", new_json), goto end);
        new_json = NULL;

        CHECK_BOOL(cJSON_AddItemToArray(profiles_json, profile_json), goto end);
    }

    *json = profiles_json;
    return 0;
end:
    if (new_json_ != NULL) {
        cJSON_free(new_json_);
    }
    if (new_json != NULL) {
        cJSON_free(new_json);
    }
    if (new_arr_json != NULL) {
        cJSON_free(new_arr_json);
    }
    if (profile_json != NULL) {
        cJSON_free(profile_json);
    }
    if (profiles_json != NULL) {
        cJSON_free(profiles_json);
    }
    return -1;
}

void* OnvifOperationGetConfig(const char* type) {
    if (strcmp(type, "profiles") == 0) {
        cJSON* json = NULL;
        if(OnvifOperationGetProfiles(&json) >= 0 && json != NULL) {
            cJSON_ReplaceItemInObjectCaseSensitive(kOnvifOperationMng.all_config, type, json);
        }
    }

    return cJSON_GetObjectItemCaseSensitive(kOnvifOperationMng.all_config, type);
}

static int OnvifOperationSetProfiles(cJSON* json) {
    // todo
    return 0;
}

void OnvifOperationSetConfig(const char* type, void* arg) {
    CHECK_POINTER(kOnvifOperationMng.oper_cb.set_config_cb, return );

    if (strcmp(type, "profiles") == 0) {
        OnvifOperationSetProfiles(arg);
    } else if (strcmp(type, "users") == 0 || strcmp(type, "scopes") == 0) {
        FILE* fp = fopen(ONVIF_CONFIG_FILE, "w+");
        CHECK_POINTER(fp, return);

        char* buff = cJSON_Print(kOnvifOperationMng.all_config);
        CHECK_POINTER(buff, fclose(fp);return);

        fwrite(buff, 1, strlen(buff), fp);

        free(buff);
        fflush(fp);
        fclose(fp);
    } else {
        if (strcmp(type, "presets") == 0) {
            kOnvifOperationMng.oper_cb.set_config_cb(ONVIF_CONFIG_PRESETS, arg, sizeof(OnvifPresets));
        }
    }
}

void OnvifOperationPtzCtrl(OnvifPtzCtrlType type, char* arg) {
    CHECK_POINTER(kOnvifOperationMng.oper_cb.ptz_ctrl_cb, return );

    kOnvifOperationMng.oper_cb.ptz_ctrl_cb(type, arg);
}

void OnvifOperationEventUpload(OnvifEventInfo* info) {
    pthread_mutex_lock(&kOnvifOperationMng.list_mutex);
    ListPush(kOnvifOperationMng.event_list, info, sizeof(OnvifEventInfo));
    pthread_mutex_unlock(&kOnvifOperationMng.list_mutex);
}

int OnvifOperationGetEventInfo(void** json, long long timeout) {
    CHECK_POINTER(json, return -1);

    cJSON* event_info = cJSON_CreateArray();
    CHECK_POINTER(event_info, return -1);
    
    int cnt = 0;
    while ((cnt++)*1000 < timeout) {
        pthread_mutex_lock(&kOnvifOperationMng.list_mutex);
        while(ListSize(kOnvifOperationMng.event_list) > 0) {
            OnvifEventInfo info;
            ListPop(kOnvifOperationMng.event_list, &info, sizeof(OnvifEventInfo));

            cJSON* item = NULL;
            CHECK_LT(StructToCjsonEventInfo(&info, &item), 0, break);
            CHECK_BOOL(cJSON_AddItemToArray(event_info, item), cJSON_free(item);break);
        }
        pthread_mutex_unlock(&kOnvifOperationMng.list_mutex);

        if (cJSON_GetArraySize(event_info) > 0) {
            *(cJSON**)json = event_info;
            break;
        }
        usleep(1000*1000);
    }

    if (cJSON_GetArraySize(event_info) == 0) {
        cJSON_free(event_info);
        return -1;
    }
    return 0;
}