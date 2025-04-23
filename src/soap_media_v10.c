#include "soapStub.h"
#include "onvif_operation.h"
#include "onvif_conf.h"
#include "check_common.h"
#include "soap_common.h"
#include "onvif_auth.h"
#include "cJSON.h"
#include "cjson_common.h"

static void GetVideoSourceConfiguration(struct soap* soap, int cam, int stream, OnvifConfigCameraVideoEncodeInfo encode_info, struct tt__VideoSourceConfiguration* conf_video_source) {
	char token[64] = {0};
	snprintf(token, sizeof(token), "ONFVideoSourceToken_%03d", cam*100+stream+1);
	conf_video_source->token = soap_strdup(soap, token);

	char name[64] = {0};
	snprintf(name, sizeof(name), "ONFVideoSourceConfig_%03d", cam*100+stream+1);
	conf_video_source->Name = soap_strdup(soap, name);

	conf_video_source->UseCount = 3;

	char source_token[64] = {0};
	snprintf(source_token, sizeof(source_token), "ONFVideoSource_%03d", cam*100+stream+1);
	conf_video_source->SourceToken = soap_strdup(soap, source_token);

	conf_video_source->Bounds = (struct tt__IntRectangle*)soap_malloc(soap, sizeof(struct tt__IntRectangle));
	conf_video_source->Bounds->x = 0;
	conf_video_source->Bounds->y = 0;
	conf_video_source->Bounds->width = encode_info.resolution.width;
	conf_video_source->Bounds->height = encode_info.resolution.height;

	conf_video_source->Extension = (struct tt__VideoSourceConfigurationExtension*)soap_malloc(soap, sizeof(struct tt__VideoSourceConfigurationExtension));
	memset(conf_video_source->Extension, 0, sizeof(struct tt__VideoSourceConfigurationExtension));

	conf_video_source->Extension->Rotate = (struct tt__Rotate*)soap_malloc(soap, sizeof(struct tt__Rotate));
	memset(conf_video_source->Extension->Rotate, 0, sizeof(struct tt__Rotate));
	conf_video_source->Extension->Rotate->Mode = tt__RotateMode__OFF;
}

static void GetAudioSourceConfiguration(struct soap* soap, struct tt__AudioSourceConfiguration* conf_audio_source) {
	conf_audio_source->token = soap_strdup(soap, "AudioSourceConfigToken_1");
	conf_audio_source->Name = soap_strdup(soap, "AudioSourceConfig_1");
	conf_audio_source->UseCount = 3;
	conf_audio_source->SourceToken = soap_strdup(soap, "AudioSourceChannel_1");
}

static void GetVideoEncoderConfiguration(struct soap* soap, int cam, int stream, OnvifConfigCameraVideoEncodeInfo encode_info, struct tt__VideoEncoderConfiguration* conf_video_encoder) {
	char token[64] = {0};
	snprintf(token, sizeof(token), "ONFVideoEncoderToken_%03d", cam*100+stream+1);
	conf_video_encoder->token = soap_strdup(soap, token);

	char name[64] = {0};
	snprintf(name, sizeof(name), "ONFVideoEncoder_%03d", cam*100+stream+1);
	conf_video_encoder->Name = soap_strdup(soap, name);

	conf_video_encoder->UseCount = 1;

	conf_video_encoder->GuaranteedFrameRate = soap_malloc(soap, sizeof(enum xsd__boolean));
	*(conf_video_encoder->GuaranteedFrameRate) = xsd__boolean__false_;

	conf_video_encoder->Encoding = tt__VideoEncoding__H264;

	conf_video_encoder->Resolution = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution));
	memset(conf_video_encoder->Resolution, 0, sizeof(struct tt__VideoResolution));
	conf_video_encoder->Resolution->Width = encode_info.resolution.width;
	conf_video_encoder->Resolution->Height = encode_info.resolution.height;

	conf_video_encoder->Quality = encode_info.quality;

	conf_video_encoder->RateControl = (struct tt__VideoRateControl*)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
	memset(conf_video_encoder->RateControl, 0, sizeof(struct tt__VideoRateControl));
	conf_video_encoder->RateControl->FrameRateLimit = encode_info.fps;
	conf_video_encoder->RateControl->EncodingInterval = 1;
	conf_video_encoder->RateControl->BitrateLimit = encode_info.bitrate_max;

	conf_video_encoder->H264 = (struct tt__H264Configuration*)soap_malloc(soap, sizeof(struct tt__H264Configuration));
	memset(conf_video_encoder->H264, 0, sizeof(struct tt__H264Configuration));
	conf_video_encoder->H264->GovLength = encode_info.gop;
	conf_video_encoder->H264->H264Profile = tt__H264Profile__Main;

	conf_video_encoder->SessionTimeout = 5;
}

static void GetAudioEncoderConfiguration(struct soap* soap, OnvifConfigCameraAudioEncodeInfo audio_info, struct tt__AudioEncoderConfiguration* conf_audio_encoder) {
	conf_audio_encoder->token = soap_strdup(soap, "MainAudioEncoderToken_1");
	conf_audio_encoder->Name = soap_strdup(soap, "AudioEncoderConfig_1");
	conf_audio_encoder->UseCount = 1;
	conf_audio_encoder->Encoding = audio_info.encode_type;
	conf_audio_encoder->Bitrate = audio_info.bitrate;
	conf_audio_encoder->SampleRate = audio_info.sample_rate;
	conf_audio_encoder->SessionTimeout = 5000;

}

