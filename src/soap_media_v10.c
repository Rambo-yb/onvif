#include "soapStub.h"
#include "onvif_operation.h"
#include "check_common.h"
#include "soap_common.h"
#include "auth.h"
#include "cJSON.h"
#include "cjson_common.h"

static void GetVideoSourceConfigurationFromJson(struct soap* soap, cJSON* json, struct tt__VideoSourceConfiguration* conf_video_source) {
    SOAP_CJSON_GET_STRING(json, soap, "token", conf_video_source->token);
    SOAP_CJSON_GET_STRING(json, soap, "name", conf_video_source->Name);
    SOAP_CJSON_GET_NUMBER(json, soap, "use_count", conf_video_source->UseCount);
    SOAP_CJSON_GET_STRING(json, soap, "source_token", conf_video_source->SourceToken);
    SOAP_CJSON_GET_STRING(json, soap, "view_mode", conf_video_source->ViewMode);

    conf_video_source->Bounds = (struct tt__IntRectangle*)soap_malloc(soap, sizeof(struct tt__IntRectangle));
    conf_video_source->Bounds->x = 0;
    conf_video_source->Bounds->y = 0;
    conf_video_source->Bounds->width = 640;
    conf_video_source->Bounds->height = 512;
    cJSON* bounds_json = cJSON_GetObjectItemCaseSensitive(json, "bounds");
    if (bounds_json != NULL && cJSON_IsObject(bounds_json)) {
        SOAP_CJSON_GET_NUMBER(bounds_json, soap, "width", conf_video_source->Bounds->width);
        SOAP_CJSON_GET_NUMBER(bounds_json, soap, "height", conf_video_source->Bounds->height);
    }

    cJSON* extension_json = cJSON_GetObjectItemCaseSensitive(json, "extension");
    if (extension_json != NULL && cJSON_IsObject(extension_json)) {
        // todo
        LOG_WRN("extension not support!");
    }
}

static void GetAudioSourceConfigurationFromJson(struct soap* soap, cJSON* json, struct tt__AudioSourceConfiguration* conf_audio_source) {
    SOAP_CJSON_GET_STRING(json, soap, "token", conf_audio_source->token);
    SOAP_CJSON_GET_STRING(json, soap, "name", conf_audio_source->Name);
    SOAP_CJSON_GET_NUMBER(json, soap, "use_count", conf_audio_source->UseCount);
    SOAP_CJSON_GET_STRING(json, soap, "source_token", conf_audio_source->SourceToken);
}

static void GetVideoEncoderConfigurationFromJson(struct soap* soap, cJSON* json, struct tt__VideoEncoderConfiguration* conf_video_encoder) {
    SOAP_CJSON_GET_STRING(json, soap, "token", conf_video_encoder->token);
    SOAP_CJSON_GET_STRING(json, soap, "name", conf_video_encoder->Name);
    SOAP_CJSON_GET_NUMBER(json, soap, "use_count", conf_video_encoder->UseCount);
    conf_video_encoder->GuaranteedFrameRate = soap_malloc(soap, sizeof(enum xsd__boolean));
    SOAP_CJSON_GET_NUMBER(json, soap, "guaranteed_frame_rate", *(conf_video_encoder->GuaranteedFrameRate));
    SOAP_CJSON_GET_NUMBER(json, soap, "encoding", conf_video_encoder->Encoding);
    SOAP_CJSON_GET_NUMBER(json, soap, "quality", conf_video_encoder->Quality);
    SOAP_CJSON_GET_NUMBER(json, soap, "session_timeout", conf_video_encoder->SessionTimeout);

    conf_video_encoder->Resolution = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution));
    conf_video_encoder->Resolution->Width = 640;
    conf_video_encoder->Resolution->Height = 512;
    cJSON* resolution_json = cJSON_GetObjectItemCaseSensitive(json, "resolution");
    if (resolution_json != NULL && cJSON_IsObject(resolution_json)) {
        SOAP_CJSON_GET_NUMBER(resolution_json, soap, "width", conf_video_encoder->Resolution->Width);
        SOAP_CJSON_GET_NUMBER(resolution_json, soap, "height", conf_video_encoder->Resolution->Height);
    }

    cJSON* rate_control_json = cJSON_GetObjectItemCaseSensitive(json, "rate_control");
    if (rate_control_json != NULL && cJSON_IsObject(rate_control_json)) {
        conf_video_encoder->RateControl = (struct tt__VideoRateControl*)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
        SOAP_CJSON_GET_NUMBER(rate_control_json, soap, "frame_rate_limit", conf_video_encoder->RateControl->FrameRateLimit);
        SOAP_CJSON_GET_NUMBER(rate_control_json, soap, "encoding_interval", conf_video_encoder->RateControl->EncodingInterval);
        SOAP_CJSON_GET_NUMBER(rate_control_json, soap, "bitrate_limit", conf_video_encoder->RateControl->BitrateLimit);
    }

    cJSON* mpeg4_json = cJSON_GetObjectItemCaseSensitive(json, "mpeg4");
    if (mpeg4_json != NULL && cJSON_IsObject(mpeg4_json)) {
        conf_video_encoder->MPEG4 = (struct tt__Mpeg4Configuration*)soap_malloc(soap, sizeof(struct tt__Mpeg4Configuration));
        SOAP_CJSON_GET_NUMBER(mpeg4_json, soap, "gov_length", conf_video_encoder->MPEG4->GovLength);
        SOAP_CJSON_GET_NUMBER(mpeg4_json, soap, "mpeg4_profile", conf_video_encoder->MPEG4->Mpeg4Profile);
    }

    cJSON* h264_json = cJSON_GetObjectItemCaseSensitive(json, "h264");
    if (h264_json != NULL && cJSON_IsObject(h264_json)) {
        conf_video_encoder->H264 = (struct tt__H264Configuration*)soap_malloc(soap, sizeof(struct tt__H264Configuration));
        SOAP_CJSON_GET_NUMBER(h264_json, soap, "gov_length", conf_video_encoder->H264->GovLength);
        SOAP_CJSON_GET_NUMBER(h264_json, soap, "h264_profile", conf_video_encoder->H264->H264Profile);
    }

    cJSON* multicast_json = cJSON_GetObjectItemCaseSensitive(json, "multicast");
    if (multicast_json != NULL && cJSON_IsObject(multicast_json)) {
        // todo
        LOG_WRN("multicast not support!");
    }
}

