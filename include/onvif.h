#ifndef __ONVIF_H__
#define __ONVIF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "onvif_common.h"

typedef struct {
	const char* log_path;			// 日志路径，【NULL:"/data/logs"】
	const char* conf_path;			// 配置路径，【NULL:"/data/confs"】
	const char* download_path;		// 下载路径，【NULL:"/data/download"】
}OnvifInitialInfo;

int OnvifInit(OnvifInitialInfo* info);

int OnvifUnInit();

void OnvifOperationRegister(OnvifOperationType type, void* cb);

void OnvifEventUplaod(OnvifEventInfo* info);

#ifdef __cplusplus
}
#endif

#endif