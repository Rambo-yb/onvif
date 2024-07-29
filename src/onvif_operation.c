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

static int OnvifOperationDefaultConfig() {
    kOnvifOperationMng.all_config = cJSON_CreateObject();
    CHECK_POINTER(kOnvifOperationMng.all_config, return -1);

    CHECK_BOOL(cJSON_AddItemToObject(kOnvifOperationMng.all_config, "users", OnvifOperationDefaultConfigUsers()), );
    CHECK_BOOL(cJSON_AddItemToObject(kOnvifOperationMng.all_config, "profiles", OnvifOperationDefaultConfigProfile()), );

    return 0;
}

int OnvifOperationInit(OnvifOperationDeviceInfo device_info, OnvifOperCb cb) {
    kOnvifOperationMng.event_list = ListCreate();
    CHECK_POINTER(kOnvifOperationMng.event_list, return -1;)

    kOnvifOperationMng.all_config = OnvifOperationLoadFile("/root/onvif.json");
    if (kOnvifOperationMng.all_config == NULL) {
        LOG_WRN("load onvif.json fail, use memory config !");
        OnvifOperationDefaultConfig();
    }

    kOnvifOperationMng.oper_cb = cb;
    memcpy(&kOnvifOperationMng.device_info, &device_info, sizeof(OnvifOperationDeviceInfo));

    return 0;
}

void OnvifOperationUnInit() {
    if (kOnvifOperationMng.all_config != NULL) {
        cJSON_free(kOnvifOperationMng.all_config);
    }

    ListDestory(kOnvifOperationMng.event_list);
}

void OnvifOperationGetDevInfo(OnvifOperationDeviceInfo* device_info){
    memcpy(device_info, &kOnvifOperationMng.device_info, sizeof(OnvifOperationDeviceInfo));
}

static int OnvifOperationGetProfiles(cJSON** json) {
    CHECK_POINTER(json, return -1);
    CHECK_POINTER(kOnvifOperationMng.oper_cb, return -1);

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
        CHECK_LT(kOnvifOperationMng.oper_cb(ONVIF_OPER_GET_CONFIG, "video_encoder", &video_enc), 0, goto end);
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
        CHECK_LT(kOnvifOperationMng.oper_cb(ONVIF_OPER_GET_CONFIG, "audio_encoder", &audio_enc), 0, goto end);
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
        CHECK_LT(kOnvifOperationMng.oper_cb(ONVIF_OPER_GET_CONFIG, "presets", &presets), 0, goto end);
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
    CHECK_POINTER(kOnvifOperationMng.oper_cb, return );

    if (strcmp(type, "profiles") == 0) {
        OnvifOperationSetProfiles(arg);
    } else {
        kOnvifOperationMng.oper_cb(ONVIF_OPER_SET_CONFIG, (void*)type, arg);
    }

}

void OnvifOperationPtzCtrl(OnvifPtzCtrlType type, char* arg) {
    CHECK_POINTER(kOnvifOperationMng.oper_cb, return );

    kOnvifOperationMng.oper_cb(ONVIF_OPER_PTZ_CTRL, &type, arg);
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