static void GetAudioEncoderConfigurationFromJson(struct soap* soap, cJSON* json, struct tt__AudioEncoderConfiguration* conf_audio_encoder) {
    SOAP_CJSON_GET_STRING(json, soap, "token", conf_audio_encoder->token);
    SOAP_CJSON_GET_STRING(json, soap, "name", conf_audio_encoder->Name);
    SOAP_CJSON_GET_NUMBER(json, soap, "use_count", conf_audio_encoder->UseCount);
    SOAP_CJSON_GET_NUMBER(json, soap, "encoding", conf_audio_encoder->Encoding);
    SOAP_CJSON_GET_NUMBER(json, soap, "bitrate", conf_audio_encoder->Bitrate);
    SOAP_CJSON_GET_NUMBER(json, soap, "sample_rate", conf_audio_encoder->SampleRate);
    SOAP_CJSON_GET_NUMBER(json, soap, "session_timeout", conf_audio_encoder->SessionTimeout);

    cJSON* multicast_json = cJSON_GetObjectItemCaseSensitive(json, "multicast");
    if (multicast_json != NULL && cJSON_IsObject(multicast_json)) {
        // todo
        LOG_WRN("multicast not support!");
    }
}

static void GetPtzConfigurationFromJson(struct soap* soap, cJSON* json, struct tt__PTZConfiguration* conf_ptz) {
    SOAP_CJSON_GET_STRING(json, soap, "token", conf_ptz->token);
    SOAP_CJSON_GET_STRING(json, soap, "name", conf_ptz->Name);
    SOAP_CJSON_GET_NUMBER(json, soap, "use_count", conf_ptz->UseCount);
    SOAP_CJSON_GET_STRING(json, soap, "node_token", conf_ptz->NodeToken);
    conf_ptz->DefaultPTZTimeout = soap_malloc(soap, sizeof(LONG64));
    SOAP_CJSON_GET_NUMBER(json, soap, "ptz_timeout", *(conf_ptz->DefaultPTZTimeout));

    conf_ptz->DefaultAbsolutePantTiltPositionSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
    conf_ptz->DefaultAbsoluteZoomPositionSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
    conf_ptz->DefaultRelativePanTiltTranslationSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");
    conf_ptz->DefaultRelativeZoomTranslationSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");
    conf_ptz->DefaultContinuousPanTiltVelocitySpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace");
    conf_ptz->DefaultContinuousZoomVelocitySpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace");

    cJSON* ptz_speed_json = cJSON_GetObjectItemCaseSensitive(json, "default_ptz_speed");
    if (ptz_speed_json != NULL && cJSON_IsObject(ptz_speed_json)) {
        // todo
        // http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace
        // http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace
        LOG_WRN("default ptz speed not support!");
    }
    
    cJSON* pan_tilt_limits_json = cJSON_GetObjectItemCaseSensitive(json, "pan_tilt_limits");
    if (pan_tilt_limits_json != NULL && cJSON_IsObject(pan_tilt_limits_json)) {
        // todo
        // http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace
        LOG_WRN("pan tilt limits not support!");
    }

    cJSON* zoom_limits_json = cJSON_GetObjectItemCaseSensitive(json, "zoom_limits");
    if (zoom_limits_json != NULL && cJSON_IsObject(zoom_limits_json)) {
        // todo
        // http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace
        LOG_WRN("zoom limits not support!");
    }
}

