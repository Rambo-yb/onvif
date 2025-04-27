#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include "cjson_common.h"

#include "onvif_conf.h"

#define PROTOCOL_CONFIG_FILE "protocol.conf"
#define ONVIF_CONFIG_FILE "onvif.conf"

typedef struct {
	char conf_path[256];

	cJSON* protocol_conf;
	cJSON* onvif_conf;
	pthread_t pthread_conf_monitor;
	pthread_mutex_t mutex;

}OnvifConfMng;
static OnvifConfMng kOnvifConfMng = {.mutex = PTHREAD_MUTEX_INITIALIZER};

static cJSON* OnvifConfLoadFile(char* file) {
    FILE* fp = fopen(file, "r");
    CHECK_POINTER(fp, return NULL);

    CHECK_LT(fseek(fp, 0, SEEK_END), 0, fclose(fp);return NULL);
    int file_len = ftell(fp);
    CHECK_LT(fseek(fp, 0, SEEK_SET), 0, fclose(fp);return NULL);

    char* file_data = (char*)malloc(file_len+1);
    CHECK_POINTER(file_data, fclose(fp);return NULL);

    CHECK_EQ(fread(file_data, 1, file_len+1, fp), file_len, free(file_data);fclose(fp);return NULL);

    cJSON* json = cJSON_Parse(file_data);
    CHECK_POINTER(json, free(file_data);fclose(fp);return NULL);

    free(file_data);
    fclose(fp);
    return json;
}

static int OnvifConfSaveFile(char* file, cJSON* json) {
	char* conf = cJSON_Print(json);
	CHECK_POINTER(conf, return -1);

	FILE* fp = fopen(file, "w+");
	CHECK_POINTER(fp, free(conf);return -1);

	CHECK_EQ(fwrite(conf, 1, strlen(conf), fp), strlen(conf), fclose(fp);free(conf);return -1);

	fflush(fp);
	fclose(fp);
	free(conf);
	return 0;
}

static void OnvifConfigUserDefault(cJSON* json) {
	cJSON* user_json = cJSON_CreateObject();
	CHECK_POINTER(user_json, return );

	CJSON_SET_STRING(user_json, "username", "admin", free(user_json); return );
	CJSON_SET_STRING(user_json, "password", "123456", free(user_json); return );
	CJSON_SET_NUMBER(user_json, "user_type", 0, free(user_json); return );

	cJSON* sub_json = cJSON_CreateArray();
	CHECK_POINTER(sub_json, return );
	CHECK_BOOL(cJSON_AddItemToArray(sub_json, user_json), cJSON_free(user_json); cJSON_free(sub_json); return );

	CHECK_BOOL(cJSON_AddItemToObject(json, "users", sub_json), cJSON_free(sub_json); return );
}

static void OnvifConfDefaultProtocol() {
	cJSON* json = cJSON_CreateObject();
	CHECK_POINTER(json, return );

	OnvifConfigUserDefault(json);

	pthread_mutex_lock(&kOnvifConfMng.mutex);
	kOnvifConfMng.protocol_conf = json;

	char path[512] = {0};
	snprintf(path, sizeof(path), "%s/%s", kOnvifConfMng.conf_path, ONVIF_CONFIG_FILE);
	OnvifConfSaveFile(path, json);
	pthread_mutex_unlock(&kOnvifConfMng.mutex);
	return ;
}

static void OnvifConfigScopesDefault(cJSON* json) {
	cJSON* scope = cJSON_CreateObject();
	CHECK_POINTER(scope, return );

	CJSON_SET_NUMBER(scope, "def", 0, free(scope); return );
	CJSON_SET_STRING(scope, "item", "onvif://www.onvif.org/Profile/Streaming", free(scope); return );

	cJSON* scopes = cJSON_CreateArray();
	CHECK_POINTER(scopes, free(scope); return );
	CHECK_BOOL(cJSON_AddItemToArray(scopes, scope), cJSON_free(scope); cJSON_free(scopes); return );

	CHECK_BOOL(cJSON_AddItemToObject(json, "scopes", scopes), cJSON_free(scopes); return );
}

#define CJSON_SET_RANGE(json, key, min, max, err) \
	do {	\
		cJSON* sub = cJSON_CreateObject();	\
		CHECK_POINTER(sub, err);	\
		CJSON_SET_NUMBER(sub, "min", min, free(sub); err);	\
		CJSON_SET_NUMBER(sub, "max", max, free(sub); err);	\
		CHECK_BOOL(cJSON_AddItemToObject(json, key, sub), cJSON_free(sub); err);	\
	} while (0);
	