static void GetPtzConfiguration(struct soap* soap, struct tt__PTZConfiguration* conf_ptz) {
	conf_ptz->Name = soap_strdup(soap, "PTZ");
	conf_ptz->token = soap_strdup(soap, "PTZToken");
	conf_ptz->UseCount = 3;
	conf_ptz->NodeToken = soap_strdup(soap, "PTZNodeToken");

	conf_ptz->DefaultAbsolutePantTiltPositionSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
	conf_ptz->DefaultAbsoluteZoomPositionSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
	conf_ptz->DefaultRelativePanTiltTranslationSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");
	conf_ptz->DefaultRelativeZoomTranslationSpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");
	conf_ptz->DefaultContinuousPanTiltVelocitySpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace");
	conf_ptz->DefaultContinuousZoomVelocitySpace = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace");

	conf_ptz->DefaultPTZSpeed = (struct tt__PTZSpeed*)soap_malloc(soap, sizeof(struct tt__PTZSpeed));
	memset(conf_ptz->DefaultPTZSpeed, 0, sizeof(struct tt__PTZSpeed));
	conf_ptz->DefaultPTZSpeed->PanTilt = (struct tt__Vector2D*)soap_malloc(soap, sizeof(struct tt__Vector2D));
	memset(conf_ptz->DefaultPTZSpeed->PanTilt, 0, sizeof(struct tt__Vector2D));
	conf_ptz->DefaultPTZSpeed->PanTilt->x = 0.1;
	conf_ptz->DefaultPTZSpeed->PanTilt->y = 0.1;
	conf_ptz->DefaultPTZSpeed->PanTilt->space = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");
	conf_ptz->DefaultPTZSpeed->Zoom = (struct tt__Vector1D*)soap_malloc(soap, sizeof(struct tt__Vector1D));
	memset(conf_ptz->DefaultPTZSpeed->Zoom, 0, sizeof(struct tt__Vector1D));
	conf_ptz->DefaultPTZSpeed->Zoom->x = 1;
	conf_ptz->DefaultPTZSpeed->Zoom->space = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");
	
	conf_ptz->PanTiltLimits = (struct tt__PanTiltLimits*)soap_malloc(soap, sizeof(struct tt__PanTiltLimits));
	memset(conf_ptz->PanTiltLimits, 0, sizeof(struct tt__PanTiltLimits));
	conf_ptz->PanTiltLimits->Range = (struct tt__Space2DDescription*)soap_malloc(soap, sizeof(struct tt__Space2DDescription));
	memset(conf_ptz->PanTiltLimits->Range, 0, sizeof(struct tt__Space2DDescription));
	conf_ptz->PanTiltLimits->Range->URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
	SOAP_SET_RANGE(soap, conf_ptz->PanTiltLimits->Range->XRange, sizeof(struct tt__FloatRange), -1, 1);
	SOAP_SET_RANGE(soap, conf_ptz->PanTiltLimits->Range->YRange, sizeof(struct tt__FloatRange), -1, 1);

	conf_ptz->ZoomLimits = (struct tt__ZoomLimits*)soap_malloc(soap, sizeof(struct tt__ZoomLimits));
	memset(conf_ptz->ZoomLimits, 0, sizeof(struct tt__ZoomLimits));
	conf_ptz->ZoomLimits->Range = (struct tt__Space1DDescription*)soap_malloc(soap, sizeof(struct tt__Space1DDescription));
	memset(conf_ptz->ZoomLimits->Range, 0, sizeof(struct tt__Space1DDescription));
	conf_ptz->ZoomLimits->Range->URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
	SOAP_SET_RANGE(soap, conf_ptz->PanTiltLimits->Range->XRange, sizeof(struct tt__FloatRange), 0, 1);
}

void GetMediaV10ServiceCapabilities(struct soap* soap, struct trt__Capabilities* capabilities) {
	capabilities->ProfileCapabilities = (struct trt__ProfileCapabilities*)soap_malloc(soap, sizeof(struct trt__ProfileCapabilities));
	memset(capabilities->ProfileCapabilities, 0, sizeof(struct trt__ProfileCapabilities));
	SOAP_SET_NUMBER(soap, capabilities->ProfileCapabilities->MaximumNumberOfProfiles, sizeof(int), 10);

	capabilities->StreamingCapabilities = (struct trt__StreamingCapabilities*)soap_malloc(soap, sizeof(struct trt__StreamingCapabilities));
	memset(capabilities->StreamingCapabilities, 0, sizeof(struct trt__StreamingCapabilities));
	SOAP_SET_NUMBER(soap, capabilities->StreamingCapabilities->RTPMulticast, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->StreamingCapabilities->RTP_USCORETCP, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->StreamingCapabilities->RTP_USCORERTSP_USCORETCP, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->StreamingCapabilities->NonAggregateControl, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->StreamingCapabilities->NoRTSPStreaming, sizeof(enum xsd__boolean), xsd__boolean__false_);

	SOAP_SET_NUMBER(soap, capabilities->SnapshotUri, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->Rotation, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->OSD, sizeof(enum xsd__boolean), xsd__boolean__true_);
}

