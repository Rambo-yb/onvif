#include "soapStub.h"
#include "config.h"
#include "check_common.h"
#include "soap_common.h"
#include "auth.h"
/** Web service operation '__trt__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetServiceCapabilities(struct soap* soap, struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities, struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetVideoSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSources(struct soap* soap, struct _trt__GetVideoSources *trt__GetVideoSources, struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse) {
    CHECK_LT(AuthUser(soap), 0, 401);

    OnvifConfigVideosInfo* videos_info = OnvifConfigGet("videos_info");
    CHECK_POINTER(videos_info, 404);
    
    // trt__GetVideoSourcesResponse->__sizeVideoSources = videos_info->num;
    // trt__GetVideoSourcesResponse->VideoSources = (struct tt__VideoSource*)soap_malloc(soap, sizeof(struct tt__VideoSource)*trt__GetVideoSourcesResponse->__sizeVideoSources);
    // memset(trt__GetVideoSourcesResponse->VideoSources, 0, sizeof(struct tt__VideoSource)*trt__GetVideoSourcesResponse->__sizeVideoSources);
    // for(int i = 0; i < trt__GetVideoSourcesResponse->__sizeVideoSources; i++) {
    //     trt__GetVideoSourcesResponse->VideoSources[i].token = soap_strdup(soap, videos_info->video_info[i].token);
    //     trt__GetVideoSourcesResponse->VideoSources[i].Framerate = videos_info->video_info[i].frame_rate;
    //     trt__GetVideoSourcesResponse->VideoSources[i].Resolution = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution));
    //     trt__GetVideoSourcesResponse->VideoSources[i].Resolution->Width = videos_info->video_info[i].width;
    //     trt__GetVideoSourcesResponse->VideoSources[i].Resolution->Height = videos_info->video_info[i].height;
    // }

    trt__GetVideoSourcesResponse->__sizeVideoSources = 1;
    trt__GetVideoSourcesResponse->VideoSources = (struct tt__VideoSource*)soap_malloc(soap, sizeof(struct tt__VideoSource)*trt__GetVideoSourcesResponse->__sizeVideoSources);
    memset(trt__GetVideoSourcesResponse->VideoSources, 0, sizeof(struct tt__VideoSource)*trt__GetVideoSourcesResponse->__sizeVideoSources);
    for(int i = 0; i < trt__GetVideoSourcesResponse->__sizeVideoSources; i++) {
        trt__GetVideoSourcesResponse->VideoSources[i].token = soap_strdup(soap, "video_sources_0_t");
        trt__GetVideoSourcesResponse->VideoSources[i].Framerate = 25;
        trt__GetVideoSourcesResponse->VideoSources[i].Resolution = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution));
        trt__GetVideoSourcesResponse->VideoSources[i].Resolution->Width = 640;
        trt__GetVideoSourcesResponse->VideoSources[i].Resolution->Height = 512;
    }

    return 0;
}
/** Web service operation '__trt__GetAudioSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSources(struct soap* soap, struct _trt__GetAudioSources *trt__GetAudioSources, struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse) {
    CHECK_LT(AuthUser(soap), 0, 401);

    trt__GetAudioSourcesResponse->__sizeAudioSources = 0;
    trt__GetAudioSourcesResponse->AudioSources = NULL;
    // trt__GetAudioSourcesResponse->AudioSources = (struct tt__AudioSource*)soap_malloc(soap, sizeof(struct tt__AudioSource)*trt__GetAudioSourcesResponse->__sizeAudioSources);
    // for(int i = 0; i < trt__GetAudioSourcesResponse->__sizeAudioSources; i++) {
    //     trt__GetAudioSourcesResponse->AudioSources[i].token = soap_strdup(soap, "audio_source_0_t");
    //     trt__GetAudioSourcesResponse->AudioSources[i].Channels = 1;
    // }

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

static void GetProfile(struct soap* soap, struct tt__Profile* profile) {
    profile->Name = soap_strdup(soap, "media_profiles_0_n");
    profile->token = soap_strdup(soap, "media_profiles_0_t");
    SOAP_SET_NUMBER(soap, profile->fixed, sizeof(enum xsd__boolean), xsd__boolean__true_);

    profile->VideoSourceConfiguration = (struct tt__VideoSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration));
    memset(profile->VideoSourceConfiguration, 0, sizeof(struct tt__VideoSourceConfiguration));
    profile->VideoSourceConfiguration->Name = soap_strdup(soap, "video_source_0_n");
    profile->VideoSourceConfiguration->UseCount = 1;
    profile->VideoSourceConfiguration->token = soap_strdup(soap, "video_source_0_t");
    profile->VideoSourceConfiguration->SourceToken = soap_strdup(soap, "video_sources_0_t");
    profile->VideoSourceConfiguration->Bounds = (struct tt__IntRectangle*)soap_malloc(soap, sizeof(struct tt__IntRectangle));
    profile->VideoSourceConfiguration->Bounds->x = 0;
    profile->VideoSourceConfiguration->Bounds->y = 0;
    profile->VideoSourceConfiguration->Bounds->width = 640;
    profile->VideoSourceConfiguration->Bounds->height = 512;

    // profile->AudioSourceConfiguration = (struct tt__AudioSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__AudioSourceConfiguration));
    // memset(profile->AudioSourceConfiguration, 0, sizeof(struct tt__AudioSourceConfiguration));
    // profile->AudioSourceConfiguration->Name = soap_strdup(soap, "audio_source_0_n");
    // profile->AudioSourceConfiguration->UseCount = 1;
    // profile->AudioSourceConfiguration->token = soap_strdup(soap, "audio_source_0_t");
    // profile->AudioSourceConfiguration->SourceToken = soap_strdup(soap, "audio_source_0_st");

    profile->VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration));
    memset(profile->VideoEncoderConfiguration, 0, sizeof(struct tt__VideoEncoderConfiguration));
    profile->VideoEncoderConfiguration->Name = soap_strdup(soap, "video_encoder_0_n");
    profile->VideoEncoderConfiguration->UseCount = 1;
    SOAP_SET_NUMBER(soap, profile->VideoEncoderConfiguration->GuaranteedFrameRate, sizeof(enum xsd__boolean), xsd__boolean__false_);
    profile->VideoEncoderConfiguration->token = soap_strdup(soap, "video_encoder_0_t");
    profile->VideoEncoderConfiguration->Encoding = tt__VideoEncoding__H264;
    profile->VideoEncoderConfiguration->Resolution = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution));
    profile->VideoEncoderConfiguration->Resolution->Width = 640;
    profile->VideoEncoderConfiguration->Resolution->Height = 512;
    profile->VideoEncoderConfiguration->Quality = 99;
    profile->VideoEncoderConfiguration->RateControl = (struct tt__VideoRateControl*)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
    profile->VideoEncoderConfiguration->RateControl->FrameRateLimit = 25;
    profile->VideoEncoderConfiguration->RateControl->EncodingInterval = 25;
    profile->VideoEncoderConfiguration->RateControl->BitrateLimit = 2048;
    profile->VideoEncoderConfiguration->H264 = (struct tt__H264Configuration*)soap_malloc(soap, sizeof(struct tt__H264Configuration));
    profile->VideoEncoderConfiguration->H264->GovLength = 25;
    profile->VideoEncoderConfiguration->H264->H264Profile = tt__H264Profile__High;
    profile->VideoEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration*)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
    profile->VideoEncoderConfiguration->Multicast->Address = (struct tt__IPAddress*)soap_malloc(soap, sizeof(struct tt__IPAddress));
    memset(profile->VideoEncoderConfiguration->Multicast->Address, 0, sizeof(struct tt__IPAddress));
    profile->VideoEncoderConfiguration->Multicast->Address->Type = tt__IPType__IPv4;
    profile->VideoEncoderConfiguration->Multicast->Address->IPv4Address = soap_strdup(soap, "224.1.0.0");
    profile->VideoEncoderConfiguration->Multicast->Port = 40000;
    profile->VideoEncoderConfiguration->Multicast->TTL = 64;
    profile->VideoEncoderConfiguration->Multicast->AutoStart = xsd__boolean__true_;
    profile->VideoEncoderConfiguration->SessionTimeout = 60*1000;

    // profile->AudioEncoderConfiguration = (struct tt__AudioEncoderConfiguration*)soap_malloc(soap, sizeof(struct tt__AudioEncoderConfiguration));
    // memset(profile->AudioEncoderConfiguration, 0, sizeof(struct tt__AudioEncoderConfiguration));
    // profile->AudioEncoderConfiguration->Name = soap_strdup(soap, "audio_encoder_0_n");
    // profile->AudioEncoderConfiguration->UseCount = 1;
    // profile->AudioEncoderConfiguration->token = soap_strdup(soap, "audio_encoder_0_t");
    // profile->AudioEncoderConfiguration->Encoding = tt__AudioEncoding__AAC;
    // profile->AudioEncoderConfiguration->Bitrate = 16000;
    // profile->AudioEncoderConfiguration->SampleRate = 44100;
    // profile->AudioEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration*)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
    // profile->AudioEncoderConfiguration->Multicast->Address = (struct tt__IPAddress*)soap_malloc(soap, sizeof(struct tt__IPAddress));
    // memset(profile->AudioEncoderConfiguration->Multicast->Address, 0, sizeof(struct tt__IPAddress));
    // profile->AudioEncoderConfiguration->Multicast->Address->Type = tt__IPType__IPv4;
    // profile->AudioEncoderConfiguration->Multicast->Address->IPv4Address = soap_strdup(soap, "224.1.0.0");
    // profile->AudioEncoderConfiguration->Multicast->Port = 40000;
    // profile->AudioEncoderConfiguration->Multicast->TTL = 64;
    // profile->AudioEncoderConfiguration->Multicast->AutoStart = xsd__boolean__true_;
    // profile->AudioEncoderConfiguration->SessionTimeout = 60*1000;

    profile->PTZConfiguration = (struct tt__PTZConfiguration*)soap_malloc(soap, sizeof(struct tt__PTZConfiguration));
    memset(profile->PTZConfiguration, 0, sizeof(struct tt__PTZConfiguration));
    profile->PTZConfiguration->Name = soap_strdup(soap, "ptz_0_n");
    profile->PTZConfiguration->UseCount = 1;
    profile->PTZConfiguration->token = soap_strdup(soap, "ptz_0_t");
    profile->PTZConfiguration->NodeToken = soap_strdup(soap, "ptz_0_nt");
    profile->PTZConfiguration->DefaultAbsolutePantTiltPositionSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
    profile->PTZConfiguration->DefaultAbsoluteZoomPositionSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
    profile->PTZConfiguration->DefaultRelativePanTiltTranslationSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");
    profile->PTZConfiguration->DefaultRelativeZoomTranslationSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");
    profile->PTZConfiguration->DefaultContinuousPanTiltVelocitySpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace");
    profile->PTZConfiguration->DefaultContinuousZoomVelocitySpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace");
    profile->PTZConfiguration->DefaultPTZSpeed = (struct tt__PTZSpeed*)soap_malloc(soap, sizeof(struct tt__PTZSpeed));
    profile->PTZConfiguration->DefaultPTZSpeed->PanTilt = (struct tt__Vector2D*)soap_malloc(soap, sizeof(struct tt__Vector2D));
    profile->PTZConfiguration->DefaultPTZSpeed->PanTilt->x = 1;
    profile->PTZConfiguration->DefaultPTZSpeed->PanTilt->y = 0.5;
    profile->PTZConfiguration->DefaultPTZSpeed->PanTilt->space = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");
    profile->PTZConfiguration->DefaultPTZSpeed->Zoom = (struct tt__Vector1D*)soap_malloc(soap, sizeof(struct tt__Vector1D));
    profile->PTZConfiguration->DefaultPTZSpeed->Zoom->x = 0;
    profile->PTZConfiguration->DefaultPTZSpeed->Zoom->space = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");
    SOAP_SET_NUMBER(soap, profile->PTZConfiguration->DefaultPTZTimeout, sizeof(LONG64), 60*1000);
    profile->PTZConfiguration->PanTiltLimits = (struct tt__PanTiltLimits*)soap_malloc(soap, sizeof(struct tt__PanTiltLimits));
    profile->PTZConfiguration->PanTiltLimits->Range = (struct tt__Space2DDescription*)soap_malloc(soap, sizeof(struct tt__Space2DDescription));
    profile->PTZConfiguration->PanTiltLimits->Range->URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
    profile->PTZConfiguration->PanTiltLimits->Range->XRange = (struct tt__FloatRange*)soap_malloc(soap, sizeof(struct tt__FloatRange));
    profile->PTZConfiguration->PanTiltLimits->Range->XRange->Min = -1;
    profile->PTZConfiguration->PanTiltLimits->Range->XRange->Max = 1;
    profile->PTZConfiguration->PanTiltLimits->Range->YRange = (struct tt__FloatRange*)soap_malloc(soap, sizeof(struct tt__FloatRange));
    profile->PTZConfiguration->PanTiltLimits->Range->YRange->Min = -1;
    profile->PTZConfiguration->PanTiltLimits->Range->YRange->Max = 1;
    profile->PTZConfiguration->ZoomLimits = (struct tt__ZoomLimits*)soap_malloc(soap, sizeof(struct tt__ZoomLimits));
    profile->PTZConfiguration->ZoomLimits->Range = (struct tt__Space1DDescription*)soap_malloc(soap, sizeof(struct tt__Space1DDescription));
    profile->PTZConfiguration->ZoomLimits->Range->URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");
    profile->PTZConfiguration->ZoomLimits->Range->XRange = (struct tt__FloatRange*)soap_malloc(soap, sizeof(struct tt__FloatRange));
    profile->PTZConfiguration->ZoomLimits->Range->XRange->Min = 0;
    profile->PTZConfiguration->ZoomLimits->Range->XRange->Max = 1;

    // profile->MetadataConfiguration = (struct tt__MetadataConfiguration*)soap_malloc(soap, sizeof(struct tt__MetadataConfiguration));
    // memset(profile->MetadataConfiguration, 0, sizeof(struct tt__MetadataConfiguration));
    // profile->MetadataConfiguration->Name = soap_strdup(soap, "metadata_0_n");
    // profile->MetadataConfiguration->UseCount = 1;
    // profile->MetadataConfiguration->token = soap_strdup(soap, "metadata_0_t");
    // SOAP_SET_NUMBER(soap, profile->MetadataConfiguration->Analytics, sizeof(enum xsd__boolean), xsd__boolean__false_);
    // profile->MetadataConfiguration->SessionTimeout = 60*1000;

}
/** Web service operation '__trt__GetProfile' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfile(struct soap* soap, struct _trt__GetProfile *trt__GetProfile, struct _trt__GetProfileResponse *trt__GetProfileResponse) {
    CHECK_LT(AuthUser(soap), 0, 401);

    // todo token查找校验
    LOG_INFO("%s", trt__GetProfile->ProfileToken);

    trt__GetProfileResponse->Profile = (struct tt__Profile*)soap_malloc(soap, sizeof(struct tt__Profile));
    memset(trt__GetProfileResponse->Profile, 0, sizeof(struct tt__Profile));
    GetProfile(soap, trt__GetProfileResponse->Profile);
    return 0;
}
/** Web service operation '__trt__GetProfiles' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfiles(struct soap* soap, struct _trt__GetProfiles *trt__GetProfiles, struct _trt__GetProfilesResponse *trt__GetProfilesResponse) {
    CHECK_LT(AuthUser(soap), 0, 401);

    trt__GetProfilesResponse->__sizeProfiles = 1;
    trt__GetProfilesResponse->Profiles = (struct tt__Profile*)soap_malloc(soap, sizeof(struct tt__Profile)*trt__GetProfilesResponse->__sizeProfiles);
    memset(trt__GetProfilesResponse->Profiles, 0, sizeof(struct tt__Profile)*trt__GetProfilesResponse->__sizeProfiles);
    for(int i = 0; i < trt__GetProfilesResponse->__sizeProfiles; i++) {
        GetProfile(soap, &trt__GetProfilesResponse->Profiles[i]);
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
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetVideoEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurations(struct soap* soap, struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__trt__GetAudioSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurations(struct soap* soap, struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
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
    CHECK_LT(AuthUser(soap), 0, 401);
    
    // todo token查找校验
    LOG_INFO("%s", trt__GetVideoSourceConfiguration->ConfigurationToken);

    trt__GetVideoSourceConfigurationResponse->Configuration = (struct tt__VideoSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration));
    memset(trt__GetVideoSourceConfigurationResponse->Configuration, 0, sizeof(struct tt__VideoSourceConfiguration));
    trt__GetVideoSourceConfigurationResponse->Configuration->Name = soap_strdup(soap, "video_source_0_n");
    trt__GetVideoSourceConfigurationResponse->Configuration->UseCount = 1;
    trt__GetVideoSourceConfigurationResponse->Configuration->token = soap_strdup(soap, "video_source_0_t");
    trt__GetVideoSourceConfigurationResponse->Configuration->SourceToken = soap_strdup(soap, "video_sources_0_t");
    trt__GetVideoSourceConfigurationResponse->Configuration->Bounds = (struct tt__IntRectangle*)soap_malloc(soap, sizeof(struct tt__IntRectangle));
    trt__GetVideoSourceConfigurationResponse->Configuration->Bounds->x = 0;
    trt__GetVideoSourceConfigurationResponse->Configuration->Bounds->y = 0;
    trt__GetVideoSourceConfigurationResponse->Configuration->Bounds->width = 640;
    trt__GetVideoSourceConfigurationResponse->Configuration->Bounds->height = 512;
    trt__GetVideoSourceConfigurationResponse->Configuration->ViewMode = soap_strdup(soap, "");

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
    printf("%s:%d\n", __func__, __LINE__);
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
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
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
    CHECK_LT(AuthUser(soap), 0, 401);
    
    // todo token查找校验
    LOG_INFO("conf token:%s profile token:%s", trt__GetVideoEncoderConfigurationOptions->ConfigurationToken, trt__GetVideoEncoderConfigurationOptions->ProfileToken);

    struct tt__VideoEncoderConfigurationOptions* Options = (struct tt__VideoEncoderConfigurationOptions*)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfigurationOptions));
    memset(Options, 0, sizeof(struct tt__VideoEncoderConfigurationOptions));
    Options->QualityRange = (struct tt__IntRange*)soap_malloc(soap, sizeof(struct tt__IntRange));
    Options->QualityRange->Min = 0;
    Options->QualityRange->Max = 1;
    Options->H264 = (struct tt__H264Options*)soap_malloc(soap, sizeof(struct tt__H264Options));
    Options->H264->__sizeResolutionsAvailable = 1;
    Options->H264->ResolutionsAvailable = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution)*Options->H264->__sizeResolutionsAvailable);
    Options->H264->ResolutionsAvailable[0].Width = 640;
    Options->H264->ResolutionsAvailable[0].Height = 512;
    Options->H264->GovLengthRange = (struct tt__IntRange*)soap_malloc(soap, sizeof(struct tt__IntRange));
    Options->H264->GovLengthRange->Min = 15;
    Options->H264->GovLengthRange->Max = 30;
    Options->H264->FrameRateRange = (struct tt__IntRange*)soap_malloc(soap, sizeof(struct tt__IntRange));
    Options->H264->FrameRateRange->Min = 15;
    Options->H264->FrameRateRange->Max = 30;
    Options->H264->EncodingIntervalRange = (struct tt__IntRange*)soap_malloc(soap, sizeof(struct tt__IntRange));
    Options->H264->EncodingIntervalRange->Min = 15;
    Options->H264->EncodingIntervalRange->Max = 30;

    Options->H264->__sizeH264ProfilesSupported = 1;
    Options->H264->H264ProfilesSupported = (enum tt__H264Profile*)soap_malloc(soap, sizeof(enum tt__H264Profile)*Options->H264->__sizeH264ProfilesSupported);
    Options->H264->H264ProfilesSupported[0] = tt__H264Profile__High;
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
    CHECK_LT(AuthUser(soap), 0, 401);
    
    // todo token查找校验
    LOG_INFO("%s", trt__GetStreamUri->ProfileToken);
    
    trt__GetStreamUriResponse->MediaUri = (struct tt__MediaUri*)soap_malloc(soap, sizeof(struct tt__MediaUri));
    memset(trt__GetStreamUriResponse->MediaUri, 0, sizeof(struct tt__MediaUri));
    trt__GetStreamUriResponse->MediaUri->Uri = soap_strdup(soap, "rtsp://192.168.110.223/live/main_stream");
    trt__GetStreamUriResponse->MediaUri->InvalidAfterConnect = xsd__boolean__true_;
    trt__GetStreamUriResponse->MediaUri->InvalidAfterReboot  = xsd__boolean__true_;
    trt__GetStreamUriResponse->MediaUri->Timeout = 0;

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
    CHECK_LT(AuthUser(soap), 0, 401);

    // todo token查找校验
    LOG_INFO("%s", trt__GetSnapshotUri->ProfileToken);

    trt__GetSnapshotUriResponse->MediaUri = (struct tt__MediaUri*)soap_malloc(soap, sizeof(struct tt__MediaUri));
    trt__GetSnapshotUriResponse->MediaUri->Uri = soap_strdup(soap, "http://192.168.110.223:8080/dev_api/get_snap");
    trt__GetSnapshotUriResponse->MediaUri->InvalidAfterConnect = xsd__boolean__false_;
    trt__GetSnapshotUriResponse->MediaUri->InvalidAfterReboot = xsd__boolean__false_;
    trt__GetSnapshotUriResponse->MediaUri->Timeout = 0;

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