static void OnvifConfigEncoderOptionsDefault(cJSON* json) {
	cJSON* h264 = cJSON_CreateObject();
	CHECK_POINTER(h264, return );
	CJSON_SET_RANGE(h264, "gov_length", 1, 400, free(h264); return );
	CJSON_SET_RANGE(h264, "frame_rate", 15, 30, free(h264); return );
	CJSON_SET_RANGE(h264, "encoder_interval", 1, 1, free(h264); return );
	CJSON_SET_RANGE(h264, "bit_rate", 2*1024, 16*1024, free(h264); return );

	cJSON* resolution  = cJSON_CreateObject();
	CHECK_POINTER(resolution, free(h264); return );
	CJSON_SET_NUMBER(resolution, "width", 640, free(resolution); free(h264); return );
	CJSON_SET_NUMBER(resolution, "height", 512, free(resolution); free(h264); return );

	cJSON* resolutions = cJSON_CreateArray();
	CHECK_POINTER(resolutions, free(resolution); free(h264); return );
	CHECK_BOOL(cJSON_AddItemToArray(resolutions, resolution), free(resolution); cJSON_free(resolutions); free(h264); return );
	CHECK_BOOL(cJSON_AddItemToObject(h264, "resolutions", resolutions), cJSON_free(resolutions); free(h264); return );

	cJSON* option = cJSON_CreateObject();
	CHECK_POINTER(option, free(h264); return );
	CHECK_BOOL(cJSON_AddItemToObject(option, "h264", h264), cJSON_free(h264); free(option); return );
	CJSON_SET_RANGE(option, "quality", 1, 5, free(option); return );

	cJSON* option_stream = cJSON_CreateArray();
	CHECK_POINTER(option_stream, free(option); return );
	CHECK_BOOL(cJSON_AddItemToArray(option_stream, option), cJSON_free(option_stream); free(option); return );

	cJSON* option_chn = cJSON_CreateArray();
	CHECK_POINTER(option_chn, free(option_stream); return );
	CHECK_BOOL(cJSON_AddItemToArray(option_chn, option_stream), cJSON_free(option_chn); free(option_stream); return );

	CHECK_BOOL(cJSON_AddItemToObject(json, "video_encoder_options", option_chn), free(option_chn); return );
}

static void OnvifConfigImagingOptionsDefault(cJSON* json) {
	cJSON* imaging = cJSON_CreateObject();
	CHECK_POINTER(imaging, return );

	CJSON_SET_RANGE(imaging, "brightness", 0, 100, free(imaging); return );
	CJSON_SET_RANGE(imaging, "saturation", 0, 100, free(imaging); return );
	CJSON_SET_RANGE(imaging, "contrast", 0, 100, free(imaging); return );
	CJSON_SET_RANGE(imaging, "sharpness", 0, 100, free(imaging); return );

	cJSON* imagings = cJSON_CreateArray();
	CHECK_POINTER(imagings, free(imaging); return );
	CHECK_BOOL(cJSON_AddItemToArray(imagings, imaging), cJSON_free(imaging); cJSON_free(imagings); return );

	CHECK_BOOL(cJSON_AddItemToObject(json, "imaging_options", imagings), cJSON_free(imagings); return );
}

static void OnvifConfDefaultOnvif() {
	cJSON* json = cJSON_CreateObject();
	CHECK_POINTER(json, return );

	OnvifConfigScopesDefault(json);
	OnvifConfigEncoderOptionsDefault(json);
	OnvifConfigImagingOptionsDefault(json);

	pthread_mutex_lock(&kOnvifConfMng.mutex);
	kOnvifConfMng.onvif_conf = json;

	char path[512] = {0};
	snprintf(path, sizeof(path), "%s/%s", kOnvifConfMng.conf_path, ONVIF_CONFIG_FILE);
	OnvifConfSaveFile(path, json);
	pthread_mutex_unlock(&kOnvifConfMng.mutex);
	return ;
}

