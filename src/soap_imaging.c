#include "soapStub.h"
#include "config.h"
#include "check_common.h"
#include "soap_common.h"
#include "auth.h"
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetServiceCapabilities(struct soap* soap, struct _timg__GetServiceCapabilities *timg__GetServiceCapabilities, struct _timg__GetServiceCapabilitiesResponse *timg__GetServiceCapabilitiesResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__timg__GetImagingSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetImagingSettings(struct soap* soap, struct _timg__GetImagingSettings *timg__GetImagingSettings, struct _timg__GetImagingSettingsResponse *timg__GetImagingSettingsResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    // todo token查找校验
    LOG_INFO("%s", timg__GetImagingSettings->VideoSourceToken);

    timg__GetImagingSettingsResponse->ImagingSettings = (struct tt__ImagingSettings20*)soap_malloc(soap, sizeof(struct tt__ImagingSettings20));
    memset(timg__GetImagingSettingsResponse->ImagingSettings, 0, sizeof(struct tt__ImagingSettings20));
    timg__GetImagingSettingsResponse->ImagingSettings->BacklightCompensation = (struct tt__BacklightCompensation20*)soap_malloc(soap, sizeof(struct tt__BacklightCompensation20));
    timg__GetImagingSettingsResponse->ImagingSettings->BacklightCompensation->Mode = tt__BacklightCompensationMode__OFF;
    SOAP_SET_NUMBER(soap, timg__GetImagingSettingsResponse->ImagingSettings->BacklightCompensation->Level, sizeof(float), 0);
    SOAP_SET_NUMBER(soap, timg__GetImagingSettingsResponse->ImagingSettings->Brightness, sizeof(float), 50);
    SOAP_SET_NUMBER(soap, timg__GetImagingSettingsResponse->ImagingSettings->ColorSaturation, sizeof(float), 50);
    SOAP_SET_NUMBER(soap, timg__GetImagingSettingsResponse->ImagingSettings->Contrast, sizeof(float), 50);
    SOAP_SET_NUMBER(soap, timg__GetImagingSettingsResponse->ImagingSettings->Sharpness, sizeof(float), 50);

    return 0;
}
/** Web service operation '__timg__SetImagingSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__SetImagingSettings(struct soap* soap, struct _timg__SetImagingSettings *timg__SetImagingSettings, struct _timg__SetImagingSettingsResponse *timg__SetImagingSettingsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__timg__GetOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetOptions(struct soap* soap, struct _timg__GetOptions *timg__GetOptions, struct _timg__GetOptionsResponse *timg__GetOptionsResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    // todo token查找校验
    LOG_INFO("%s", timg__GetOptions->VideoSourceToken);

    timg__GetOptionsResponse->ImagingOptions = (struct tt__ImagingOptions20*)soap_malloc(soap, sizeof(struct tt__ImagingOptions20));
    memset(timg__GetOptionsResponse->ImagingOptions, 0, sizeof(struct tt__ImagingOptions20));
    timg__GetOptionsResponse->ImagingOptions->BacklightCompensation = (struct tt__BacklightCompensationOptions20*)soap_malloc(soap, sizeof(struct tt__BacklightCompensationOptions20));
    int backlight_mode[] = {tt__BacklightCompensationMode__OFF, tt__BacklightCompensationMode__ON};
    timg__GetOptionsResponse->ImagingOptions->BacklightCompensation->__sizeMode = sizeof(backlight_mode)/sizeof(int);
    SOAP_SET_NUMBER_LIST(soap, timg__GetOptionsResponse->ImagingOptions->BacklightCompensation->Mode, 
        sizeof(enum tt__BacklightCompensationMode), timg__GetOptionsResponse->ImagingOptions->BacklightCompensation->__sizeMode, backlight_mode);
    SOAP_SET_RANGE(soap, timg__GetOptionsResponse->ImagingOptions->BacklightCompensation->Level, sizeof(struct tt__FloatRange), 0, 99);
    SOAP_SET_RANGE(soap, timg__GetOptionsResponse->ImagingOptions->Brightness, sizeof(struct tt__FloatRange), 0, 99);
    SOAP_SET_RANGE(soap, timg__GetOptionsResponse->ImagingOptions->ColorSaturation, sizeof(struct tt__FloatRange), 0, 99);
    SOAP_SET_RANGE(soap, timg__GetOptionsResponse->ImagingOptions->Contrast, sizeof(struct tt__FloatRange), 0, 99);
    SOAP_SET_RANGE(soap, timg__GetOptionsResponse->ImagingOptions->Sharpness, sizeof(struct tt__FloatRange), 0, 99);

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
    CHECK_LT(AuthUser(soap), 0, return 401);

    // todo token查找校验
    LOG_INFO("%s", timg__GetMoveOptions->VideoSourceToken);

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
