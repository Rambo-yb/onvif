#include "soapStub.h"
#include "onvif_operation.h"
#include "check_common.h"
#include "cjson_common.h"
#include "soap_common.h"
#include "auth.h"
/** Web service operation '__tptz__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetServiceCapabilities(struct soap* soap, struct _tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, struct _tptz__GetServiceCapabilitiesResponse *tptz__GetServiceCapabilitiesResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__GetConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurations(struct soap* soap, struct _tptz__GetConfigurations *tptz__GetConfigurations, struct _tptz__GetConfigurationsResponse *tptz__GetConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__GetPresets' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresets(struct soap* soap, struct _tptz__GetPresets *tptz__GetPresets, struct _tptz__GetPresetsResponse *tptz__GetPresetsResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    cJSON* profiles_json = OnvifOperationGetConfig("profiles");
    CHECK_POINTER(profiles_json, return 500);
    CHECK_BOOL(cJSON_IsArray(profiles_json), return 500);

    for(int i = 0; i < cJSON_GetArraySize(profiles_json); i++) {
        cJSON* profile_json = cJSON_GetArrayItem(profiles_json, i);
        if (profile_json == NULL || !cJSON_IsObject(profile_json)) {
            continue;
        }

        if (strcmp(tptz__GetPresets->ProfileToken, cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(profile_json, "token"))) == 0) {
            cJSON* presets = cJSON_GetObjectItemCaseSensitive(profile_json, "presets");
            if (profile_json == NULL || !cJSON_IsArray(presets)) {
                break;
            }

            tptz__GetPresetsResponse->__sizePreset = cJSON_GetArraySize(presets);
            tptz__GetPresetsResponse->Preset = (struct tt__PTZPreset*)soap_malloc(soap, sizeof(struct tt__PTZPreset)*tptz__GetPresetsResponse->__sizePreset);
            memset(tptz__GetPresetsResponse->Preset, 0, sizeof(struct tt__PTZPreset)*tptz__GetPresetsResponse->__sizePreset);
            for(int j = 0; j < tptz__GetPresetsResponse->__sizePreset; j++) {
                cJSON* item = cJSON_GetArrayItem(presets, j);
                if (item == NULL || !cJSON_IsObject(item)) {
                    continue;
                }

                SOAP_CJSON_GET_STRING(item, soap, "token", tptz__GetPresetsResponse->Preset[j].token);
                SOAP_CJSON_GET_STRING(item, soap, "name", tptz__GetPresetsResponse->Preset[j].Name);
            }

            break;
        }
    }

    return 0;
}
/** Web service operation '__tptz__SetPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetPreset(struct soap* soap, struct _tptz__SetPreset *tptz__SetPreset, struct _tptz__SetPresetResponse *tptz__SetPresetResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    OnvifPresets presets;
    memset(&presets, 0, sizeof(OnvifPresets));
    presets.type = ONVIF_PRESET_CTRL_ADD;
    presets.preset_num = 1;
    snprintf(presets.preset[0], sizeof(presets.preset[0]), "%s", tptz__SetPreset->PresetName);
    OnvifOperationSetConfig("presets", &presets);

    return 0;
}
/** Web service operation '__tptz__RemovePreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePreset(struct soap* soap, struct _tptz__RemovePreset *tptz__RemovePreset, struct _tptz__RemovePresetResponse *tptz__RemovePresetResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    OnvifPresets presets;
    memset(&presets, 0, sizeof(OnvifPresets));
    presets.type = ONVIF_PRESET_CTRL_DEL;
    presets.preset_num = 1;
    snprintf(presets.preset[0], sizeof(presets.preset[0]), "%s", tptz__RemovePreset->PresetToken);
    OnvifOperationSetConfig("presets", &presets);
    return 0;
}
/** Web service operation '__tptz__GotoPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoPreset(struct soap* soap, struct _tptz__GotoPreset *tptz__GotoPreset, struct _tptz__GotoPresetResponse *tptz__GotoPresetResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);
    
    OnvifOperationPtzCtrl(ONVIF_PTZ_CTRL_PRESET, tptz__GotoPreset->PresetToken);
    return 0;
}
/** Web service operation '__tptz__GetStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetStatus(struct soap* soap, struct _tptz__GetStatus *tptz__GetStatus, struct _tptz__GetStatusResponse *tptz__GetStatusResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    // todo token查找校验
    LOG_INFO("%s", tptz__GetStatus->ProfileToken);

    tptz__GetStatusResponse->PTZStatus = (struct tt__PTZStatus*)soap_malloc(soap, sizeof(struct tt__PTZStatus));
    memset(tptz__GetStatusResponse->PTZStatus, 0, sizeof(struct tt__PTZStatus));
    tptz__GetStatusResponse->PTZStatus->Position = (struct tt__PTZVector*)soap_malloc(soap, sizeof(struct tt__PTZVector));
    tptz__GetStatusResponse->PTZStatus->Position->PanTilt = (struct tt__Vector2D*)soap_malloc(soap, sizeof(struct tt__Vector2D));
    tptz__GetStatusResponse->PTZStatus->Position->PanTilt->space = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
    tptz__GetStatusResponse->PTZStatus->Position->PanTilt->x = 30;
    tptz__GetStatusResponse->PTZStatus->Position->PanTilt->y = 40;
    tptz__GetStatusResponse->PTZStatus->Position->Zoom = (struct tt__Vector1D*)soap_malloc(soap, sizeof(struct tt__Vector1D));
    tptz__GetStatusResponse->PTZStatus->Position->Zoom->space = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
    tptz__GetStatusResponse->PTZStatus->Position->Zoom->x = 0.0f;
    tptz__GetStatusResponse->PTZStatus->MoveStatus = (struct tt__PTZMoveStatus*)soap_malloc(soap, sizeof(struct tt__PTZMoveStatus));
    SOAP_SET_NUMBER(soap, tptz__GetStatusResponse->PTZStatus->MoveStatus->PanTilt, sizeof(enum tt__MoveStatus), tt__MoveStatus__IDLE);
    SOAP_SET_NUMBER(soap, tptz__GetStatusResponse->PTZStatus->MoveStatus->Zoom, sizeof(enum tt__MoveStatus), tt__MoveStatus__IDLE);
    tptz__GetStatusResponse->PTZStatus->UtcTime = time(NULL);

    return 0;
}
/** Web service operation '__tptz__GetConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfiguration(struct soap* soap, struct _tptz__GetConfiguration *tptz__GetConfiguration, struct _tptz__GetConfigurationResponse *tptz__GetConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
static void GetNode(struct soap* soap, struct tt__PTZNode* ptz_node) {
    ptz_node->token = soap_strdup(soap, "default_ptz_node_token");
    ptz_node->Name = soap_strdup(soap, "ptz_node");
    SOAP_SET_NUMBER(soap, ptz_node->FixedHomePosition, sizeof(enum xsd__boolean), xsd__boolean__false_);
    SOAP_SET_NUMBER(soap, ptz_node->GeoMove, sizeof(enum xsd__boolean), xsd__boolean__false_);
    ptz_node->MaximumNumberOfPresets = 1;
    ptz_node->HomeSupported = xsd__boolean__false_;
    ptz_node->SupportedPTZSpaces = (struct tt__PTZSpaces*)soap_malloc(soap, sizeof(struct tt__PTZSpaces));
    memset(ptz_node->SupportedPTZSpaces, 0, sizeof(struct tt__PTZSpaces));
    ptz_node->SupportedPTZSpaces->__sizeAbsolutePanTiltPositionSpace = 1;
    ptz_node->SupportedPTZSpaces->AbsolutePanTiltPositionSpace = (struct tt__Space2DDescription*)soap_malloc(soap, sizeof(struct tt__Space2DDescription)*ptz_node->SupportedPTZSpaces->__sizeAbsolutePanTiltPositionSpace);
    ptz_node->SupportedPTZSpaces->AbsolutePanTiltPositionSpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
    SOAP_SET_RANGE(soap, ptz_node->SupportedPTZSpaces->AbsolutePanTiltPositionSpace[0].XRange, sizeof(struct tt__FloatRange), -100, 100);
    SOAP_SET_RANGE(soap, ptz_node->SupportedPTZSpaces->AbsolutePanTiltPositionSpace[0].YRange, sizeof(struct tt__FloatRange), -100, 100);

    ptz_node->SupportedPTZSpaces->__sizeAbsoluteZoomPositionSpace = 1;
    ptz_node->SupportedPTZSpaces->AbsoluteZoomPositionSpace = (struct tt__Space1DDescription*)soap_malloc(soap, sizeof(struct tt__Space1DDescription)*ptz_node->SupportedPTZSpaces->__sizeAbsoluteZoomPositionSpace);
    ptz_node->SupportedPTZSpaces->AbsoluteZoomPositionSpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
    SOAP_SET_RANGE(soap, ptz_node->SupportedPTZSpaces->AbsoluteZoomPositionSpace[0].XRange, sizeof(struct tt__FloatRange), -100, 100);

    ptz_node->SupportedPTZSpaces->__sizeRelativePanTiltTranslationSpace = 1;
    ptz_node->SupportedPTZSpaces->RelativePanTiltTranslationSpace = (struct tt__Space2DDescription*)soap_malloc(soap, sizeof(struct tt__Space2DDescription)*ptz_node->SupportedPTZSpaces->__sizeRelativePanTiltTranslationSpace);
    ptz_node->SupportedPTZSpaces->RelativePanTiltTranslationSpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");
    SOAP_SET_RANGE(soap, ptz_node->SupportedPTZSpaces->RelativePanTiltTranslationSpace[0].XRange, sizeof(struct tt__FloatRange), -100, 100);
    SOAP_SET_RANGE(soap, ptz_node->SupportedPTZSpaces->RelativePanTiltTranslationSpace[0].YRange, sizeof(struct tt__FloatRange), -100, 100);

    ptz_node->SupportedPTZSpaces->__sizeRelativeZoomTranslationSpace = 1;
    ptz_node->SupportedPTZSpaces->RelativeZoomTranslationSpace = (struct tt__Space1DDescription*)soap_malloc(soap, sizeof(struct tt__Space1DDescription)*ptz_node->SupportedPTZSpaces->__sizeAbsoluteZoomPositionSpace);
    ptz_node->SupportedPTZSpaces->RelativeZoomTranslationSpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");
    SOAP_SET_RANGE(soap, ptz_node->SupportedPTZSpaces->RelativeZoomTranslationSpace[0].XRange, sizeof(struct tt__FloatRange), -100, 100);

}
/** Web service operation '__tptz__GetNodes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNodes(struct soap* soap, struct _tptz__GetNodes *tptz__GetNodes, struct _tptz__GetNodesResponse *tptz__GetNodesResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    tptz__GetNodesResponse->__sizePTZNode = 1;
    tptz__GetNodesResponse->PTZNode = (struct tt__PTZNode*)soap_malloc(soap, sizeof(struct tt__PTZNode)*tptz__GetNodesResponse->__sizePTZNode);
    memset(tptz__GetNodesResponse->PTZNode, 0, sizeof(struct tt__PTZNode)*tptz__GetNodesResponse->__sizePTZNode);
    for(int i = 0; i < tptz__GetNodesResponse->__sizePTZNode; i++) {
        GetNode(soap, &tptz__GetNodesResponse->PTZNode[i]);
    }
    return 0;
}
/** Web service operation '__tptz__GetNode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNode(struct soap* soap, struct _tptz__GetNode *tptz__GetNode, struct _tptz__GetNodeResponse *tptz__GetNodeResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    // todo token查找校验
    LOG_INFO("%s", tptz__GetNode->NodeToken);

    tptz__GetNodeResponse->PTZNode = (struct tt__PTZNode*)soap_malloc(soap, sizeof(struct tt__PTZNode));
    memset(tptz__GetNodeResponse->PTZNode, 0, sizeof(struct tt__PTZNode));
    GetNode(soap, tptz__GetNodeResponse->PTZNode);
    return 0;
}
/** Web service operation '__tptz__SetConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetConfiguration(struct soap* soap, struct _tptz__SetConfiguration *tptz__SetConfiguration, struct _tptz__SetConfigurationResponse *tptz__SetConfigurationResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__GetConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurationOptions(struct soap* soap, struct _tptz__GetConfigurationOptions *tptz__GetConfigurationOptions, struct _tptz__GetConfigurationOptionsResponse *tptz__GetConfigurationOptionsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__GotoHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoHomePosition(struct soap* soap, struct _tptz__GotoHomePosition *tptz__GotoHomePosition, struct _tptz__GotoHomePositionResponse *tptz__GotoHomePositionResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);
    
    OnvifOperationPtzCtrl(ONVIF_PTZ_CTRL_HOME, NULL);
    return 0;
}
/** Web service operation '__tptz__SetHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetHomePosition(struct soap* soap, struct _tptz__SetHomePosition *tptz__SetHomePosition, struct _tptz__SetHomePositionResponse *tptz__SetHomePositionResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    OnvifPresets presets;
    memset(&presets, 0, sizeof(OnvifPresets));
    presets.type = ONVIF_PRESET_CTRL_SET_HOME;
    presets.preset_num = 0;
    OnvifOperationSetConfig("presets", &presets);
    return 0;
}
/** Web service operation '__tptz__ContinuousMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__ContinuousMove(struct soap* soap, struct _tptz__ContinuousMove *tptz__ContinuousMove, struct _tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__RelativeMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RelativeMove(struct soap* soap, struct _tptz__RelativeMove *tptz__RelativeMove, struct _tptz__RelativeMoveResponse *tptz__RelativeMoveResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    if (tptz__RelativeMove->Translation != NULL && tptz__RelativeMove->Translation->PanTilt != NULL) {
        // LOG_INFO("%s, %f, %f", tptz__RelativeMove->Translation->PanTilt->space, tptz__RelativeMove->Translation->PanTilt->x, tptz__RelativeMove->Translation->PanTilt->y);
        if (tptz__RelativeMove->Translation->PanTilt->x < 0) {
            OnvifOperationPtzCtrl(ONVIF_PTZ_CTRL_LEFT, NULL);
        } else if (tptz__RelativeMove->Translation->PanTilt->x > 0) {
            OnvifOperationPtzCtrl(ONVIF_PTZ_CTRL_RIGHT, NULL);
        }
        if (tptz__RelativeMove->Translation->PanTilt->y < 0) {
            OnvifOperationPtzCtrl(ONVIF_PTZ_CTRL_DOWN, NULL);
        } else if (tptz__RelativeMove->Translation->PanTilt->y > 0) {
            OnvifOperationPtzCtrl(ONVIF_PTZ_CTRL_UP, NULL);
        }
    }
    if (tptz__RelativeMove->Translation != NULL && tptz__RelativeMove->Translation->Zoom != NULL) {
        LOG_INFO("%s, %f", tptz__RelativeMove->Translation->Zoom->space, tptz__RelativeMove->Translation->Zoom->x);
    }
    if (tptz__RelativeMove->Speed != NULL && tptz__RelativeMove->Speed->PanTilt != NULL) {
        LOG_INFO("%s, %f, %f", tptz__RelativeMove->Speed->PanTilt->space, tptz__RelativeMove->Speed->PanTilt->x, tptz__RelativeMove->Speed->PanTilt->y);
    }
    if (tptz__RelativeMove->Speed != NULL && tptz__RelativeMove->Speed->Zoom != NULL) {
        LOG_INFO("%s, %f", tptz__RelativeMove->Speed->Zoom->space, tptz__RelativeMove->Speed->Zoom->x);
    }
    return 0;
}
/** Web service operation '__tptz__SendAuxiliaryCommand' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SendAuxiliaryCommand(struct soap* soap, struct _tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand, struct _tptz__SendAuxiliaryCommandResponse *tptz__SendAuxiliaryCommandResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__AbsoluteMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__AbsoluteMove(struct soap* soap, struct _tptz__AbsoluteMove *tptz__AbsoluteMove, struct _tptz__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__GeoMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GeoMove(struct soap* soap, struct _tptz__GeoMove *tptz__GeoMove, struct _tptz__GeoMoveResponse *tptz__GeoMoveResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__Stop' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__Stop(struct soap* soap, struct _tptz__Stop *tptz__Stop, struct _tptz__StopResponse *tptz__StopResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__GetPresetTours' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTours(struct soap* soap, struct _tptz__GetPresetTours *tptz__GetPresetTours, struct _tptz__GetPresetToursResponse *tptz__GetPresetToursResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__GetPresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTour(struct soap* soap, struct _tptz__GetPresetTour *tptz__GetPresetTour, struct _tptz__GetPresetTourResponse *tptz__GetPresetTourResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__GetPresetTourOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTourOptions(struct soap* soap, struct _tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, struct _tptz__GetPresetTourOptionsResponse *tptz__GetPresetTourOptionsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__CreatePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__CreatePresetTour(struct soap* soap, struct _tptz__CreatePresetTour *tptz__CreatePresetTour, struct _tptz__CreatePresetTourResponse *tptz__CreatePresetTourResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__ModifyPresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__ModifyPresetTour(struct soap* soap, struct _tptz__ModifyPresetTour *tptz__ModifyPresetTour, struct _tptz__ModifyPresetTourResponse *tptz__ModifyPresetTourResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__OperatePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__OperatePresetTour(struct soap* soap, struct _tptz__OperatePresetTour *tptz__OperatePresetTour, struct _tptz__OperatePresetTourResponse *tptz__OperatePresetTourResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__RemovePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePresetTour(struct soap* soap, struct _tptz__RemovePresetTour *tptz__RemovePresetTour, struct _tptz__RemovePresetTourResponse *tptz__RemovePresetTourResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__GetCompatibleConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetCompatibleConfigurations(struct soap* soap, struct _tptz__GetCompatibleConfigurations *tptz__GetCompatibleConfigurations, struct _tptz__GetCompatibleConfigurationsResponse *tptz__GetCompatibleConfigurationsResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tptz__MoveAndStartTracking' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__MoveAndStartTracking(struct soap* soap, struct _tptz__MoveAndStartTracking *tptz__MoveAndStartTracking, struct _tptz__MoveAndStartTrackingResponse *tptz__MoveAndStartTrackingResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