static void* OnvifConfMonitorProc(void* arg) {
	char protocol_file[512] = {0};
	snprintf(protocol_file, sizeof(protocol_file), "%s/%s", kOnvifConfMng.conf_path, PROTOCOL_CONFIG_FILE);

	char onvif_file[512] = {0};
	snprintf(onvif_file, sizeof(onvif_file), "%s/%s", kOnvifConfMng.conf_path, ONVIF_CONFIG_FILE);

	struct stat pre_protocol_file_stat = {0};
	stat(protocol_file, &pre_protocol_file_stat);

	struct stat pre_onvif_file_stat = {0};
	stat(onvif_file, &pre_onvif_file_stat);

	while (1) {
		struct stat cur_file_stat = {0};
		if (stat(protocol_file, &cur_file_stat) == -1) {
			sleep(1);
			continue;
		}

		if (cur_file_stat.st_mtime != pre_protocol_file_stat.st_mtime) {
			cJSON* json = OnvifConfLoadFile(protocol_file);
			CHECK_POINTER(json, sleep(1);continue);

			pthread_mutex_lock(&kOnvifConfMng.mutex);
			if (kOnvifConfMng.protocol_conf != NULL) {
				cJSON_free(kOnvifConfMng.protocol_conf);
			}
			kOnvifConfMng.protocol_conf = json;
			pthread_mutex_unlock(&kOnvifConfMng.mutex);
			memcpy(&pre_protocol_file_stat, &cur_file_stat, sizeof(struct stat));
		}

		if (stat(onvif_file, &cur_file_stat) == -1) {
			sleep(1);
			continue;
		}

		if (cur_file_stat.st_mtime != pre_onvif_file_stat.st_mtime) {
			cJSON* json = OnvifConfLoadFile(onvif_file);
			CHECK_POINTER(json, sleep(1);continue);

			pthread_mutex_lock(&kOnvifConfMng.mutex);
			if (kOnvifConfMng.onvif_conf != NULL) {
				cJSON_free(kOnvifConfMng.onvif_conf);
			}
			kOnvifConfMng.onvif_conf = json;
			pthread_mutex_unlock(&kOnvifConfMng.mutex);
			memcpy(&pre_onvif_file_stat, &cur_file_stat, sizeof(struct stat));
		}

		usleep(1000*1000);
	}
	
	return NULL;
}

int OnvifConfInit(const char* conf_path) {
	strcpy(kOnvifConfMng.conf_path, conf_path);

	char protocol_file[512] = {0};
	snprintf(protocol_file, sizeof(protocol_file), "%s/%s", conf_path, PROTOCOL_CONFIG_FILE);
	kOnvifConfMng.protocol_conf = OnvifConfLoadFile(protocol_file);
	if (kOnvifConfMng.protocol_conf == NULL) {
        LOG_WRN("load protocol.conf fail, use memory config !");
        OnvifConfDefaultProtocol();
    }

	char onvif_file[512] = {0};
	snprintf(onvif_file, sizeof(onvif_file), "%s/%s", conf_path, ONVIF_CONFIG_FILE);
	kOnvifConfMng.onvif_conf = OnvifConfLoadFile(onvif_file);
	if (kOnvifConfMng.onvif_conf == NULL) {
        LOG_WRN("load onvif.conf fail, use memory config !");
        OnvifConfDefaultOnvif();
    }

	int ret = pthread_create(&kOnvifConfMng.pthread_conf_monitor, NULL, OnvifConfMonitorProc, NULL);
	CHECK_EQ(ret, 0, return -1);

	return 0;
}

int OnvifConfUninit() {
    if (kOnvifConfMng.pthread_conf_monitor) {
        pthread_cancel(kOnvifConfMng.pthread_conf_monitor);
        pthread_join(kOnvifConfMng.pthread_conf_monitor, NULL);
    }

	if (kOnvifConfMng.protocol_conf) {
		cJSON_free(kOnvifConfMng.protocol_conf);
	}

	if (kOnvifConfMng.onvif_conf) {
		cJSON_free(kOnvifConfMng.onvif_conf);
	}

	return 0;
}

void* OnvifConfGetConfig(const char* key) {
	cJSON* json = kOnvifConfMng.onvif_conf;
	if(strcmp(key, "users") == 0) {
		json = kOnvifConfMng.protocol_conf;
	}
	
	cJSON* sub = cJSON_GetObjectItemCaseSensitive(json, key);
	CHECK_POINTER(sub, return NULL);

	return (void*)cJSON_Duplicate(sub, cJSON_True);
}

int OnvifConfSetConfig(const char* key, void* param) {
	cJSON* sub = cJSON_Duplicate((cJSON*)param, cJSON_True);
	CHECK_POINTER(sub, return -1);

	cJSON* json = kOnvifConfMng.onvif_conf;
	char* file = ONVIF_CONFIG_FILE;
	if(strcmp(key, "users") == 0) {
		json = kOnvifConfMng.protocol_conf;
		file = PROTOCOL_CONFIG_FILE;
	}
	CHECK_BOOL(cJSON_ReplaceItemInObjectCaseSensitive(json, key, sub), return -1); 

	char path[512] = {0};
	snprintf(path, sizeof(path), "%s/%s", kOnvifConfMng.conf_path, file);
	OnvifConfSaveFile(path, json);

	return 0;
}