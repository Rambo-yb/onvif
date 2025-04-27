#include "soapStub.h"
#include "onvif_operation.h"
#include "onvif_auth.h"
#include "onvif_conf.h"
#include "check_common.h"
#include "cjson_common.h"
#include "soap_common.h"

void GetImagingServiceCapabilities(struct soap* soap, struct timg__Capabilities* capabilities) {
	SOAP_SET_NUMBER(soap, capabilities->ImageStabilization, sizeof(enum xsd__boolean), xsd__boolean__false_);
}

SOAP_FMAC5 int SOAP_FMAC6 __timg__GetServiceCapabilities(struct soap* soap, struct _timg__GetServiceCapabilities *timg__GetServiceCapabilities, struct _timg__GetServiceCapabilitiesResponse *timg__GetServiceCapabilitiesResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	timg__GetServiceCapabilitiesResponse->Capabilities = (struct timg__Capabilities*)soap_malloc(soap, sizeof(struct timg__Capabilities));
	memset(timg__GetServiceCapabilitiesResponse->Capabilities, 0, sizeof(struct timg__Capabilities));
	GetImagingServiceCapabilities(soap, timg__GetServiceCapabilitiesResponse->Capabilities);
	return 0;
}
/** Web service operation '__timg__GetImagingSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetImagingSettings(struct soap* soap, struct _timg__GetImagingSettings *timg__GetImagingSettings, struct _timg__GetImagingSettingsResponse *timg__GetImagingSettingsResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	if (timg__GetImagingSettings->VideoSourceToken) {
		sscanf(timg__GetImagingSettings->VideoSourceToken, "%*[^_]_%d\n", &code);
	} else {
		SOAP_ERROR_REPLAY(soap, "invalid token", 400);
	}

	OnvifConfigCameraImageInfos image_infos;
	memset(&image_infos, 0, sizeof(OnvifConfigCameraImageInfos));
	image_infos.deal_cam = code/100;
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_IMAGE_INFO, &image_infos, sizeof(OnvifConfigCameraImageInfos));
	CHECK_LT(ret, 0, return 500);

	timg__GetImagingSettingsResponse->ImagingSettings = (struct tt__ImagingSettings20*)soap_malloc(soap, sizeof(struct tt__ImagingSettings20));
	memset(timg__GetImagingSettingsResponse->ImagingSettings, 0, sizeof(struct tt__ImagingSettings20));
	SOAP_SET_NUMBER(soap, timg__GetImagingSettingsResponse->ImagingSettings->Brightness, sizeof(float), image_infos.image_info[0].brightness);
	SOAP_SET_NUMBER(soap, timg__GetImagingSettingsResponse->ImagingSettings->ColorSaturation, sizeof(float), image_infos.image_info[0].saturation);
	SOAP_SET_NUMBER(soap, timg__GetImagingSettingsResponse->ImagingSettings->Contrast, sizeof(float), image_infos.image_info[0].contrast);
	SOAP_SET_NUMBER(soap, timg__GetImagingSettingsResponse->ImagingSettings->Sharpness, sizeof(float), image_infos.image_info[0].sharpness);

	return 0;
}
/** Web service operation '__timg__SetImagingSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__SetImagingSettings(struct soap* soap, struct _timg__SetImagingSettings *timg__SetImagingSettings, struct _timg__SetImagingSettingsResponse *timg__SetImagingSettingsResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);
	CHECK_POINTER(timg__SetImagingSettings->ImagingSettings, return 400);

	int code = 0;
	if (timg__SetImagingSettings->VideoSourceToken) {
		sscanf(timg__SetImagingSettings->VideoSourceToken, "%*[^_]_%d\n", &code);
	} else {
		SOAP_ERROR_REPLAY(soap, "invalid token", 400);
	}

	OnvifConfigCameraImageInfos image_infos;
	memset(&image_infos, 0, sizeof(OnvifConfigCameraImageInfos));
	image_infos.deal_cam = code/100;
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_CAMERA_IMAGE_INFO, &image_infos, sizeof(OnvifConfigCameraImageInfos));
	CHECK_LT(ret, 0, return 500);

	if (timg__SetImagingSettings->ImagingSettings->Brightness != NULL) {
		image_infos.image_info[0].brightness = *timg__SetImagingSettings->ImagingSettings->Brightness;
	}
	if (timg__SetImagingSettings->ImagingSettings->ColorSaturation != NULL) {
		image_infos.image_info[0].saturation = *timg__SetImagingSettings->ImagingSettings->ColorSaturation;
	}
	if (timg__SetImagingSettings->ImagingSettings->Contrast != NULL) {
		image_infos.image_info[0].contrast = *timg__SetImagingSettings->ImagingSettings->Contrast;
	}
	if (timg__SetImagingSettings->ImagingSettings->Sharpness != NULL) {
		image_infos.image_info[0].sharpness = *timg__SetImagingSettings->ImagingSettings->Sharpness;
	}

	ret = OnvifOperationSetConfig(ONVIF_CONFIG_CAMERA_IMAGE_INFO, &image_infos, sizeof(OnvifConfigCameraImageInfos));
	CHECK_LT(ret, 0, return 500);

	return 0;
}
/** Web service operation '__timg__GetOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetOptions(struct soap* soap, struct _timg__GetOptions *timg__GetOptions, struct _timg__GetOptionsResponse *timg__GetOptionsResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	if (timg__GetOptions->VideoSourceToken) {
		sscanf(timg__GetOptions->VideoSourceToken, "%*[^_]_%d\n", &code);
	} else {
		SOAP_ERROR_REPLAY(soap, "invalid token", 400);
	}

	cJSON* json = (cJSON*)OnvifConfGetConfig("imaging_options");
	CHECK_POINTER(json, return 500);

	cJSON* option = cJSON_GetArrayItem(json, code/100);
	CHECK_POINTER(option, cJSON_free(json); return 500);

	struct tt__ImagingOptions20 *imaging_options = (struct tt__ImagingOptions20*)soap_malloc(soap, sizeof(struct tt__ImagingOptions20));
	memset(imaging_options, 0, sizeof(struct tt__ImagingOptions20));

	SOAP_SET_RANGE_FROM_JSON(soap, imaging_options->Brightness, sizeof(struct tt__FloatRange), option, "brightness");
	SOAP_SET_RANGE_FROM_JSON(soap, imaging_options->ColorSaturation, sizeof(struct tt__FloatRange), option, "saturation");
	SOAP_SET_RANGE_FROM_JSON(soap, imaging_options->Contrast, sizeof(struct tt__FloatRange), option, "contrast");
	SOAP_SET_RANGE_FROM_JSON(soap, imaging_options->Sharpness, sizeof(struct tt__FloatRange), option, "sharpness");

	cJSON_free(json);
	timg__GetOptionsResponse->ImagingOptions = imaging_options;

	return 0;
}
/** Web service operation '__timg__Move' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__Move(struct soap* soap, struct _timg__Move *timg__Move, struct _timg__MoveResponse *timg__MoveResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__timg__Stop' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__Stop(struct soap* soap, struct _timg__Stop *timg__Stop, struct _timg__StopResponse *timg__StopResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__timg__GetStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetStatus(struct soap* soap, struct _timg__GetStatus *timg__GetStatus, struct _timg__GetStatusResponse *timg__GetStatusResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__timg__GetMoveOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetMoveOptions(struct soap* soap, struct _timg__GetMoveOptions *timg__GetMoveOptions, struct _timg__GetMoveOptionsResponse *timg__GetMoveOptionsResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	timg__GetMoveOptionsResponse->MoveOptions = (struct tt__MoveOptions20*)soap_malloc(soap, sizeof(struct tt__MoveOptions20));
	timg__GetMoveOptionsResponse->MoveOptions->Absolute = (struct tt__AbsoluteFocusOptions*)soap_malloc(soap, sizeof(struct tt__AbsoluteFocusOptions));
	SOAP_SET_RANGE(soap, timg__GetMoveOptionsResponse->MoveOptions->Absolute->Position, sizeof(struct tt__FloatRange), -180, 180);
	SOAP_SET_RANGE(soap, timg__GetMoveOptionsResponse->MoveOptions->Absolute->Speed , sizeof(struct tt__FloatRange), 0, 10);
	timg__GetMoveOptionsResponse->MoveOptions->Relative = (struct tt__RelativeFocusOptions20*)soap_malloc(soap, sizeof(struct tt__RelativeFocusOptions20));
	SOAP_SET_RANGE(soap, timg__GetMoveOptionsResponse->MoveOptions->Relative->Distance, sizeof(struct tt__FloatRange), 0, 10);
	SOAP_SET_RANGE(soap, timg__GetMoveOptionsResponse->MoveOptions->Relative->Speed, sizeof(struct tt__FloatRange), 0, 10);
	timg__GetMoveOptionsResponse->MoveOptions->Continuous = (struct tt__ContinuousFocusOptions*)soap_malloc(soap, sizeof(struct tt__ContinuousFocusOptions));
	SOAP_SET_RANGE(soap, timg__GetMoveOptionsResponse->MoveOptions->Continuous->Speed, sizeof(struct tt__FloatRange), 0, 10);

	return 0;
}
/** Web service operation '__timg__GetPresets' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetPresets(struct soap* soap, struct _timg__GetPresets *timg__GetPresets, struct _timg__GetPresetsResponse *timg__GetPresetsResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__timg__GetCurrentPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetCurrentPreset(struct soap* soap, struct _timg__GetCurrentPreset *timg__GetCurrentPreset, struct _timg__GetCurrentPresetResponse *timg__GetCurrentPresetResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__timg__SetCurrentPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__SetCurrentPreset(struct soap* soap, struct _timg__SetCurrentPreset *timg__SetCurrentPreset, struct _timg__SetCurrentPresetResponse *timg__SetCurrentPresetResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