static void GetProfile(struct soap* soap, cJSON* json, struct tt__Profile* profile) {
    SOAP_CJSON_GET_STRING(json, soap, "token", profile->token);
    SOAP_CJSON_GET_STRING(json, soap, "name", profile->Name);
    profile->fixed = soap_malloc(soap, sizeof(enum xsd__boolean));
    SOAP_CJSON_GET_NUMBER(json, soap, "fixed", *(profile->fixed));

    cJSON* video_source_json = cJSON_GetObjectItemCaseSensitive(json, "video_source");
    if (video_source_json != NULL && cJSON_IsObject(video_source_json)) {
        profile->VideoSourceConfiguration = (struct tt__VideoSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration));
        memset(profile->VideoSourceConfiguration, 0, sizeof(struct tt__VideoSourceConfiguration));
        GetVideoSourceConfigurationFromJson(soap, video_source_json, profile->VideoSourceConfiguration);
    }

    cJSON* audio_source_json = cJSON_GetObjectItemCaseSensitive(json, "audio_source");
    if (audio_source_json != NULL && cJSON_IsObject(audio_source_json)) {
        profile->AudioSourceConfiguration = (struct tt__AudioSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__AudioSourceConfiguration));
        memset(profile->AudioSourceConfiguration, 0, sizeof(struct tt__AudioSourceConfiguration));
        GetAudioSourceConfigurationFromJson(soap, audio_source_json, profile->AudioSourceConfiguration);
    }

    int video_encoder_index = 0;
    SOAP_CJSON_GET_NUMBER(json, soap, "use_video_encoder_index", video_encoder_index);
    cJSON* video_encoder_json = cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(json, "compatible_video_encoder"), video_encoder_index);
    if (video_encoder_json != NULL && cJSON_IsObject(video_encoder_json)) {
        profile->VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration));
        memset(profile->VideoEncoderConfiguration, 0, sizeof(struct tt__VideoEncoderConfiguration));
        GetVideoEncoderConfigurationFromJson(soap, video_encoder_json, profile->VideoEncoderConfiguration);
    }

    int audio_encoder_index = 0;
    SOAP_CJSON_GET_NUMBER(json, soap, "use_audio_encoder_index", audio_encoder_index);
    cJSON* audio_encoder_json = cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(json, "compatible_audio_encoder"), audio_encoder_index);
    if (audio_encoder_json != NULL && cJSON_IsObject(audio_encoder_json)) {
        profile->AudioEncoderConfiguration = (struct tt__AudioEncoderConfiguration*)soap_malloc(soap, sizeof(struct tt__AudioEncoderConfiguration));
        memset(profile->AudioEncoderConfiguration, 0, sizeof(struct tt__AudioEncoderConfiguration));
        GetAudioEncoderConfigurationFromJson(soap, audio_encoder_json, profile->AudioEncoderConfiguration);
    }

    int ptz_index = 0;
    SOAP_CJSON_GET_NUMBER(json, soap, "use_ptz_index", ptz_index);
    cJSON* ptz_json = cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(json, "compatible_ptz"), ptz_index);
    if (ptz_json != NULL && cJSON_IsObject(ptz_json)) {
        profile->PTZConfiguration = (struct tt__PTZConfiguration*)soap_malloc(soap, sizeof(struct tt__PTZConfiguration));
        memset(profile->PTZConfiguration, 0, sizeof(struct tt__PTZConfiguration));
        GetPtzConfigurationFromJson(soap, ptz_json, profile->PTZConfiguration);
    }
}

