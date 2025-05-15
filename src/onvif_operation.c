#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "onvif_operation.h"
#include "check_common.h"

typedef struct {
	OnvifOperationSystemCb system_cb;
	OnvifOperationGetConfigCb get_config_cb;
	OnvifOperationSetConfigCb set_config_cb;
	OnvifOperationControlCb control_cb;
}OnvifOperationMng;
static OnvifOperationMng kOnvifOperationMng = {0};

void OnvifOperationRegisterCb(OnvifOperationType type, void* cb) {
	switch(type) {
	case ONVIF_OPERATION_SYSTEM_REQUEST:
		kOnvifOperationMng.system_cb = (OnvifOperationSystemCb)cb;
		break;
	case ONVIF_OPERATION_GET_CONFIG:
		kOnvifOperationMng.get_config_cb = (OnvifOperationGetConfigCb)cb;
		break;
	case ONVIF_OPERATION_SET_CONFIG:
		kOnvifOperationMng.set_config_cb = (OnvifOperationSetConfigCb)cb;
		break;
	case ONVIF_OPERATION_CONTORL_REQUEST:
		kOnvifOperationMng.control_cb = (OnvifOperationControlCb)cb;
		break;
	default:
		LOG_ERR("unknown type:%x", type);
		break;
	}
}

int OnvifOperationSystemRequest(OnvifSystem* sys) {
	CHECK_POINTER(kOnvifOperationMng.system_cb, return -1);
	return kOnvifOperationMng.system_cb(sys);
}

int OnvifOperationGetConfig(int type, void* st, int size) {
	CHECK_POINTER(kOnvifOperationMng.get_config_cb, return -1);
	return kOnvifOperationMng.get_config_cb(type, st, size);
}

int OnvifOperationSetConfig(int type, void* st, int size) {
	CHECK_POINTER(kOnvifOperationMng.set_config_cb, return -1);
	return kOnvifOperationMng.set_config_cb(type, st, size);
}

int OnvifOperationContorlRequest(int type, int chn, void* st, int size) {
	CHECK_POINTER(kOnvifOperationMng.control_cb, return -1);
	return kOnvifOperationMng.control_cb(type, chn, st, size);
}