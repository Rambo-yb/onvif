
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "onvif_event.h"

#include "list.h"
#include "check_common.h"

typedef struct {
	void* event_list;
	pthread_mutex_t list_mutex;
}OnvifEventMng;
static OnvifEventMng kOnvifEventMng = {.list_mutex = PTHREAD_MUTEX_INITIALIZER};

int OnvifEventInit() {
	kOnvifEventMng.event_list = ListCreate();
    CHECK_POINTER(kOnvifEventMng.event_list, return -1);

	return 0;
}

void OnvifEventUninit() {
	ListDestory(kOnvifEventMng.event_list);
}

int OnvifEventUpload(OnvifEventInfo* info) {
	CHECK_POINTER(info, return -1);
	CHECK_POINTER(kOnvifEventMng.event_list, return -1);

	OnvifEventMsg msg;
	memset(&msg, 0, sizeof(OnvifEventMsg));
	msg.type = info->type;
	msg.status = info->state;
	memcpy(&msg.msg, info->val, sizeof(msg.msg));

    pthread_mutex_lock(&kOnvifEventMng.list_mutex);
    ListPush(kOnvifEventMng.event_list, &msg, sizeof(OnvifEventMsg));
    pthread_mutex_unlock(&kOnvifEventMng.list_mutex);

	return 0;
}

int OnvifEventGetEvets(OnvifEventMsg* msg, long long timeout) {
	CHECK_POINTER(kOnvifEventMng.event_list, return -1);

	int cnt = 0;
	while((cnt++)*1000 < timeout) {
		pthread_mutex_lock(&kOnvifEventMng.list_mutex);
		if (ListSize(kOnvifEventMng.event_list) > 0) {
			memset(msg, 0, sizeof(OnvifEventMsg));
			ListPop(kOnvifEventMng.event_list, msg, sizeof(OnvifEventMsg));
			pthread_mutex_unlock(&kOnvifEventMng.list_mutex);
			return 0;
		}

		pthread_mutex_unlock(&kOnvifEventMng.list_mutex);
		usleep(1000*1000);
	}

	return -1;
}
