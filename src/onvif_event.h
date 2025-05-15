#ifndef __ONVIF_EVENT_H__
#define __ONVIF_EVENT_H__

#include "onvif_common.h"

typedef struct {
	int type;
	int status;
	union {
		OnvifEventMonitioringProcessorUsage minitioring_processor_usage;
		OnvifEventMonitioringUploadStatus minitioring_upload_status;
		OnvifEventMonitioringDefinedLimitReached minitioring_defined_limit_reached;
		OnvifEventMonitioringOperatingTime minitioring_operation_time;

		OnvifEventVideoSource video_source;
		OnvifEventPtzPresets ptz_presets;
	}msg;
}OnvifEventMsg;

int OnvifEventInit();

void OnvifEventUninit();

int OnvifEventUpload(OnvifEventInfo* info);

int OnvifEventGetEvets(OnvifEventMsg* msg, long long timeout);

#endif