/** Web service operation '__trt__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetServiceCapabilities(struct soap* soap, struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities, struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	trt__GetServiceCapabilitiesResponse->Capabilities = (struct trt__Capabilities*)soap_malloc(soap, sizeof(struct trt__Capabilities));
	memset(trt__GetServiceCapabilitiesResponse->Capabilities, 0, sizeof(struct trt__Capabilities));
	GetMediaV10ServiceCapabilities(soap, trt__GetServiceCapabilitiesResponse->Capabilities);

	return 0;
}
/** Web service operation '__trt__GetVideoSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSources(struct soap* soap, struct _trt__GetVideoSources *trt__GetVideoSources, struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigCameraVideoEncodeInfos encode_infos;
	memset(&encode_infos, 0, sizeof(OnvifConfigCameraVideoEncodeInfos));
	encode_infos.deal_cam = -1;
	encode_infos.deal_stream = -1;
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_INFO, &encode_infos, sizeof(OnvifConfigCameraVideoEncodeInfos));
	CHECK_LT(ret, 0, return 500);

	trt__GetVideoSourcesResponse->__sizeVideoSources = 0;
	for (int i = 0; i < encode_infos.cam_num; i++) {
		trt__GetVideoSourcesResponse->__sizeVideoSources += encode_infos.stream_num[i];
	}

	trt__GetVideoSourcesResponse->VideoSources = (struct tt__VideoSource*)soap_malloc(soap, sizeof(struct tt__VideoSource)*trt__GetVideoSourcesResponse->__sizeVideoSources);
	memset(trt__GetVideoSourcesResponse->VideoSources, 0, sizeof(struct tt__VideoSource)*trt__GetVideoSourcesResponse->__sizeVideoSources);

	int num = 0;
	for (int i = 0; i < encode_infos.cam_num; i++) {
		for (int j = 0; j < encode_infos.stream_num[i]; j++) {
			char token[64] = {0};
			snprintf(token, sizeof(token), "ONFVideoSource_%03d", i*100+j+1);
			trt__GetVideoSourcesResponse->VideoSources[num].token = soap_strdup(soap, token);
			trt__GetVideoSourcesResponse->VideoSources[num].Framerate = encode_infos.encode_info[i][j].fps;
			trt__GetVideoSourcesResponse->VideoSources[num].Resolution = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution));
			trt__GetVideoSourcesResponse->VideoSources[num].Resolution->Width = encode_infos.encode_info[i][j].resolution.width;
			trt__GetVideoSourcesResponse->VideoSources[num].Resolution->Height = encode_infos.encode_info[i][j].resolution.height;

			num++;
		}
	}

	return 0;
}
/** Web service operation '__trt__GetAudioSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSources(struct soap* soap, struct _trt__GetAudioSources *trt__GetAudioSources, struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigCameraAudioEncodeInfo audio_info;
	memset(&audio_info, 0, sizeof(OnvifConfigCameraAudioEncodeInfo));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_AUDEO_ENCODE_INFO, &audio_info, sizeof(OnvifConfigCameraAudioEncodeInfo));
	CHECK_LT(ret, 0, return 500);

	trt__GetAudioSourcesResponse->__sizeAudioSources = 1;
	trt__GetAudioSourcesResponse->AudioSources = (struct tt__AudioSource*)soap_malloc(soap, sizeof(struct tt__AudioSource)*trt__GetAudioSourcesResponse->__sizeAudioSources);
	memset(trt__GetAudioSourcesResponse->AudioSources, 0, sizeof(struct tt__AudioSource)*trt__GetAudioSourcesResponse->__sizeAudioSources);

	trt__GetAudioSourcesResponse->AudioSources[0].token = soap_strdup(soap, "AudioSourceConfig_1");
	trt__GetAudioSourcesResponse->AudioSources[0].Channels = audio_info.channel;

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
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	if (trt__GetProfile->ProfileToken) {
		sscanf(trt__GetProfile->ProfileToken, "%*[^_]_%d\n", &code);
	} else {
		return 404;
	}

	OnvifConfigCameraVideoEncodeInfos encode_infos;
	memset(&encode_infos, 0, sizeof(OnvifConfigCameraVideoEncodeInfos));
	encode_infos.deal_cam = code/100;
	encode_infos.deal_stream = code%100 - 1;
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_INFO, &encode_infos, sizeof(OnvifConfigCameraVideoEncodeInfos));
	CHECK_LT(ret, 0, return 500);

	OnvifConfigCameraAudioEncodeInfo audio_info;
	memset(&audio_info, 0, sizeof(OnvifConfigCameraAudioEncodeInfo));
	ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_AUDEO_ENCODE_INFO, &audio_info, sizeof(OnvifConfigCameraAudioEncodeInfo));
	CHECK_LT(ret, 0, return 500);


	trt__GetProfileResponse->Profile = (struct tt__Profile*)soap_malloc(soap, sizeof(struct tt__Profile));
	memset(trt__GetProfileResponse->Profile, 0, sizeof(struct tt__Profile));

	char token[64] = {0};
	snprintf(token, sizeof(token), "ONFProfileToken_%03d", code);
	trt__GetProfileResponse->Profile->token = soap_strdup(soap, token);

	char name[64] = {0};
	snprintf(name, sizeof(name), "ONFProfileName_%03d", code);
	trt__GetProfileResponse->Profile->Name = soap_strdup(soap, token);

	trt__GetProfileResponse->Profile->fixed = soap_malloc(soap, sizeof(enum xsd__boolean));
	*(trt__GetProfileResponse->Profile->fixed) = xsd__boolean__true_;

	trt__GetProfileResponse->Profile->VideoSourceConfiguration = (struct tt__VideoSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration));
	memset(trt__GetProfileResponse->Profile->VideoSourceConfiguration, 0, sizeof(struct tt__VideoSourceConfiguration));
	GetVideoSourceConfiguration(soap, code/100, code%100 - 1, encode_infos.encode_info[0][0], trt__GetProfileResponse->Profile->VideoSourceConfiguration);

	trt__GetProfileResponse->Profile->AudioSourceConfiguration = (struct tt__AudioSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__AudioSourceConfiguration));
	memset(trt__GetProfileResponse->Profile->AudioSourceConfiguration, 0, sizeof(struct tt__AudioSourceConfiguration));
	GetAudioSourceConfiguration(soap, trt__GetProfileResponse->Profile->AudioSourceConfiguration);

	trt__GetProfileResponse->Profile->VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration));
	memset(trt__GetProfileResponse->Profile->VideoEncoderConfiguration, 0, sizeof(struct tt__VideoEncoderConfiguration));
	GetVideoEncoderConfiguration(soap, code/100, code%100 - 1, encode_infos.encode_info[0][0], trt__GetProfileResponse->Profile->VideoEncoderConfiguration);

	trt__GetProfileResponse->Profile->AudioEncoderConfiguration = (struct tt__AudioEncoderConfiguration*)soap_malloc(soap, sizeof(struct tt__AudioEncoderConfiguration));
	memset(trt__GetProfileResponse->Profile->AudioEncoderConfiguration, 0, sizeof(struct tt__AudioEncoderConfiguration));
	GetAudioEncoderConfiguration(soap, audio_info, trt__GetProfileResponse->Profile->AudioEncoderConfiguration);

	trt__GetProfileResponse->Profile->PTZConfiguration = (struct tt__PTZConfiguration*)soap_malloc(soap, sizeof(struct tt__PTZConfiguration));
	memset(trt__GetProfileResponse->Profile->PTZConfiguration, 0, sizeof(struct tt__PTZConfiguration));
	GetPtzConfiguration(soap, trt__GetProfileResponse->Profile->PTZConfiguration);

	return 0;
}
/** Web service operation '__trt__GetProfiles' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfiles(struct soap* soap, struct _trt__GetProfiles *trt__GetProfiles, struct _trt__GetProfilesResponse *trt__GetProfilesResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigCameraVideoEncodeInfos encode_infos;
	memset(&encode_infos, 0, sizeof(OnvifConfigCameraVideoEncodeInfos));
	encode_infos.deal_cam = -1;
	encode_infos.deal_stream = -1;
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_INFO, &encode_infos, sizeof(OnvifConfigCameraVideoEncodeInfos));
	CHECK_LT(ret, 0, return 500);

	OnvifConfigCameraAudioEncodeInfo audio_info;
	memset(&audio_info, 0, sizeof(OnvifConfigCameraAudioEncodeInfo));
	ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_AUDEO_ENCODE_INFO, &audio_info, sizeof(OnvifConfigCameraAudioEncodeInfo));
	CHECK_LT(ret, 0, return 500);

	trt__GetProfilesResponse->__sizeProfiles = 0;
	for (int i = 0; i < encode_infos.cam_num; i++) {
		trt__GetProfilesResponse->__sizeProfiles += encode_infos.stream_num[i];
	}

	trt__GetProfilesResponse->Profiles = (struct tt__Profile*)soap_malloc(soap, sizeof(struct tt__Profile)*trt__GetProfilesResponse->__sizeProfiles);
	memset(trt__GetProfilesResponse->Profiles, 0, sizeof(struct tt__Profile)*trt__GetProfilesResponse->__sizeProfiles);

	int num = 0;
	for (int i = 0; i < encode_infos.cam_num; i++) {
		for (int j = 0; j < encode_infos.stream_num[i]; j++) {
			char token[64] = {0};
			snprintf(token, sizeof(token), "ONFProfileToken_%03d", i * 100 + j + 1);
			trt__GetProfilesResponse->Profiles[num].token = soap_strdup(soap, token);
		
			char name[64] = {0};
			snprintf(name, sizeof(name), "ONFProfileName_%03d", i * 100 + j + 1);
			trt__GetProfilesResponse->Profiles[num].Name = soap_strdup(soap, token);
		
			trt__GetProfilesResponse->Profiles[num].fixed = soap_malloc(soap, sizeof(enum xsd__boolean));
			*(trt__GetProfilesResponse->Profiles[num].fixed) = xsd__boolean__true_;

			trt__GetProfilesResponse->Profiles[num].VideoSourceConfiguration = (struct tt__VideoSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration));
			memset(trt__GetProfilesResponse->Profiles[num].VideoSourceConfiguration, 0, sizeof(struct tt__VideoSourceConfiguration));
			GetVideoSourceConfiguration(soap, i, j, encode_infos.encode_info[i][j], trt__GetProfilesResponse->Profiles[num].VideoSourceConfiguration);
		
			trt__GetProfilesResponse->Profiles[num].AudioSourceConfiguration = (struct tt__AudioSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__AudioSourceConfiguration));
			memset(trt__GetProfilesResponse->Profiles[num].AudioSourceConfiguration, 0, sizeof(struct tt__AudioSourceConfiguration));
			GetAudioSourceConfiguration(soap, trt__GetProfilesResponse->Profiles[num].AudioSourceConfiguration);
		
			trt__GetProfilesResponse->Profiles[num].VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration));
			memset(trt__GetProfilesResponse->Profiles[num].VideoEncoderConfiguration, 0, sizeof(struct tt__VideoEncoderConfiguration));
			GetVideoEncoderConfiguration(soap, i, j, encode_infos.encode_info[i][j], trt__GetProfilesResponse->Profiles[num].VideoEncoderConfiguration);
		
			trt__GetProfilesResponse->Profiles[num].AudioEncoderConfiguration = (struct tt__AudioEncoderConfiguration*)soap_malloc(soap, sizeof(struct tt__AudioEncoderConfiguration));
			memset(trt__GetProfilesResponse->Profiles[num].AudioEncoderConfiguration, 0, sizeof(struct tt__AudioEncoderConfiguration));
			GetAudioEncoderConfiguration(soap, audio_info, trt__GetProfilesResponse->Profiles[num].AudioEncoderConfiguration);
		
			trt__GetProfilesResponse->Profiles[num].PTZConfiguration = (struct tt__PTZConfiguration*)soap_malloc(soap, sizeof(struct tt__PTZConfiguration));
			memset(trt__GetProfilesResponse->Profiles[num].PTZConfiguration, 0, sizeof(struct tt__PTZConfiguration));
			GetPtzConfiguration(soap, trt__GetProfilesResponse->Profiles[num].PTZConfiguration);
			
			num++;
		}
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
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigCameraVideoEncodeInfos encode_infos;
	memset(&encode_infos, 0, sizeof(OnvifConfigCameraVideoEncodeInfos));
	encode_infos.deal_cam = -1;
	encode_infos.deal_stream = -1;
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_INFO, &encode_infos, sizeof(OnvifConfigCameraVideoEncodeInfos));
	CHECK_LT(ret, 0, return 500);

	trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations = 0;
	for (int i = 0; i < encode_infos.cam_num; i++) {
		trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations += encode_infos.stream_num[i];
	}

	trt__GetVideoSourceConfigurationsResponse->Configurations = (struct tt__VideoSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration)*trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations);
	memset(trt__GetVideoSourceConfigurationsResponse->Configurations, 0, sizeof(struct tt__VideoSourceConfiguration)*trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations);

	int num = 0;
	for (int i = 0; i < encode_infos.cam_num; i++) {
		for (int j = 0; j < encode_infos.stream_num[i]; j++) {
			GetVideoSourceConfiguration(soap, i, j, encode_infos.encode_info[i][j], &trt__GetVideoSourceConfigurationsResponse->Configurations[num]);
			num++;
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
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations = 1;
	trt__GetAudioSourceConfigurationsResponse->Configurations = (struct tt__AudioSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__AudioSourceConfiguration)*trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations);
	memset(trt__GetAudioSourceConfigurationsResponse->Configurations, 0, sizeof(struct tt__AudioSourceConfiguration)*trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations);
	
	GetAudioSourceConfiguration(soap, &trt__GetAudioSourceConfigurationsResponse->Configurations[0]);

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
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	if (trt__GetVideoSourceConfiguration->ConfigurationToken) {
		sscanf(trt__GetVideoSourceConfiguration->ConfigurationToken, "%*[^_]_%d\n", &code);
	} else {
		return 404;
	}

	OnvifConfigCameraVideoEncodeInfos encode_infos;
	memset(&encode_infos, 0, sizeof(OnvifConfigCameraVideoEncodeInfos));
	encode_infos.deal_cam = code/100;
	encode_infos.deal_stream = (code%100) - 1;
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_INFO, &encode_infos, sizeof(OnvifConfigCameraVideoEncodeInfos));
	CHECK_LT(ret, 0, return 500);
	
	trt__GetVideoSourceConfigurationResponse->Configuration = (struct tt__VideoSourceConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration));
	memset(trt__GetVideoSourceConfigurationResponse->Configuration, 0, sizeof(struct tt__VideoSourceConfiguration));
	GetVideoSourceConfiguration(soap, code/100, code%100 - 1, encode_infos.encode_info[0][0], trt__GetVideoSourceConfigurationResponse->Configuration);

	return 0;
}
/** Web service operation '__trt__GetVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfiguration(struct soap* soap, struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	if (trt__GetVideoEncoderConfiguration->ConfigurationToken) {
		sscanf(trt__GetVideoEncoderConfiguration->ConfigurationToken, "%*[^_]_%d\n", &code);
	} else {
		return 404;
	}

	OnvifConfigCameraVideoEncodeInfos encode_infos;
	memset(&encode_infos, 0, sizeof(OnvifConfigCameraVideoEncodeInfos));
	encode_infos.deal_cam = code/100;
	encode_infos.deal_stream = code%100 - 1;
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_INFO, &encode_infos, sizeof(OnvifConfigCameraVideoEncodeInfos));
	CHECK_LT(ret, 0, return 500);

	trt__GetVideoEncoderConfigurationResponse->Configuration = (struct tt__VideoEncoderConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration));
	memset(trt__GetVideoEncoderConfigurationResponse->Configuration, 0, sizeof(struct tt__VideoEncoderConfiguration));
	GetVideoEncoderConfiguration(soap, code/100, code%100 - 1, encode_infos.encode_info[0][0], trt__GetVideoEncoderConfigurationResponse->Configuration);

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
	// CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	// cJSON* profiles_json = OnvifOperationGetConfig("profiles");
	// CHECK_POINTER(profiles_json, return 500);
	// CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

	// for(int i = 0; i < cJSON_GetArraySize(profiles_json); i++) {
	//     cJSON* profile_json = cJSON_GetArrayItem(profiles_json, i);
	//     if (profile_json == NULL || !cJSON_IsObject(profile_json)) {
	//         continue;
	//     }

	//     if (strcmp(trt__GetCompatibleVideoEncoderConfigurations->ProfileToken, cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(profile_json, "token"))) == 0) {
	//         cJSON* video_encoder_arr_json = cJSON_GetObjectItemCaseSensitive(profile_json, "compatible_video_encoder");
	//         CHECK_POINTER(video_encoder_arr_json, return 500);
	//         CHECK_BOOL(cJSON_IsArray(video_encoder_arr_json), return 500);

	//         trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations = cJSON_GetArraySize(video_encoder_arr_json);
	//         trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations = (struct tt__VideoEncoderConfiguration*)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration)*trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations);
	//         memset(trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations, 0, sizeof(struct tt__VideoEncoderConfiguration)*trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations);
	//         for(int j = 0; j < trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations; j++) {
	//             cJSON* video_encoder_json = cJSON_GetArrayItem(video_encoder_arr_json, j);
	//             if (video_encoder_json == NULL || !cJSON_IsObject(video_encoder_json)) {
	//                 continue;
	//             }

	//             GetVideoEncoderConfigurationFromJson(soap, video_encoder_json, &trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations[j]);
	//         }
	//         break;
	//     }
	// }

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
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

    CHECK_POINTER(trt__SetVideoEncoderConfiguration->Configuration, return 500);
	
	int code = 0;
	sscanf(trt__SetVideoEncoderConfiguration->Configuration->token, "%*[^_]_%d\n", &code);

	OnvifConfigCameraVideoEncodeInfos encode_infos;
	memset(&encode_infos, 0, sizeof(OnvifConfigCameraVideoEncodeInfos));
	encode_infos.deal_cam = code/100;
	encode_infos.deal_stream = code%100 - 1;
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_INFO, &encode_infos, sizeof(OnvifConfigCameraVideoEncodeInfos));
	CHECK_LT(ret, 0, return 500);

	encode_infos.encode_info[0][0].quality = trt__SetVideoEncoderConfiguration->Configuration->Quality;

	if (trt__SetVideoEncoderConfiguration->Configuration->Resolution != NULL) {
		encode_infos.encode_info[0][0].resolution.width = trt__SetVideoEncoderConfiguration->Configuration->Resolution->Width;
		encode_infos.encode_info[0][0].resolution.height = trt__SetVideoEncoderConfiguration->Configuration->Resolution->Height;
	}

	if (trt__SetVideoEncoderConfiguration->Configuration->RateControl != NULL) {
		encode_infos.encode_info[0][0].bitrate_max = trt__SetVideoEncoderConfiguration->Configuration->RateControl->BitrateLimit;
		encode_infos.encode_info[0][0].fps = trt__SetVideoEncoderConfiguration->Configuration->RateControl->FrameRateLimit;
	}

	if (trt__SetVideoEncoderConfiguration->Configuration->H264 != NULL) {
		encode_infos.encode_info[0][0].gop = trt__SetVideoEncoderConfiguration->Configuration->H264->GovLength;
	}

	if (trt__SetVideoEncoderConfiguration->Configuration->MPEG4 != NULL) {
		encode_infos.encode_info[0][0].gop = trt__SetVideoEncoderConfiguration->Configuration->MPEG4->GovLength;
	}

	encode_infos.deal_cam = code/100;
	encode_infos.deal_stream = code%100 - 1;
	ret = OnvifOperationSetConfig(ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_INFO, &encode_infos, sizeof(OnvifConfigCameraVideoEncodeInfos));
	CHECK_LT(ret, 0, return 500);

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

static int GetH264Options(struct soap* soap, cJSON* json, int flag, void* h264) {
	if(flag == 0) {
		struct tt__H264Options* h264_option = (struct tt__H264Options*)h264;

		cJSON* resolutions = cJSON_GetObjectItemCaseSensitive(json, "resolutions");
		if (resolutions != NULL && cJSON_IsArray(resolutions)) {
			h264_option->__sizeResolutionsAvailable = cJSON_GetArraySize(resolutions);
			h264_option->ResolutionsAvailable = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution)*h264_option->__sizeResolutionsAvailable);
			memset(h264_option->ResolutionsAvailable, 0, sizeof(struct tt__VideoResolution));
			for(int i = 0; i < h264_option->__sizeResolutionsAvailable; i++) {
				cJSON* item = cJSON_GetArrayItem(resolutions, i);
				CHECK_POINTER(item, continue);
		
				h264_option->ResolutionsAvailable[i].Width = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(item, "width"));
				h264_option->ResolutionsAvailable[i].Height = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(item, "height"));
			}
		}
	
		SOAP_SET_RANGE_FROM_JSON(soap, h264_option->GovLengthRange, sizeof(struct tt__IntRange), json, "gov_length");
		SOAP_SET_RANGE_FROM_JSON(soap, h264_option->FrameRateRange, sizeof(struct tt__IntRange), json, "frame_rate");
		SOAP_SET_RANGE_FROM_JSON(soap, h264_option->EncodingIntervalRange, sizeof(struct tt__IntRange), json, "encoder_interval");
	
		int h264_profile[] = {tt__H264Profile__Baseline, tt__H264Profile__Main, tt__H264Profile__High};
		h264_option->__sizeH264ProfilesSupported = sizeof(h264_profile)/sizeof(int);
		SOAP_SET_NUMBER_LIST(soap, h264_option->H264ProfilesSupported, sizeof(enum tt__H264Profile), h264_option->__sizeH264ProfilesSupported, h264_profile);
	} else {
		struct tt__H264Options2* h264_option_2 = (struct tt__H264Options2*)h264;

		cJSON* resolutions = cJSON_GetObjectItemCaseSensitive(json, "resolutions");
		if (resolutions != NULL && cJSON_IsArray(resolutions)) {
			h264_option_2->__sizeResolutionsAvailable = cJSON_GetArraySize(resolutions);
			h264_option_2->ResolutionsAvailable = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution)*h264_option_2->__sizeResolutionsAvailable);
			memset(h264_option_2->ResolutionsAvailable, 0, sizeof(struct tt__VideoResolution));
			for(int i = 0; i < h264_option_2->__sizeResolutionsAvailable; i++) {
				cJSON* item = cJSON_GetArrayItem(resolutions, i);
				CHECK_POINTER(item, continue);
		
				h264_option_2->ResolutionsAvailable[i].Width = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(item, "width"));
				h264_option_2->ResolutionsAvailable[i].Height = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(item, "height"));
			}
		}
	
		SOAP_SET_RANGE_FROM_JSON(soap, h264_option_2->GovLengthRange, sizeof(struct tt__IntRange), json, "gov_length");
		SOAP_SET_RANGE_FROM_JSON(soap, h264_option_2->FrameRateRange, sizeof(struct tt__IntRange), json, "frame_rate");
		SOAP_SET_RANGE_FROM_JSON(soap, h264_option_2->EncodingIntervalRange, sizeof(struct tt__IntRange), json, "encoder_interval");
		SOAP_SET_RANGE_FROM_JSON(soap, h264_option_2->BitrateRange, sizeof(struct tt__IntRange), json, "bit_rate");
	
		int h264_profile[] = {tt__H264Profile__Baseline, tt__H264Profile__Main, tt__H264Profile__High};
		h264_option_2->__sizeH264ProfilesSupported = sizeof(h264_profile)/sizeof(int);
		SOAP_SET_NUMBER_LIST(soap, h264_option_2->H264ProfilesSupported, sizeof(enum tt__H264Profile), h264_option_2->__sizeH264ProfilesSupported, h264_profile);
	}

	return 0;
}

/** Web service operation '__trt__GetVideoEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurationOptions(struct soap* soap, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);
	
	int code = 0;
	if (trt__GetVideoEncoderConfigurationOptions->ProfileToken) {
		sscanf(trt__GetVideoEncoderConfigurationOptions->ProfileToken, "%*[^_]_%d\n", &code);
	} else if (trt__GetVideoEncoderConfigurationOptions->ConfigurationToken) {
		sscanf(trt__GetVideoEncoderConfigurationOptions->ConfigurationToken, "%*[^_]_%d\n", &code);
	} else {
		return 404;
	}

	cJSON* json = (cJSON*)OnvifConfGetConfig("video_encoder_options");
	CHECK_POINTER(json, return 500);

	cJSON* option = cJSON_GetArrayItem(cJSON_GetArrayItem(json, code/100), code%100 - 1);
	CHECK_POINTER(option, cJSON_free(json); return 500);

	struct tt__VideoEncoderConfigurationOptions* Options = (struct tt__VideoEncoderConfigurationOptions*)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfigurationOptions));
	memset(Options, 0, sizeof(struct tt__VideoEncoderConfigurationOptions));
	SOAP_SET_RANGE_FROM_JSON(soap, Options->QualityRange, sizeof(struct tt__IntRange), option, "quality");

	cJSON* h264 = cJSON_GetObjectItemCaseSensitive(option, "h264");
	if (h264 != NULL) {
		Options->H264 = (struct tt__H264Options*)soap_malloc(soap, sizeof(struct tt__H264Options));
		memset(Options->H264, 0, sizeof(struct tt__H264Options));
		GetH264Options(soap, h264, 0, Options->H264);
	}
	
	Options->Extension = (struct tt__VideoEncoderOptionsExtension*)soap_malloc(soap, sizeof(struct tt__VideoEncoderOptionsExtension));
	memset(Options->Extension, 0, sizeof(struct tt__VideoEncoderOptionsExtension));

	if (h264 != NULL) {
		Options->Extension->H264 = (struct tt__H264Options2*)soap_malloc(soap, sizeof(struct tt__H264Options2));
		memset(Options->Extension->H264, 0, sizeof(struct tt__H264Options2));
		GetH264Options(soap, h264, 1, Options->Extension->H264);
	}

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
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	if (trt__GetStreamUri->ProfileToken) {
		sscanf(trt__GetStreamUri->ProfileToken, "%*[^_]_%d\n", &code);
	} else {
		return 404;
	}

	OnvifSystem system_request;
	system_request.type = ONVIF_SYSTEM_RTSP_INFO;
	system_request.cam = code/100;
	OnvifOperationSystemRequest(&system_request);

	trt__GetStreamUriResponse->MediaUri = (struct tt__MediaUri*)soap_malloc(soap, sizeof(struct tt__MediaUri));
	memset(trt__GetStreamUriResponse->MediaUri, 0, sizeof(struct tt__MediaUri));

	switch (code%100 - 1)
	{
	case ONVIF_CONFIG_CAMERA_VIDEO_STREAM_MAIN:
		trt__GetStreamUriResponse->MediaUri->Uri = soap_strdup(soap, system_request.out.rtsp_url.main_stream);
		break;
	case ONVIF_CONFIG_CAMERA_VIDEO_STREAM_SUB:
		trt__GetStreamUriResponse->MediaUri->Uri = soap_strdup(soap, system_request.out.rtsp_url.sub_stream);
		break;
	case ONVIF_CONFIG_CAMERA_VIDEO_STREAM_THIRD:
		trt__GetStreamUriResponse->MediaUri->Uri = soap_strdup(soap, system_request.out.rtsp_url.third_stream);
		break;
	default:
		break;
	}

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
	printf("%s:%d\n", __func__, __LINE__);
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
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	if (trt__GetStreamUri->ConfigurationToken) {
		sscanf(trt__GetStreamUri->ConfigurationToken, "%*[^_]_%d\n", &code);
	} else {
		return 404;
	}

	OnvifConfigCameraOsdInfos osd_infos;
	memset(&osd_infos, 0, sizeof(OnvifConfigCameraOsdInfos));
	osd_infos.deal_num = code/100;
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_OSD_INFO, &osd_infos, sizeof(OnvifConfigCameraOsdInfos));
	CHECK_LT(ret, 0, return 500);

	trt__GetOSDsResponse->__sizeOSDs = 0;
	if (osd_infos.osd_info[0].show_camera_name) {
		trt__GetOSDsResponse->__sizeOSDs++;
	}

	if (osd_infos.osd_info[0].show_date) {
		trt__GetOSDsResponse->__sizeOSDs++;
	}

	for(int i = 0 ; i < ONVIF_CONFIG_OSD_INFO_STRING_OVERLAY_MAX; i++) {
		if (osd_infos.osd_info[0].string_overlay[i].show) {
			trt__GetOSDsResponse->__sizeOSDs++;
		}
	}

	trt__GetOSDsResponse->OSDs = (struct tt__OSDConfiguration*)soap_malloc(soap, sizeof(struct tt__OSDConfiguration)*trt__GetOSDsResponse->__sizeOSDs);
	memset(trt__GetOSDsResponse->OSDs, 0, sizeof(struct tt__OSDConfiguration)*trt__GetOSDsResponse->__sizeOSDs);
	
	if (osd_infos.osd_info[0].show_camera_name) {
		trt__GetOSDsResponse->OSDs[0].token = soap_strdup(soap, "ONFOsdToken_100");

		trt__GetOSDsResponse->OSDs[0].VideoSourceConfigurationToken = (struct tt__OSDReference*)soap_malloc(soap, sizeof(struct tt__OSDReference));
		memset(trt__GetOSDsResponse->OSDs[0].VideoSourceConfigurationToken, 0, sizeof(struct tt__OSDReference));
		trt__GetOSDsResponse->OSDs[0].VideoSourceConfigurationToken->__item = soap_strdup(soap, "trt__GetStreamUri->ConfigurationToken");
		trt__GetOSDsResponse->OSDs[0].Type = tt__OSDType__Text;

		trt__GetOSDsResponse->OSDs[0].Position = (struct tt__OSDPosConfiguration*)soap_malloc(soap, sizeof(struct tt__OSDPosConfiguration));
		memset(trt__GetOSDsResponse->OSDs[0].Position, 0, sizeof(struct tt__OSDPosConfiguration));
		trt__GetOSDsResponse->OSDs[0].Position->Type = soap_strdup(soap, "Custom");
		trt__GetOSDsResponse->OSDs[0].Position->Pos = (struct tt__Vector*)soap_malloc(soap, sizeof(struct tt__Vector));
		memset(trt__GetOSDsResponse->OSDs[0].Position->Pos, 0, sizeof(struct tt__Vector));
		SOAP_SET_NUMBER(soap, trt__GetOSDsResponse->OSDs[0].Position->Pos->x, sizeof(float), osd_infos.osd_info[0].camera_name_pos.x);
		SOAP_SET_NUMBER(soap, trt__GetOSDsResponse->OSDs[0].Position->Pos->y, sizeof(float), osd_infos.osd_info[0].camera_name_pos.y);

		trt__GetOSDsResponse->OSDs[0].TextString = (struct tt__OSDTextConfiguration*)soap_malloc(soap, sizeof(struct tt__OSDTextConfiguration));
		memset(trt__GetOSDsResponse->OSDs[0].TextString, 0, sizeof(struct tt__OSDTextConfiguration));
		trt__GetOSDsResponse->OSDs[0].TextString->Type = soap_strdup(soap, "Plain");
		SOAP_SET_NUMBER(soap, trt__GetOSDsResponse->OSDs[0].TextString->FontSize, sizeof(int), 64);
		trt__GetOSDsResponse->OSDs[0].TextString->PlainText = soap_strdup(soap, osd_infos.osd_info[0].camera_name);

		// trt__GetOSDsResponse->OSDs[0].TextString->Extension = (struct tt__OSDTextConfigurationExtension*)soap_malloc(soap, sizeof(struct tt__OSDTextConfigurationExtension));
		// memset(trt__GetOSDsResponse->OSDs[0].TextString->Extension, 0, sizeof(struct tt__OSDTextConfigurationExtension));
	}

	if (osd_infos.osd_info[0].show_date) {
		trt__GetOSDsResponse->OSDs[1].token = soap_strdup(soap, "ONFOsdToken_101");

		trt__GetOSDsResponse->OSDs[1].VideoSourceConfigurationToken = (struct tt__OSDReference*)soap_malloc(soap, sizeof(struct tt__OSDReference));
		memset(trt__GetOSDsResponse->OSDs[1].VideoSourceConfigurationToken, 0, sizeof(struct tt__OSDReference));
		trt__GetOSDsResponse->OSDs[1].VideoSourceConfigurationToken->__item = soap_strdup(soap, "trt__GetStreamUri->ConfigurationToken");
		trt__GetOSDsResponse->OSDs[1].Type = tt__OSDType__Text;

		trt__GetOSDsResponse->OSDs[1].Position = (struct tt__OSDPosConfiguration*)soap_malloc(soap, sizeof(struct tt__OSDPosConfiguration));
		memset(trt__GetOSDsResponse->OSDs[1].Position, 0, sizeof(struct tt__OSDPosConfiguration));
		trt__GetOSDsResponse->OSDs[1].Position->Type = soap_strdup(soap, "Custom");
		trt__GetOSDsResponse->OSDs[1].Position->Pos = (struct tt__Vector*)soap_malloc(soap, sizeof(struct tt__Vector));
		memset(trt__GetOSDsResponse->OSDs[1].Position->Pos, 0, sizeof(struct tt__Vector));
		SOAP_SET_NUMBER(soap, trt__GetOSDsResponse->OSDs[1].Position->Pos->x, sizeof(float), osd_infos.osd_info[0].camera_name_pos.x);
		SOAP_SET_NUMBER(soap, trt__GetOSDsResponse->OSDs[1].Position->Pos->y, sizeof(float), osd_infos.osd_info[0].camera_name_pos.y);

		trt__GetOSDsResponse->OSDs[1].TextString = (struct tt__OSDTextConfiguration*)soap_malloc(soap, sizeof(struct tt__OSDTextConfiguration));
		memset(trt__GetOSDsResponse->OSDs[1].TextString, 0, sizeof(struct tt__OSDTextConfiguration));
		trt__GetOSDsResponse->OSDs[1].TextString->Type = soap_strdup(soap, "DateAndTime");
		trt__GetOSDsResponse->OSDs[1].TextString->DateFormat = soap_strdup(soap, "yyyy-MM-dd");
		trt__GetOSDsResponse->OSDs[1].TextString->TimeFormat = soap_strdup(soap, "HH:mm:ss");
		SOAP_SET_NUMBER(soap, trt__GetOSDsResponse->OSDs[1].TextString->FontSize, sizeof(int), 64);
	}

	for (int i = 0; i < ONVIF_CONFIG_OSD_INFO_STRING_OVERLAY_MAX; i++) {
		if (!osd_infos.osd_info[0].string_overlay[i].show) {
			continue;
		}

		char token[16] = {0};
		snprintf(token, sizeof(token), "ONFOsdToken_1%02d", i+2);
		trt__GetOSDsResponse->OSDs[i+2].token = soap_strdup(soap, token);

		trt__GetOSDsResponse->OSDs[i+2].VideoSourceConfigurationToken = (struct tt__OSDReference*)soap_malloc(soap, sizeof(struct tt__OSDReference));
		memset(trt__GetOSDsResponse->OSDs[i+2].VideoSourceConfigurationToken, 0, sizeof(struct tt__OSDReference));
		trt__GetOSDsResponse->OSDs[i+2].VideoSourceConfigurationToken->__item = soap_strdup(soap, "trt__GetStreamUri->ConfigurationToken");
		trt__GetOSDsResponse->OSDs[i+2].Type = tt__OSDType__Text;

		trt__GetOSDsResponse->OSDs[i+2].Position = (struct tt__OSDPosConfiguration*)soap_malloc(soap, sizeof(struct tt__OSDPosConfiguration));
		memset(trt__GetOSDsResponse->OSDs[i+2].Position, 0, sizeof(struct tt__OSDPosConfiguration));
		trt__GetOSDsResponse->OSDs[i+2].Position->Type = soap_strdup(soap, "Custom");
		trt__GetOSDsResponse->OSDs[i+2].Position->Pos = (struct tt__Vector*)soap_malloc(soap, sizeof(struct tt__Vector));
		memset(trt__GetOSDsResponse->OSDs[i+2].Position->Pos, 0, sizeof(struct tt__Vector));
		SOAP_SET_NUMBER(soap, trt__GetOSDsResponse->OSDs[i+2].Position->Pos->x, sizeof(float), osd_infos.osd_info[0].string_overlay[i].pos.x);
		SOAP_SET_NUMBER(soap, trt__GetOSDsResponse->OSDs[i+2].Position->Pos->y, sizeof(float), osd_infos.osd_info[0].string_overlay[i].pos.y);

		trt__GetOSDsResponse->OSDs[i+2].TextString = (struct tt__OSDTextConfiguration*)soap_malloc(soap, sizeof(struct tt__OSDTextConfiguration));
		memset(trt__GetOSDsResponse->OSDs[i+2].TextString, 0, sizeof(struct tt__OSDTextConfiguration));
		trt__GetOSDsResponse->OSDs[i+2].TextString->Type = soap_strdup(soap, "Plain");
		SOAP_SET_NUMBER(soap, trt__GetOSDsResponse->OSDs[i+2].TextString->FontSize, sizeof(int), 64);
		trt__GetOSDsResponse->OSDs[i+2].TextString->PlainText = soap_strdup(soap, osd_infos.osd_info[0].string_overlay[i].value);
	}



			/** Optional element 'tt:TextString' of XML schema type 'tt:OSDTextConfiguration' */
			struct tt__OSDTextConfiguration *TextString;



			        /** Required element 'tt:Type' of XML schema type 'xsd:string' */
					char *Type;
					/** Optional element 'tt:DateFormat' of XML schema type 'xsd:string' */
					char *DateFormat;
					/** Optional element 'tt:TimeFormat' of XML schema type 'xsd: