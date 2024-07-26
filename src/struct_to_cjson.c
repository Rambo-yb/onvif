#include "struct_to_cjson.h"
#include "cjson_common.h"
#include "onvif_common.h"


int StructToCjsonVideoEncoder(void* st, cJSON** enc_json, cJSON** src_json) {
    CHECK_POINTER(st, return -1);
    OnvifVideoEncoder* encoder = (OnvifVideoEncoder*)st;
    cJSON* json_sub = NULL;
    cJSON* new_src_json = NULL;
    cJSON* new_enc_json = NULL;

    // video_source
    new_src_json = cJSON_CreateObject();
    CHECK_POINTER(new_src_json, goto end);

    CJSON_SET_STRING(new_src_json, "token", "video_source_token", goto end);
    CJSON_SET_STRING(new_src_json, "name", "video_source_name", goto end);
    CJSON_SET_STRING(new_src_json, "source_token", "video_token", goto end);
    CJSON_SET_NUMBER(new_src_json, "use_count", 1, goto end);

    json_sub = cJSON_CreateObject();
    CHECK_POINTER(json_sub, goto end);
    CJSON_SET_NUMBER(json_sub, "x", 0, goto end);
    CJSON_SET_NUMBER(json_sub, "y", 0, goto end);
    CJSON_SET_NUMBER(json_sub, "width", encoder->resolution_width, goto end);
    CJSON_SET_NUMBER(json_sub, "height", encoder->resolution_height, goto end);
    CHECK_BOOL(cJSON_AddItemToObject(new_src_json, "bounds", json_sub), goto end);
    json_sub = NULL;

    // video_encoder
    new_enc_json = cJSON_CreateObject();
    CHECK_POINTER(new_enc_json, goto end);

    CJSON_SET_STRING(new_enc_json, "token", "video_encoder_token", goto end);
    CJSON_SET_STRING(new_enc_json, "name", "video_encoder_name", goto end);
    CJSON_SET_NUMBER(new_enc_json, "use_count", 1, goto end);
    CJSON_SET_NUMBER(new_enc_json, "encoding", encoder->encoding, goto end);
    CJSON_SET_NUMBER(new_enc_json, "guaranteed_frame_rate", 0, goto end);

    json_sub = cJSON_CreateObject();
    CHECK_POINTER(json_sub, goto end);
    CJSON_SET_NUMBER(json_sub, "width", encoder->resolution_width, goto end);
    CJSON_SET_NUMBER(json_sub, "height", encoder->resolution_height, goto end);
    CHECK_BOOL(cJSON_AddItemToObject(new_enc_json, "resolution", json_sub), goto end);
    json_sub = NULL;

    CJSON_SET_NUMBER(new_enc_json, "quality", 100, goto end);

    json_sub = cJSON_CreateObject();
    CHECK_POINTER(json_sub, goto end);
    CJSON_SET_NUMBER(json_sub, "frame_rate_limit", encoder->gov_length*2, goto end);
    CJSON_SET_NUMBER(json_sub, "encoding_interval", encoder->gov_length, goto end);
    CJSON_SET_NUMBER(json_sub, "bitrate_limit", 2048, goto end);
    CHECK_BOOL(cJSON_AddItemToObject(new_enc_json, "rate_control", json_sub), goto end);
    json_sub = NULL;

    if (encoder->encoding == ONVIF_VIDEO_ENCODING_H264) {
        json_sub = cJSON_CreateObject();
        CHECK_POINTER(json_sub, goto end);
        CJSON_SET_NUMBER(json_sub, "gov_length", encoder->gov_length, goto end);
        CJSON_SET_NUMBER(json_sub, "h264_profile", encoder->profile, goto end);
        CHECK_BOOL(cJSON_AddItemToObject(new_enc_json, "h264", json_sub), goto end);
        json_sub = NULL;
    } else if (encoder->encoding == ONVIF_VIDEO_ENCODING_MPEG4) {
        json_sub = cJSON_CreateObject();
        CHECK_POINTER(json_sub, goto end);
        CJSON_SET_NUMBER(json_sub, "gov_length", encoder->gov_length, goto end);
        CJSON_SET_NUMBER(json_sub, "mpeg4_profile", encoder->profile, goto end);
        CHECK_BOOL(cJSON_AddItemToObject(new_enc_json, "mpeg4", json_sub), goto end);
        json_sub = NULL;
    }
    
    CJSON_SET_NUMBER(new_enc_json, "session_timeout", 10*1000, goto end);
    
    *enc_json = new_enc_json;
    *src_json = new_src_json;
    return 0;
end:
    if (json_sub != NULL) {
        cJSON_free(json_sub);
    }
    if (new_src_json != NULL) {
        cJSON_free(new_src_json);
    }
    if (new_enc_json != NULL) {
        cJSON_free(new_enc_json);
    }
    return -1;
}

int StructToCjsonAudioEncoder(void* st, cJSON** enc_json, cJSON** src_json) {
    CHECK_POINTER(st, return -1);
    OnvifAudioEncoder* encoder = (OnvifAudioEncoder*)st;
    cJSON* new_src_json = NULL;
    cJSON* new_enc_json = NULL;

    // audio_source
    new_src_json = cJSON_CreateObject();
    CHECK_POINTER(new_src_json, goto end);

    CJSON_SET_STRING(new_src_json, "token", "audio_source_token", goto end);
    CJSON_SET_STRING(new_src_json, "name", "audio_source_name", goto end);
    CJSON_SET_STRING(new_src_json, "source_token", "audio_token", goto end);
    CJSON_SET_NUMBER(new_src_json, "use_count", 1, goto end);
    CJSON_SET_NUMBER(new_src_json, "channels", encoder->channels, goto end);

    // audio_encoder
    new_enc_json = cJSON_CreateObject();
    CHECK_POINTER(new_enc_json, goto end);

    CJSON_SET_STRING(new_enc_json, "token", "video_encoder_token", goto end);
    CJSON_SET_STRING(new_enc_json, "name", "video_encoder_name", goto end);
    CJSON_SET_NUMBER(new_enc_json, "use_count", 1, goto end);
    CJSON_SET_NUMBER(new_enc_json, "encoding", encoder->encoding, goto end);
    CJSON_SET_NUMBER(new_enc_json, "bitrate", encoder->bitrate, goto end);           
    CJSON_SET_NUMBER(new_enc_json, "sample_rate", encoder->sample_rate, goto end);           
    CJSON_SET_NUMBER(new_enc_json, "session_timeout", 10*1000, goto end);

    *enc_json = new_enc_json;
    *src_json = new_src_json;
    return 0;
end:
    if (new_src_json != NULL) {
        cJSON_free(new_src_json);
    }
    if (new_enc_json != NULL) {
        cJSON_free(new_enc_json);
    }
    return -1; 
}

int StructToCjsonPresets(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    OnvifPresets* presets = (OnvifPresets*)st;

    cJSON* new_json = NULL;
    cJSON* presets_json = cJSON_CreateArray();
    CHECK_POINTER(presets_json, goto end);

    for(int i = 0; i < presets->preset_num; i++) {
        new_json = cJSON_CreateObject();
        CHECK_POINTER(new_json, goto end);

        CJSON_SET_STRING(new_json, "token", presets->preset[i], goto end);
        CJSON_SET_STRING(new_json, "name", presets->preset[i], goto end);
        
        CHECK_BOOL(cJSON_AddItemToArray(presets_json, new_json), goto end) ;
    }

    *json = presets_json;
    return 0;
end:
    if (new_json != NULL) {
        cJSON_free(new_json);
    }
    if (presets_json != NULL) {
        cJSON_free(presets_json);
    }

    return -1;
}

int StructToCjsonEventInfo(void* st, cJSON** json) {
    CHECK_POINTER(st, return -1);
    OnvifEventInfo* event_info = (OnvifEventInfo*)st;

    cJSON* event_json = cJSON_CreateObject();
    CHECK_POINTER(event_json, goto end);

    CJSON_SET_STRING(event_json, "type", event_info->event_type, goto end);
    CJSON_SET_NUMBER(event_json, "time", event_info->time, goto end);
    CJSON_SET_NUMBER(event_json, "state", event_info->state, goto end);

    *json = event_json;
    return 0;
end:
    if (event_json != NULL) {
        cJSON_free(event_json);
    }
    return -1;
}