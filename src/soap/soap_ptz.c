#include "soapStub.h"
#include "onvif_operation.h"
#include "check_common.h"
#include "cjson_common.h"
#include "soap_common.h"
#include "onvif_auth.h"

void GetPtzServiceCapabilities(struct soap* soap, struct tptz__Capabilities* capabilities) {
	SOAP_SET_NUMBER(soap, capabilities->EFlip, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->Reverse, sizeof(enum xsd__boolean), xsd__boolean__true_);
}

/** Web service operation '__tptz__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetServiceCapabilities(struct soap* soap, struct _tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, struct _tptz__GetServiceCapabilitiesResponse *tptz__GetServiceCapabilitiesResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	tptz__GetServiceCapabilitiesResponse->Capabilities = (struct tptz__Capabilities*)soap_malloc(soap, sizeof(struct tptz__Capabilities));
	memset(tptz__GetServiceCapabilitiesResponse->Capabilities, 0, sizeof(struct tptz__Capabilities));
	GetPtzServiceCapabilities(soap, tptz__GetServiceCapabilitiesResponse->Capabilities);
	
	return 0;
}
/** Web service operation '__tptz__GetConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurations(struct soap* soap, struct _tptz__GetConfigurations *tptz__GetConfigurations, struct _tptz__GetConfigurationsResponse *tptz__GetConfigurationsResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tptz__GetPresets' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresets(struct soap* soap, struct _tptz__GetPresets *tptz__GetPresets, struct _tptz__GetPresetsResponse *tptz__GetPresetsResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigPtzPresets ptz_presets;
	memset(&ptz_presets, 0, sizeof(OnvifConfigPtzPresets));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_PTZ_PRESET, &ptz_presets, sizeof(OnvifConfigPtzPresets));
	CHECK_LT(ret, 0, return 500);

	tptz__GetPresetsResponse->__sizePreset = ptz_presets.preset_num;
	tptz__GetPresetsResponse->Preset = (struct tt__PTZPreset*)soap_malloc(soap, sizeof(struct tt__PTZPreset)*tptz__GetPresetsResponse->__sizePreset);
	memset(tptz__GetPresetsResponse->Preset, 0, sizeof(struct tt__PTZPreset)*tptz__GetPresetsResponse->__sizePreset);
	for(int i = 0; i < ptz_presets.preset_num; i++) {
		tptz__GetPresetsResponse->Preset[i].token = soap_strdup(soap, ptz_presets.preset[i].name);
		tptz__GetPresetsResponse->Preset[i].Name = soap_strdup(soap, ptz_presets.preset[i].name);
	}

	return 0;
}
/** Web service operation '__tptz__SetPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetPreset(struct soap* soap, struct _tptz__SetPreset *tptz__SetPreset, struct _tptz__SetPresetResponse *tptz__SetPresetResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigPtzPresets ptz_presets;
	memset(&ptz_presets, 0, sizeof(OnvifConfigPtzPresets));
	ptz_presets.type = ONVIF_CONFIG_PTZ_PRESET_ADD;
	ptz_presets.preset_num = 1;
	snprintf(ptz_presets.preset[0].name, sizeof(ptz_presets.preset[0].name), "%s", tptz__SetPreset->PresetToken);
	int ret = OnvifOperationSetConfig(ONVIF_CONFIG_PTZ_PRESET, &ptz_presets, sizeof(OnvifConfigPtzPresets));
	CHECK_LT(ret, 0, return 500);

	return 0;
}
/** Web service operation '__tptz__RemovePreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePreset(struct soap* soap, struct _tptz__RemovePreset *tptz__RemovePreset, struct _tptz__RemovePresetResponse *tptz__RemovePresetResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigPtzPresets ptz_presets;
	memset(&ptz_presets, 0, sizeof(OnvifConfigPtzPresets));
	ptz_presets.type = ONVIF_CONFIG_PTZ_PRESET_DEL;
	ptz_presets.preset_num = 1;
	snprintf(ptz_presets.preset[0].name, sizeof(ptz_presets.preset[0].name), "%s", tptz__RemovePreset->PresetToken);
	int ret = OnvifOperationSetConfig(ONVIF_CONFIG_PTZ_PRESET, &ptz_presets, sizeof(OnvifConfigPtzPresets));
	CHECK_LT(ret, 0, return 500);

	return 0;
}
/** Web service operation '__tptz__GotoPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoPreset(struct soap* soap, struct _tptz__GotoPreset *tptz__GotoPreset, struct _tptz__GotoPresetResponse *tptz__GotoPresetResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	sscanf(tptz__GotoPreset->ProfileToken, "%*[^_]_%d\n", &code);

	OnvifContorlPtzCtrl ptz_ctrl;
	memset(&ptz_ctrl, 0, sizeof(OnvifContorlPtzCtrl));
	ptz_ctrl.mode = ONVIF_CONTORL_PTZ_CTRL_PRESET;
	snprintf(ptz_ctrl.preset_name, sizeof(ptz_ctrl.preset_name), "%s", tptz__GotoPreset->PresetToken);
	int ret = OnvifOperationContorlRequest(ONVIF_CONTORL_PTZ, code/100, &ptz_ctrl, sizeof(OnvifContorlPtzCtrl));
	CHECK_LT(ret, 0, return 500);

	return 0;
}
/** Web service operation '__tptz__GetStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetStatus(struct soap* soap, struct _tptz__GetStatus *tptz__GetStatus, struct _tptz__GetStatusResponse *tptz__GetStatusResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	tptz__GetStatusResponse->PTZStatus = (struct tt__PTZStatus*)soap_malloc(soap, sizeof(struct tt__PTZStatus));
	memset(tptz__GetStatusResponse->PTZStatus, 0, sizeof(struct tt__PTZStatus));
	tptz__GetStatusResponse->PTZStatus->Position = (struct tt__PTZVector*)soap_malloc(soap, sizeof(struct tt__PTZVector));
	tptz__GetStatusResponse->PTZStatus->Position->PanTilt = (struct tt__Vector2D*)soap_malloc(soap, sizeof(struct tt__Vector2D));
	tptz__GetStatusResponse->PTZStatus->Position->PanTilt->space = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
	tptz__GetStatusResponse->PTZStatus->Position->PanTilt->x = 0;
	tptz__GetStatusResponse->PTZStatus->Position->PanTilt->y = 0;
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

static void GetPtzSpace(struct soap* soap, struct tt__PTZSpaces* ptz_space) {
	ptz_space->__sizeAbsolutePanTiltPositionSpace = 2;
	ptz_space->AbsolutePanTiltPositionSpace = (struct tt__Space2DDescription*)soap_malloc(soap, sizeof(struct tt__Space2DDescription)*ptz_space->__sizeAbsolutePanTiltPositionSpace);
	memset(ptz_space->AbsolutePanTiltPositionSpace, 0, sizeof(struct tt__Space2DDescription)*ptz_space->__sizeAbsolutePanTiltPositionSpace);
	ptz_space->AbsolutePanTiltPositionSpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
	SOAP_SET_RANGE(soap, ptz_space->AbsolutePanTiltPositionSpace[0].XRange, sizeof(struct tt__FloatRange), -1, 1);
	SOAP_SET_RANGE(soap, ptz_space->AbsolutePanTiltPositionSpace[0].YRange, sizeof(struct tt__FloatRange), -1, 1);
	ptz_space->AbsolutePanTiltPositionSpace[1].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");
	SOAP_SET_RANGE(soap, ptz_space->AbsolutePanTiltPositionSpace[1].XRange, sizeof(struct tt__FloatRange), -1, 1);
	SOAP_SET_RANGE(soap, ptz_space->AbsolutePanTiltPositionSpace[1].YRange, sizeof(struct tt__FloatRange), -1, 1);

	ptz_space->__sizeAbsoluteZoomPositionSpace = 2;
	ptz_space->AbsoluteZoomPositionSpace = (struct tt__Space1DDescription*)soap_malloc(soap, sizeof(struct tt__Space1DDescription)*ptz_space->__sizeAbsoluteZoomPositionSpace);
	memset(ptz_space->AbsoluteZoomPositionSpace, 0, sizeof(struct tt__Space1DDescription)*ptz_space->__sizeAbsoluteZoomPositionSpace);
	ptz_space->AbsoluteZoomPositionSpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
	SOAP_SET_RANGE(soap, ptz_space->AbsoluteZoomPositionSpace[0].XRange, sizeof(struct tt__FloatRange), 0, 1);
	ptz_space->AbsoluteZoomPositionSpace[1].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");
	SOAP_SET_RANGE(soap, ptz_space->AbsoluteZoomPositionSpace[1].XRange, sizeof(struct tt__FloatRange), 0, 1);

	ptz_space->__sizeRelativePanTiltTranslationSpace = 2;
	ptz_space->RelativePanTiltTranslationSpace = (struct tt__Space2DDescription*)soap_malloc(soap, sizeof(struct tt__Space2DDescription)*ptz_space->__sizeRelativePanTiltTranslationSpace);
	memset(ptz_space->RelativePanTiltTranslationSpace, 0, sizeof(struct tt__Space2DDescription)*ptz_space->__sizeRelativePanTiltTranslationSpace);
	ptz_space->RelativePanTiltTranslationSpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");
	SOAP_SET_RANGE(soap, ptz_space->RelativePanTiltTranslationSpace[0].XRange, sizeof(struct tt__FloatRange), -1, 1);
	SOAP_SET_RANGE(soap, ptz_space->RelativePanTiltTranslationSpace[0].YRange, sizeof(struct tt__FloatRange), -1, 1);
	ptz_space->RelativePanTiltTranslationSpace[1].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationSpaceFov");
	SOAP_SET_RANGE(soap, ptz_space->RelativePanTiltTranslationSpace[1].XRange, sizeof(struct tt__FloatRange), -1, 1);
	SOAP_SET_RANGE(soap, ptz_space->RelativePanTiltTranslationSpace[1].YRange, sizeof(struct tt__FloatRange), -1, 1);

	ptz_space->__sizeRelativeZoomTranslationSpace = 1;
	ptz_space->RelativeZoomTranslationSpace = (struct tt__Space1DDescription*)soap_malloc(soap, sizeof(struct tt__Space1DDescription)*ptz_space->__sizeAbsoluteZoomPositionSpace);
	memset(ptz_space->RelativeZoomTranslationSpace, 0, sizeof(struct tt__Space1DDescription)*ptz_space->__sizeAbsoluteZoomPositionSpace);
	ptz_space->RelativeZoomTranslationSpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");
	SOAP_SET_RANGE(soap, ptz_space->RelativeZoomTranslationSpace[0].XRange, sizeof(struct tt__FloatRange), -1, 1);

	ptz_space->__sizeContinuousPanTiltVelocitySpace = 1;
	ptz_space->ContinuousPanTiltVelocitySpace = (struct tt__Space2DDescription*)soap_malloc(soap, sizeof(struct tt__Space2DDescription)*ptz_space->__sizeContinuousPanTiltVelocitySpace);
	memset(ptz_space->ContinuousPanTiltVelocitySpace, 0, sizeof(struct tt__Space2DDescription)*ptz_space->__sizeContinuousPanTiltVelocitySpace);
	ptz_space->ContinuousPanTiltVelocitySpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace");
	SOAP_SET_RANGE(soap, ptz_space->ContinuousPanTiltVelocitySpace[0].XRange, sizeof(struct tt__FloatRange), -1, 1);
	SOAP_SET_RANGE(soap, ptz_space->ContinuousPanTiltVelocitySpace[0].YRange, sizeof(struct tt__FloatRange), -1, 1);

	ptz_space->__sizeContinuousZoomVelocitySpace = 1;
	ptz_space->ContinuousZoomVelocitySpace = (struct tt__Space1DDescription*)soap_malloc(soap, sizeof(struct tt__Space1DDescription)*ptz_space->__sizeContinuousZoomVelocitySpace);
	memset(ptz_space->ContinuousZoomVelocitySpace, 0, sizeof(struct tt__Space1DDescription)*ptz_space->__sizeContinuousZoomVelocitySpace);
	ptz_space->ContinuousZoomVelocitySpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace");
	SOAP_SET_RANGE(soap, ptz_space->ContinuousZoomVelocitySpace[0].XRange, sizeof(struct tt__FloatRange), -1, 1);

	ptz_space->__sizePanTiltSpeedSpace = 1;
	ptz_space->PanTiltSpeedSpace = (struct tt__Space1DDescription*)soap_malloc(soap, sizeof(struct tt__Space1DDescription)*ptz_space->__sizePanTiltSpeedSpace);
	memset(ptz_space->PanTiltSpeedSpace, 0, sizeof(struct tt__Space1DDescription)*ptz_space->__sizePanTiltSpeedSpace);
	ptz_space->PanTiltSpeedSpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");
	SOAP_SET_RANGE(soap, ptz_space->PanTiltSpeedSpace[0].XRange, sizeof(struct tt__FloatRange), 0, 1);
	
	ptz_space->__sizeZoomSpeedSpace = 1;
	ptz_space->ZoomSpeedSpace = (struct tt__Space1DDescription*)soap_malloc(soap, sizeof(struct tt__Space1DDescription)*ptz_space->__sizeZoomSpeedSpace);
	memset(ptz_space->ZoomSpeedSpace, 0, sizeof(struct tt__Space1DDescription)*ptz_space->__sizeZoomSpeedSpace);
	ptz_space->ZoomSpeedSpace[0].URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");
	SOAP_SET_RANGE(soap, ptz_space->ZoomSpeedSpace[0].XRange, sizeof(struct tt__FloatRange), 0, 1);
}

static void GetNode(struct soap* soap, struct tt__PTZNode* ptz_node) {
	ptz_node->token = soap_strdup(soap, "PTZNodeToken");
	ptz_node->Name = soap_strdup(soap, "PTZNode");
	ptz_node->SupportedPTZSpaces = (struct tt__PTZSpaces*)soap_malloc(soap, sizeof(struct tt__PTZSpaces));
	memset(ptz_node->SupportedPTZSpaces, 0, sizeof(struct tt__PTZSpaces));
	GetPtzSpace(soap, ptz_node->SupportedPTZSpaces);

	ptz_node->MaximumNumberOfPresets = 255;
	ptz_node->HomeSupported = xsd__boolean__false_;

	ptz_node->__sizeAuxiliaryCommands = 6;
	ptz_node->AuxiliaryCommands = (char**)soap_malloc(soap, sizeof(char*)*ptz_node->__sizeAuxiliaryCommands);
	ptz_node->AuxiliaryCommands[0] = soap_strdup(soap, "tt:Wiper|On");
	ptz_node->AuxiliaryCommands[1] = soap_strdup(soap, "tt:Wiper|Off");
	ptz_node->AuxiliaryCommands[2] = soap_strdup(soap, "tt:IRLamp|On");
	ptz_node->AuxiliaryCommands[3] = soap_strdup(soap, "tt:IRLamp|Off");
	ptz_node->AuxiliaryCommands[4] = soap_strdup(soap, "tt:Wiper|On");
	ptz_node->AuxiliaryCommands[5] = soap_strdup(soap, "tt:Wiper|Off");

	ptz_node->Extension = (struct tt__PTZNodeExtension*)soap_malloc(soap, sizeof(struct tt__PTZNodeExtension));
	memset(ptz_node->Extension, 0, sizeof(struct tt__PTZNodeExtension));

	ptz_node->Extension->SupportedPresetTour = (struct tt__PTZPresetTourSupported*)soap_malloc(soap, sizeof(struct tt__PTZPresetTourSupported));
	memset(ptz_node->Extension->SupportedPresetTour, 0, sizeof(struct tt__PTZPresetTourSupported));

	ptz_node->Extension->SupportedPresetTour->MaximumNumberOfPresetTours = 8;
	ptz_node->Extension->SupportedPresetTour->__sizePTZPresetTourOperation = 2;
	ptz_node->Extension->SupportedPresetTour->PTZPresetTourOperation = (enum tt__PTZPresetTourOperation*)soap_malloc(soap, sizeof(enum tt__PTZPresetTourOperation));
	memset(ptz_node->Extension->SupportedPresetTour->PTZPresetTourOperation, 0, sizeof(enum tt__PTZPresetTourOperation));
	ptz_node->Extension->SupportedPresetTour->PTZPresetTourOperation[0] = tt__PTZPresetTourOperation__Start;
	ptz_node->Extension->SupportedPresetTour->PTZPresetTourOperation[1] = tt__PTZPresetTourOperation__Stop;
}
/** Web service operation '__tptz__GetNodes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNodes(struct soap* soap, struct _tptz__GetNodes *tptz__GetNodes, struct _tptz__GetNodesResponse *tptz__GetNodesResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

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
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

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
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);
	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions = (struct tt__PTZConfigurationOptions*)soap_malloc(soap, sizeof(struct tt__PTZConfigurationOptions));
	memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions, 0, sizeof(struct tt__PTZConfigurationOptions));

	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces = (struct tt__PTZSpaces*)soap_malloc(soap, sizeof(struct tt__PTZSpaces));
	memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces, 0, sizeof(struct tt__PTZSpaces));
	GetPtzSpace(soap, tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces);

	SOAP_SET_RANGE(soap, tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->PTZTimeout, sizeof(struct tt__DurationRange), 1, 600);

	return 0;
}
/** Web service operation '__tptz__GotoHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoHomePosition(struct soap* soap, struct _tptz__GotoHomePosition *tptz__GotoHomePosition, struct _tptz__GotoHomePositionResponse *tptz__GotoHomePositionResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	sscanf(tptz__GotoHomePosition->ProfileToken, "%*[^_]_%d\n", &code);

	OnvifContorlPtzCtrl ptz_ctrl;
	memset(&ptz_ctrl, 0, sizeof(OnvifContorlPtzCtrl));
	ptz_ctrl.mode = ONVIF_CONTORL_PTZ_CTRL_HOME;
	int ret = OnvifOperationContorlRequest(ONVIF_CONTORL_PTZ, code/100, &ptz_ctrl, sizeof(OnvifContorlPtzCtrl));
	CHECK_LT(ret, 0, return 500);

	return 0;
}
/** Web service operation '__tptz__SetHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetHomePosition(struct soap* soap, struct _tptz__SetHomePosition *tptz__SetHomePosition, struct _tptz__SetHomePositionResponse *tptz__SetHomePositionResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigPtzPresets ptz_presets;
	memset(&ptz_presets, 0, sizeof(OnvifConfigPtzPresets));
	ptz_presets.type = ONVIF_CONFIG_PTZ_PRESET_HOME;
	ptz_presets.preset_num = 1;
	snprintf(ptz_presets.preset[0].name, sizeof(ptz_presets.preset[0].name), "preset_home");
	int ret = OnvifOperationSetConfig(ONVIF_CONFIG_PTZ_PRESET, &ptz_presets, sizeof(OnvifConfigPtzPresets));
	CHECK_LT(ret, 0, return 500);

	return 0;
}
/** Web service operation '__tptz__ContinuousMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__ContinuousMove(struct soap* soap, struct _tptz__ContinuousMove *tptz__ContinuousMove, struct _tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	sscanf(tptz__ContinuousMove->ProfileToken, "%*[^_]_%d\n", &code);

	if (tptz__ContinuousMove->Velocity->PanTilt != NULL) {
		OnvifContorlPtzCtrl ptz_ctrl;
		memset(&ptz_ctrl, 0, sizeof(OnvifContorlPtzCtrl));
		if (tptz__ContinuousMove->Velocity->PanTilt->x < 0) {
			ptz_ctrl.mode = ONVIF_CONTORL_PTZ_CTRL_LEFT;
			OnvifOperationContorlRequest(ONVIF_CONTORL_PTZ, code/100, &ptz_ctrl, sizeof(OnvifContorlPtzCtrl));
		} else if (tptz__ContinuousMove->Velocity->PanTilt->x > 0) {
			ptz_ctrl.mode = ONVIF_CONTORL_PTZ_CTRL_RIGHT;
			OnvifOperationContorlRequest(ONVIF_CONTORL_PTZ, code/100, &ptz_ctrl, sizeof(OnvifContorlPtzCtrl));
		}
		if (tptz__ContinuousMove->Velocity->PanTilt->y < 0) {
			ptz_ctrl.mode = ONVIF_CONTORL_PTZ_CTRL_DOWN;
			OnvifOperationContorlRequest(ONVIF_CONTORL_PTZ, code/100, &ptz_ctrl, sizeof(OnvifContorlPtzCtrl));
		} else if (tptz__ContinuousMove->Velocity->PanTilt->y > 0) {
			ptz_ctrl.mode = ONVIF_CONTORL_PTZ_CTRL_UP;
			OnvifOperationContorlRequest(ONVIF_CONTORL_PTZ, code/100, &ptz_ctrl, sizeof(OnvifContorlPtzCtrl));
		}
	}
	if (tptz__ContinuousMove->Velocity->Zoom != NULL) {
		LOG_INFO("zoo:x:%lf", tptz__ContinuousMove->Velocity->Zoom->x);
	}

	return 0;
}
/** Web service operation '__tptz__RelativeMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RelativeMove(struct soap* soap, struct _tptz__RelativeMove *tptz__RelativeMove, struct _tptz__RelativeMoveResponse *tptz__RelativeMoveResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	sscanf(tptz__RelativeMove->ProfileToken, "%*[^_]_%d\n", &code);

	if (tptz__RelativeMove->Translation != NULL && tptz__RelativeMove->Translation->PanTilt != NULL) {
		OnvifContorlPtzCtrl ptz_ctrl;
		memset(&ptz_ctrl, 0, sizeof(OnvifContorlPtzCtrl));
		if (tptz__RelativeMove->Translation->PanTilt->x < 0) {
			ptz_ctrl.mode = ONVIF_CONTORL_PTZ_CTRL_LEFT;
			OnvifOperationContorlRequest(ONVIF_CONTORL_PTZ, code/100, &ptz_ctrl, sizeof(OnvifContorlPtzCtrl));
		} else if (tptz__RelativeMove->Translation->PanTilt->x > 0) {
			ptz_ctrl.mode = ONVIF_CONTORL_PTZ_CTRL_RIGHT;
			OnvifOperationContorlRequest(ONVIF_CONTORL_PTZ, code/100, &ptz_ctrl, sizeof(OnvifContorlPtzCtrl));
		}
		if (tptz__RelativeMove->Translation->PanTilt->y < 0) {
			ptz_ctrl.mode = ONVIF_CONTORL_PTZ_CTRL_DOWN;
			OnvifOperationContorlRequest(ONVIF_CONTORL_PTZ, code/100, &ptz_ctrl, sizeof(OnvifContorlPtzCtrl));
		} else if (tptz__RelativeMove->Translation->PanTilt->y > 0) {
			ptz_ctrl.mode = ONVIF_CONTORL_PTZ_CTRL_UP;
			OnvifOperationContorlRequest(ONVIF_CONTORL_PTZ, code/100, &ptz_ctrl, sizeof(OnvifContorlPtzCtrl));
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
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	int code = 0;
	sscanf(tptz__Stop->ProfileToken, "%*[^_]_%d\n", &code);

	OnvifContorlPtzCtrl ptz_ctrl;
	memset(&ptz_ctrl, 0, sizeof(OnvifContorlPtzCtrl));
	ptz_ctrl.mode = ONVIF_CONTORL_PTZ_CTRL_STOP;
	OnvifOperationContorlRequest(ONVIF_CONTORL_PTZ, code/100, &ptz_ctrl, sizeof(OnvifContorlPtzCtrl));

	// TODO: 处理zoom stop

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