/** Web service operation '__trt__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetServiceCapabilities(struct soap* soap, struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities, struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetVideoSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSources(struct soap* soap, struct _trt__GetVideoSources *trt__GetVideoSources, struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    trt__GetVideoSourcesResponse->__sizeVideoSources = cJSON_GetArraySize(profiles_json);
    trt__GetVideoSourcesResponse->VideoSources = (struct tt__VideoSource*)soap_malloc(soap, sizeof(struct tt__VideoSource)*trt__GetVideoSourcesResponse->__sizeVideoSources);
    memset(trt__GetVideoSourcesResponse->VideoSources, 0, sizeof(struct tt__VideoSource)*trt__GetVideoSourcesResponse->__sizeVideoSources);
    for(int i = 0; i < trt__GetVideoSourcesResponse->__sizeVideoSources; i++) {
        cJSON* profile_json = cJSON_GetArrayItem(profiles_json, i);
        if (profile_json == NULL || !cJSON_IsObject(profile_json)) {
            continue;
        }

        cJSON* video_source_json = cJSON_GetObjectItemCaseSensitive(profile_json, "video_source");
        if (video_source_json == NULL || !cJSON_IsObject(video_source_json)) {
            continue;
        }

        SOAP_CJSON_GET_STRING(video_source_json, soap, "source_token", trt__GetVideoSourcesResponse->VideoSources[i].token);

        int video_encoder_index = 0;
        SOAP_CJSON_GET_NUMBER(profile_json, soap, "use_video_encoder_index", video_encoder_index);
        cJSON* video_encoder_json = cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(profile_json, "compatible_video_encoder"), video_encoder_index);
        if (video_encoder_json == NULL || !cJSON_IsObject(video_encoder_json)) {
            continue;
        }

        trt__GetVideoSourcesResponse->VideoSources[i].Framerate = 30;
        cJSON* mpeg4_json = cJSON_GetObjectItemCaseSensitive(video_encoder_json, "mpeg4");
        cJSON* h264_json = cJSON_GetObjectItemCaseSensitive(video_encoder_json, "h264");
        if (mpeg4_json != NULL && cJSON_IsObject(mpeg4_json)) {
            SOAP_CJSON_GET_NUMBER(mpeg4_json, soap, "gov_length", trt__GetVideoSourcesResponse->VideoSources[i].Framerate);
        } else if (h264_json != NULL && cJSON_IsObject(h264_json)) {
            SOAP_CJSON_GET_NUMBER(h264_json, soap, "gov_length", trt__GetVideoSourcesResponse->VideoSources[i].Framerate);
        }

        trt__GetVideoSourcesResponse->VideoSources[i].Resolution = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution));
        trt__GetVideoSourcesResponse->VideoSources[i].Resolution->Width = 640;
        trt__GetVideoSourcesResponse->VideoSources[i].Resolution->Height = 512;
        cJSON* resolution_json = cJSON_GetObjectItemCaseSensitive(video_encoder_json, "resolution");
        if (resolution_json != NULL && cJSON_IsObject(resolution_json)) {
            SOAP_CJSON_GET_NUMBER(resolution_json, soap, "width", trt__GetVideoSourcesResponse->VideoSources[i].Resolution->Width);
            SOAP_CJSON_GET_NUMBER(resolution_json, soap, "height", trt__GetVideoSourcesResponse->VideoSources[i].Resolution->Height);
        }
    }

    return 0;
}
/** Web service operation '__trt__GetAudioSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSources(struct soap* soap, struct _trt__GetAudioSources *trt__GetAudioSources, struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    trt__GetAudioSourcesResponse->__sizeAudioSources = cJSON_GetArraySize(profiles_json);;
    trt__GetAudioSourcesResponse->AudioSources = (struct tt__AudioSource*)soap_malloc(soap, sizeof(struct tt__AudioSource)*trt__GetAudioSourcesResponse->__sizeAudioSources);
    for(int i = 0; i < trt__GetAudioSourcesResponse->__sizeAudioSources; i++) {
        cJSON* audio_source_json = cJSON_GetObjectItemCaseSensitive(cJSON_GetArrayItem(profiles_json, i), "video_source");
        if (audio_source_json == NULL || !cJSON_IsObject(audio_source_json)) {
            continue;
        }

        SOAP_CJSON_GET_STRING(audio_source_json, soap, "source_token", trt__GetAudioSourcesResponse->AudioSources[i].token);
        SOAP_CJSON_GET_NUMBER(audio_source_json, soap, "channels", trt__GetAudioSourcesResponse->AudioSources[i].Channels);
    }

    return 0;
}
/** Web service operation '__trt__GetAudioOutputs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputs(struct soap* soap, struct _trt__GetAudioOutputs *trt__GetAudioOutputs, struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__CreateProfile' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateProfile(struct soap* soap, struct _trt__CreateProfile *trt__CreateProfile, struct _trt__CreateProfileResponse *trt__CreateProfileResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetProfile' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfile(struct soap* soap, struct _trt__GetProfile *trt__GetProfile, struct _trt__GetProfileResponse *trt__GetProfileResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    for(int i = 0; i < cJSON_GetArraySize(profiles_json); i++) {
        cJSON* profile_json = cJSON_GetArrayItem(profiles_json, i);
        if (profile_json == NULL || !cJSON_IsObject(profile_json)) {
            continue;
        }

        if (strcmp(trt__GetProfile->ProfileToken, cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(profile_json, "token"))) == 0) {
            trt__GetProfileResponse->Profile = (struct tt__Profile*)soap_malloc(soap, sizeof(struct tt__Profile));
            memset(trt__GetProfileResponse->Profile, 0, sizeof(struct tt__Profile));
            GetProfile(soap, profile_json, trt__GetProfileResponse->Profile); 
            break;
        }
    }

    return 0;
}
/** Web service operation '__trt__GetProfiles' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfiles(struct soap* soap, struct _trt__GetProfiles *trt__GetProfiles, struct _trt__GetProfilesResponse *trt__GetProfilesResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    trt__GetProfilesResponse->__sizeProfiles = cJSON_GetArraySize(profiles_json);
    trt__GetProfilesResponse->Profiles = (struct tt__Profile*)soap_malloc(soap, sizeof(struct tt__Profile)*trt__GetProfilesResponse->__sizeProfiles);
    memset(trt__GetProfilesResponse->Profiles, 0, sizeof(struct tt__Profile)*trt__GetProfilesResponse->__sizeProfiles);
    for(int i = 0; i < trt__GetProfilesResponse->__sizeProfiles; i++) {
        cJSON* profile_json = cJSON_GetArrayItem(profiles_json, i);
        CHECK_POINTER(profile_json, return 500);

        GetProfile(soap, profile_json, &trt__GetProfilesResponse->Profiles[i]);
    }

    return 0;
}
/** Web service operation '__trt__AddVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoEncoderConfiguration(struct soap* soap, struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__AddVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoSourceConfiguration(struct soap* soap, struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    LOG_INFO("profile:%s, token:%s", trt__AddVideoSourceConfiguration->ProfileToken, trt__AddVideoSourceConfiguration->ConfigurationToken);
    return 0;
}
/** Web service operation '__trt__AddAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioEncoderConfiguration(struct soap* soap, struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__AddAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioSourceConfiguration(struct soap* soap, struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__AddPTZConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddPTZConfiguration(struct soap* soap, struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration, struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__AddVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoAnalyticsConfiguration(struct soap* soap, struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__AddMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddMetadataConfiguration(struct soap* soap, struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__AddAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioOutputConfiguration(struct soap* soap, struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__AddAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioDecoderConfiguration(struct soap* soap, struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__RemoveVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoEncoderConfiguration(struct soap* soap, struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__RemoveVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoSourceConfiguration(struct soap* soap, struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__RemoveAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioEncoderConfiguration(struct soap* soap, struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__RemoveAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioSourceConfiguration(struct soap* soap, struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__RemovePTZConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemovePTZConfiguration(struct soap* soap, struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__RemoveVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoAnalyticsConfiguration(struct soap* soap, struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__RemoveMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveMetadataConfiguration(struct soap* soap, struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__RemoveAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioOutputConfiguration(struct soap* soap, struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__RemoveAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioDecoderConfiguration(struct soap* soap, struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__DeleteProfile' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteProfile(struct soap* soap, struct _trt__DeleteProfile *trt__DeleteProfile, struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetVideoSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurations(struct soap* soap, struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations = cJSON_GetArraySize(profiles_json);
    trt__GetVideoSourceConfigurationsResponse->Configurations = (struct tt__VideoSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration)*trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations);
    memset(trt__GetVideoSourceConfigurationsResponse->Configurations, 0, sizeof(struct tt__VideoSourceConfiguration)*trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations);
    for(int i = 0; i < trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations; i++) {
        cJSON* profile_json = cJSON_GetArrayItem(profiles_json, i);
        CHECK_POINTER(profile_json, return 500);

        cJSON* video_source_json = cJSON_GetObjectItemCaseSensitive(profile_json, "video_source");
        if (video_source_json != NULL && cJSON_IsObject(video_source_json)) {
            GetVideoSourceConfigurationFromJson(soap, video_source_json, &trt__GetVideoSourceConfigurationsResponse->Configurations[i]);
        }
    }

    return 0;
}
/** Web service operation '__trt__GetVideoEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurations(struct soap* soap, struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetAudioSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurations(struct soap* soap, struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations = cJSON_GetArraySize(profiles_json);
    trt__GetAudioSourceConfigurationsResponse->Configurations = (struct tt__AudioSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__AudioSourceConfiguration)*trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations);
    memset(trt__GetAudioSourceConfigurationsResponse->Configurations, 0, sizeof(struct tt__AudioSourceConfiguration)*trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations);
    for(int i = 0; i < trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations; i++) {
        cJSON* profile_json = cJSON_GetArrayItem(profiles_json, i);
        CHECK_POINTER(profile_json, return 500);

        cJSON* audio_source_json = cJSON_GetObjectItemCaseSensitive(profile_json, "audio_source");
        if (audio_source_json != NULL && cJSON_IsObject(audio_source_json)) {
            GetAudioSourceConfigurationFromJson(soap, audio_source_json, &trt__GetAudioSourceConfigurationsResponse->Configurations[i]);
        }
    }
    return 0;
}
/** Web service operation '__trt__GetAudioEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurations(struct soap* soap, struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetVideoAnalyticsConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfigurations(struct soap* soap, struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetMetadataConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurations(struct soap* soap, struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetAudioOutputConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurations(struct soap* soap, struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetAudioDecoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurations(struct soap* soap, struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfiguration(struct soap* soap, struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);
    
    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    for(int i = 0; i < cJSON_GetArraySize(profiles_json); i++) {
        cJSON* video_source_json = cJSON_GetObjectItemCaseSensitive(cJSON_GetArrayItem(profiles_json, i), "video_source");
        if (video_source_json == NULL || !cJSON_IsObject(video_source_json)) {
            continue;
        }

        if (strcmp(trt__GetVideoSourceConfiguration->ConfigurationToken, cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(video_source_json, "token"))) == 0) {
            trt__GetVideoSourceConfigurationResponse->Configuration = (struct tt__VideoSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration));
            memset(trt__GetVideoSourceConfigurationResponse->Configuration, 0, sizeof(struct tt__VideoSourceConfiguration));
            GetVideoSourceConfigurationFromJson(soap, video_source_json, trt__GetVideoSourceConfigurationResponse->Configuration);
            break;
        }
    }

    return 0;
}
/** Web service operation '__trt__GetVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfiguration(struct soap* soap, struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfiguration(struct soap* soap, struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfiguration(struct soap* soap, struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfiguration(struct soap* soap, struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfiguration(struct soap* soap, struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfiguration(struct soap* soap, struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfiguration(struct soap* soap, struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetCompatibleVideoEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoEncoderConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    for(int i = 0; i < cJSON_GetArraySize(profiles_json); i++) {
        cJSON* profile_json = cJSON_GetArrayItem(profiles_json, i);
        if (profile_json == NULL || !cJSON_IsObject(profile_json)) {
            continue;
        }

        if (strcmp(trt__GetCompatibleVideoEncoderConfigurations->ProfileToken, cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(profile_json, "token"))) == 0) {
            cJSON* video_encoder_arr_json = cJSON_GetObjectItemCaseSensitive(profile_json, "compatible_video_encoder");
            CHECK_POINTER(video_encoder_arr_json, return 500);
            CHECK_BOOL(cJSON_IsArray(video_encoder_arr_json), return 500);

            trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations = cJSON_GetArraySize(video_encoder_arr_json);
            trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations = (struct tt__VideoEncoderConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration)*trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations);
            memset(trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations, 0, sizeof(struct tt__VideoEncoderConfiguration)*trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations);
            for(int j = 0; j < trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations; j++) {
                cJSON* video_encoder_json = cJSON_GetArrayItem(video_encoder_arr_json, j);
                if (video_encoder_json == NULL || !cJSON_IsObject(video_encoder_json)) {
                    continue;
                }

                GetVideoEncoderConfigurationFromJson(soap, video_encoder_json, &trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations[j]);
            }
            break;
        }
    }

    return 0;
}
/** Web service operation '__trt__GetCompatibleVideoSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoSourceConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetCompatibleAudioEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioEncoderConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetCompatibleAudioSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioSourceConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetCompatibleVideoAnalyticsConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoAnalyticsConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetCompatibleMetadataConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleMetadataConfigurations(struct soap* soap, struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetCompatibleAudioOutputConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioOutputConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetCompatibleAudioDecoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioDecoderConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__SetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceConfiguration(struct soap* soap, struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__SetVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoEncoderConfiguration(struct soap* soap, struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    CHECK_POINTER(trt__SetVideoEncoderConfiguration->Configuration, return 500);
    
    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    int ret = 0;
    for(int i = 0; i < cJSON_GetArraySize(profiles_json); i++) {
        cJSON* profile_json = cJSON_GetArrayItem(profiles_json, i);
        if (profile_json == NULL || !cJSON_IsObject(profile_json)) {
            continue;
        }

        int index = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(profile_json, "use_video_encoder_index"));
        cJSON* encoder_arr_json = cJSON_GetObjectItemCaseSensitive(profile_json, "compatible_video_encoder");
        cJSON* encoder_json = cJSON_GetArrayItem(encoder_arr_json, index);
        if (encoder_json == NULL || !cJSON_IsObject(encoder_json)
            || strcmp(trt__SetVideoEncoderConfiguration->Configuration->token, cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(encoder_json, "token"))) != 0) {
            continue;
        }

        cJSON* new_sub_json = NULL;
        cJSON* new_encoder_json = cJSON_CreateObject();
        CHECK_POINTER(new_encoder_json, goto end);
        CJSON_SET_STRING(new_encoder_json, "token", trt__SetVideoEncoderConfiguration->Configuration->token, goto end);
        CJSON_SET_STRING(new_encoder_json, "name", trt__SetVideoEncoderConfiguration->Configuration->Name, goto end);
        CJSON_SET_NUMBER(new_encoder_json, "use_count", trt__SetVideoEncoderConfiguration->Configuration->UseCount, goto end);
        CJSON_SET_NUMBER(new_encoder_json, "encoding", trt__SetVideoEncoderConfiguration->Configuration->Encoding, goto end);
        CJSON_SET_NUMBER(new_encoder_json, "guaranteed_frame_rate", *(trt__SetVideoEncoderConfiguration->Configuration->GuaranteedFrameRate), goto end);
        CJSON_SET_NUMBER(new_encoder_json, "quality", trt__SetVideoEncoderConfiguration->Configuration->Quality, goto end);
        CJSON_SET_NUMBER(new_encoder_json, "session_timeout", trt__SetVideoEncoderConfiguration->Configuration->SessionTimeout, goto end);
        
        if (trt__SetVideoEncoderConfiguration->Configuration->Resolution != NULL) {
            new_sub_json = cJSON_CreateObject();
            CHECK_POINTER(new_sub_json, goto end);
            CJSON_SET_NUMBER(new_sub_json, "width", trt__SetVideoEncoderConfiguration->Configuration->Resolution->Width, goto end);
            CJSON_SET_NUMBER(new_sub_json, "height", trt__SetVideoEncoderConfiguration->Configuration->Resolution->Height, goto end);

            CHECK_BOOL(cJSON_AddItemToObject(new_encoder_json, "resolution", new_sub_json), goto end);
            new_sub_json = NULL;
        }

        if (trt__SetVideoEncoderConfiguration->Configuration->RateControl != NULL) {
            new_sub_json = cJSON_CreateObject();
            CHECK_POINTER(new_sub_json, goto end);
            CJSON_SET_NUMBER(new_sub_json, "frame_rate_limit", trt__SetVideoEncoderConfiguration->Configuration->RateControl->FrameRateLimit, goto end);
            CJSON_SET_NUMBER(new_sub_json, "encoding_interval", trt__SetVideoEncoderConfiguration->Configuration->RateControl->EncodingInterval, goto end);
            CJSON_SET_NUMBER(new_sub_json, "bitrate_limit", trt__SetVideoEncoderConfiguration->Configuration->RateControl->BitrateLimit, goto end);
            
            CHECK_BOOL(cJSON_AddItemToObject(new_encoder_json, "rate_control", new_sub_json), goto end);
            new_sub_json = NULL;
        }

        if (trt__SetVideoEncoderConfiguration->Configuration->H264 != NULL) {
            new_sub_json = cJSON_CreateObject();
            CHECK_POINTER(new_sub_json, goto end);
            CJSON_SET_NUMBER(new_sub_json, "gov_length", trt__SetVideoEncoderConfiguration->Configuration->H264->GovLength, goto end);
            CJSON_SET_NUMBER(new_sub_json, "h264_profile", trt__SetVideoEncoderConfiguration->Configuration->H264->H264Profile, goto end);
            
            CHECK_BOOL(cJSON_AddItemToObject(new_encoder_json, "h264", new_sub_json), goto end);
            new_sub_json = NULL;
        }

        if (trt__SetVideoEncoderConfiguration->Configuration->MPEG4 != NULL) {
            new_sub_json = cJSON_CreateObject();
            CHECK_POINTER(new_sub_json, goto end);
            CJSON_SET_NUMBER(new_sub_json, "gov_length", trt__SetVideoEncoderConfiguration->Configuration->MPEG4->GovLength, goto end);
            CJSON_SET_NUMBER(new_sub_json, "h264_profile", trt__SetVideoEncoderConfiguration->Configuration->MPEG4->Mpeg4Profile, goto end);
            
            CHECK_BOOL(cJSON_AddItemToObject(new_encoder_json, "mpeg4_profile", new_sub_json), goto end);
            new_sub_json = NULL;
        }

        CHECK_BOOL(cJSON_ReplaceItemInArray(encoder_arr_json, index, new_encoder_json), goto end);
        break;
end:
        if (new_sub_json != NULL) {
            cJSON_free(new_sub_json);
        }

        if (new_encoder_json != NULL) {
            cJSON_free(new_encoder_json);
        }
        ret = 500;
        break;
    }

    // todo save config to file

    return ret;
}
/** Web service operation '__trt__SetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioSourceConfiguration(struct soap* soap, struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__SetAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioEncoderConfiguration(struct soap* soap, struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__SetVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoAnalyticsConfiguration(struct soap* soap, struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__SetMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetMetadataConfiguration(struct soap* soap, struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__SetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioOutputConfiguration(struct soap* soap, struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__SetAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioDecoderConfiguration(struct soap* soap, struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetVideoSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurationOptions(struct soap* soap, struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetVideoEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurationOptions(struct soap* soap, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);
    
    // todo token查找校验
    LOG_INFO("conf token:%s profile token:%s", trt__GetVideoEncoderConfigurationOptions->ConfigurationToken, trt__GetVideoEncoderConfigurationOptions->ProfileToken);

    struct tt__VideoEncoderConfigurationOptions* Options = (struct tt__VideoEncoderConfigurationOptions*)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfigurationOptions));
    memset(Options, 0, sizeof(struct tt__VideoEncoderConfigurationOptions));
    SOAP_SET_RANGE(soap, Options->QualityRange, sizeof(struct tt__IntRange), 0, 100);
    Options->H264 = (struct tt__H264Options*)soap_malloc(soap, sizeof(struct tt__H264Options));
    Options->H264->__sizeResolutionsAvailable = 1;
    Options->H264->ResolutionsAvailable = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution)*Options->H264->__sizeResolutionsAvailable);
    Options->H264->ResolutionsAvailable[0].Width = 640;
    Options->H264->ResolutionsAvailable[0].Height = 512;
    SOAP_SET_RANGE(soap, Options->H264->GovLengthRange, sizeof(struct tt__IntRange), 1, 60);
    SOAP_SET_RANGE(soap, Options->H264->FrameRateRange, sizeof(struct tt__IntRange), 15, 30);
    SOAP_SET_RANGE(soap, Options->H264->EncodingIntervalRange, sizeof(struct tt__IntRange), 15, 30);

    Options->H264->__sizeH264ProfilesSupported = 4;
    int h264_profile[] = {tt__H264Profile__Baseline, tt__H264Profile__Main, tt__H264Profile__Extended, tt__H264Profile__High};
    SOAP_SET_NUMBER_LIST(soap, Options->H264->H264ProfilesSupported, sizeof(enum tt__H264Profile), Options->H264->__sizeH264ProfilesSupported, h264_profile);
    SOAP_SET_NUMBER(soap, Options->GuaranteedFrameRateSupported, sizeof(enum xsd__boolean), xsd__boolean__false_);

    trt__GetVideoEncoderConfigurationOptionsResponse->Options = Options;
    return 0;
}
/** Web service operation '__trt__GetAudioSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurationOptions(struct soap* soap, struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetAudioEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurationOptions(struct soap* soap, struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetMetadataConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurationOptions(struct soap* soap, struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetAudioOutputConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurationOptions(struct soap* soap, struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetAudioDecoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurationOptions(struct soap* soap, struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetGuaranteedNumberOfVideoEncoderInstances' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap* soap, struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetStreamUri' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetStreamUri(struct soap* soap, struct _trt__GetStreamUri *trt__GetStreamUri, struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    for(int i = 0; i < cJSON_GetArraySize(profiles_json); i++) {
        cJSON* profile_json = cJSON_GetArrayItem(profiles_json, i);
        if (profile_json == NULL || !cJSON_IsObject(profile_json)) {
            continue;
        }

        if (strcmp(trt__GetStreamUri->ProfileToken, cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(profile_json, "token"))) == 0) {

            trt__GetStreamUriResponse->MediaUri = (struct tt__MediaUri*)soap_malloc(soap, sizeof(struct tt__MediaUri));
            memset(trt__GetStreamUriResponse->MediaUri, 0, sizeof(struct tt__MediaUri));
            SOAP_CJSON_GET_STRING(profile_json, soap, "rtsp_url", trt__GetStreamUriResponse->MediaUri->Uri);
            trt__GetStreamUriResponse->MediaUri->InvalidAfterConnect = xsd__boolean__true_;
            trt__GetStreamUriResponse->MediaUri->InvalidAfterReboot  = xsd__boolean__true_;
            trt__GetStreamUriResponse->MediaUri->Timeout = 0;

            break;
        }
    }
    return 0;
}
/** Web service operation '__trt__StartMulticastStreaming' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__StartMulticastStreaming(struct soap* soap, struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__StopMulticastStreaming' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__StopMulticastStreaming(struct soap* soap, struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__SetSynchronizationPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetSynchronizationPoint(struct soap* soap, struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetSnapshotUri' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetSnapshotUri(struct soap* soap, struct _trt__GetSnapshotUri *trt__GetSnapshotUri, struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    for(int i = 0; i < cJSON_GetArraySize(profiles_json); i++) {
        cJSON* profile_json = cJSON_GetArrayItem(profiles_json, i);
        if (profile_json == NULL || !cJSON_IsObject(profile_json)) {
            continue;
        }

        if (strcmp(trt__GetSnapshotUri->ProfileToken, cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(profile_json, "token"))) == 0) {

            trt__GetSnapshotUriResponse->MediaUri = (struct tt__MediaUri*)soap_malloc(soap, sizeof(struct tt__MediaUri));
            memset(trt__GetSnapshotUriResponse->MediaUri, 0, sizeof(struct tt__MediaUri));
            SOAP_CJSON_GET_STRING(profile_json, soap, "snap_url", trt__GetSnapshotUriResponse->MediaUri->Uri);
            trt__GetSnapshotUriResponse->MediaUri->InvalidAfterConnect = xsd__boolean__false_;
            trt__GetSnapshotUriResponse->MediaUri->InvalidAfterReboot = xsd__boolean__false_;
            trt__GetSnapshotUriResponse->MediaUri->Timeout = 0;

            break;
        }
    }
    return 0;
}
/** Web service operation '__trt__GetVideoSourceModes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceModes(struct soap* soap, struct _trt__GetVideoSourceModes *trt__GetVideoSourceModes, struct _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__SetVideoSourceMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceMode(struct soap* soap, struct _trt__SetVideoSourceMode *trt__SetVideoSourceMode, struct _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetOSDs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSDs(struct soap* soap, struct _trt__GetOSDs *trt__GetOSDs, struct _trt__GetOSDsResponse *trt__GetOSDsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSD(struct soap* soap, struct _trt__GetOSD *trt__GetOSD, struct _trt__GetOSDResponse *trt__GetOSDResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetOSDOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSDOptions(struct soap* soap, struct _trt__GetOSDOptions *trt__GetOSDOptions, struct _trt__GetOSDOptionsResponse *trt__GetOSDOptionsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__SetOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetOSD(struct soap* soap, struct _trt__SetOSD *trt__SetOSD, struct _trt__SetOSDResponse *trt__SetOSDResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__CreateOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateOSD(struct soap* soap, struct _trt__CreateOSD *trt__CreateOSD, struct _trt__CreateOSDResponse *trt__CreateOSDResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__DeleteOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteOSD(struct soap* soap, struct _trt__DeleteOSD *trt__DeleteOSD, struct _trt__DeleteOSDResponse *trt__DeleteOSDResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}