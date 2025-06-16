#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "soapH.h"
#include "wsseapi.h"
#include "httpda.h"
#include "wsdd.nsmap"

#include "onvif.h"
#include "onvif_operation.h"
#include "onvif_conf.h"
#include "onvif_event.h"

#include "log.h"
#include "check_common.h"

#define ONVIF_LIB_VERSION ("V1.0.1")
#define ONVIF_DEFAULT_LOGS_PATH "/data/logs"
#define ONVIF_DEFAULT_CONF_PATH "/data/confs"

#define ONVIF_MULTICAST_ADDR ("239.255.255.250")
#define ONVIF_MULTICAST_PORT (3702)
#define ONVIF_WEB_SERVER_PORT (3333)
#define ONVIF_EVENT_MESSAGE_PORT (3334)

typedef struct {
	pthread_t discorvery_id;
	pthread_t web_server_id;
	pthread_t event_msg_id;
}OnvifMng;
static OnvifMng kOnvifMng = {};

static void* OnvifDiscorveryProc(void* arg) {
	struct soap probe_soap;
	soap_init2(&probe_soap, SOAP_IO_UDP | SOAP_IO_FLUSH, SOAP_IO_UDP|SOAP_IO_FLUSH);
	probe_soap.bind_flags = SO_REUSEADDR;
	soap_set_namespaces(&probe_soap, namespaces);
	
	if(!soap_valid_socket(soap_bind(&probe_soap, NULL, ONVIF_MULTICAST_PORT, 10))) {
		soap_print_fault(&probe_soap, stderr);
		return NULL;
	}

	struct ip_mreq mcast;
	mcast.imr_multiaddr.s_addr = inet_addr(ONVIF_MULTICAST_ADDR);
	mcast.imr_interface.s_addr = htonl(INADDR_ANY);
	if(setsockopt(probe_soap.master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)) < 0) {
		LOG_ERR("setsockopt error! error code = %d,err string = %s",errno,strerror(errno));
		return NULL;
	}

	while(1) {
		if (!soap_valid_socket(soap_accept(&probe_soap))) {
			soap_print_fault(&probe_soap, stderr);
			break;
		}
		
		if( soap_serve(&probe_soap) != SOAP_OK) {
			soap_print_fault(&probe_soap, stderr);
		}

		soap_destroy(&probe_soap);
		soap_end(&probe_soap);
	}

	soap_done(&probe_soap);
	return NULL;
}

static void* OnvifWebServerProc(void* arg) {
	struct soap *ser_soap = soap_new1(SOAP_XML_INDENT);
	ser_soap->port = ONVIF_WEB_SERVER_PORT;
	ser_soap->bind_flags = SO_REUSEADDR;
	soap_set_namespaces(ser_soap, namespaces);
	
	CHECK_EQ(soap_register_plugin(ser_soap, soap_wsse), 0, );
	CHECK_EQ(soap_register_plugin(ser_soap, http_da), 0, );

	if(!soap_valid_socket(soap_bind(ser_soap, NULL, ONVIF_WEB_SERVER_PORT, 10))) {
		soap_print_fault(ser_soap, stderr);
		return NULL;
	}

	while(1) {
		if (!soap_valid_socket(soap_accept(ser_soap))) {
			soap_print_fault(ser_soap, stderr);
			break;
		}

		if( soap_serve(ser_soap) != SOAP_OK) {
			soap_print_fault(ser_soap, stderr);
		}

		soap_destroy(ser_soap);
		soap_end(ser_soap);
	}

	soap_done(ser_soap);
	return NULL;
}

static void* OnvifEventMessageProc(void* arg) {
	struct soap *ser_soap = soap_new1(SOAP_XML_INDENT);
	ser_soap->port = ONVIF_EVENT_MESSAGE_PORT;
	ser_soap->bind_flags = SO_REUSEADDR;
	soap_set_namespaces(ser_soap, namespaces);
	
	CHECK_EQ(soap_register_plugin(ser_soap, soap_wsse), 0, );
	CHECK_EQ(soap_register_plugin(ser_soap, http_da), 0, );

	if(!soap_valid_socket(soap_bind(ser_soap, NULL, ONVIF_EVENT_MESSAGE_PORT, 10))) {
		soap_print_fault(ser_soap, stderr);
		return NULL;
	}

	while(1) {
		if (!soap_valid_socket(soap_accept(ser_soap))) {
			soap_print_fault(ser_soap, stderr);
			break;
		}
		
		if( soap_serve(ser_soap) != SOAP_OK) {
			soap_print_fault(ser_soap, stderr);
		}

		soap_destroy(ser_soap);
		soap_end(ser_soap);
	}

	soap_done(ser_soap);
	return NULL;
}

static int OnvifCreatePath(const char* path) {
	char _p[256] = {0};
	strncpy(_p, path, sizeof(_p));

	for(int i = 0; i < strlen(_p); i++) {
		if (_p[i] == '/') {
			_p[i] = '\0';
			if (strlen(_p) > 0 && access(_p, F_OK) != 0) {
				mkdir(_p, 0755);
			}
			_p[i] = '/';
		}
	}

	if (strlen(_p) > 0 && access(_p, F_OK) != 0) {
		mkdir(_p, 0755);
	}

	return 0;
}

int OnvifInit(OnvifInitialInfo* info) {
	char file_path[256] = {0};
	const char* log_path = (info == NULL || info->log_path == NULL) ? ONVIF_DEFAULT_LOGS_PATH : info->log_path;
	if (access(log_path, F_OK)) {
		OnvifCreatePath(log_path);
	}
	snprintf(file_path, sizeof(file_path), "%s/onvif.log", log_path);
	LogInit(file_path, 512*1024, 3, 5);

	OnvifConfInit((info == NULL || info->conf_path == NULL) ? ONVIF_DEFAULT_CONF_PATH : info->conf_path);
	OnvifEventInit();

	pthread_create(&kOnvifMng.discorvery_id, NULL, OnvifDiscorveryProc, NULL);
	pthread_create(&kOnvifMng.web_server_id, NULL, OnvifWebServerProc, NULL);
	pthread_create(&kOnvifMng.event_msg_id, NULL, OnvifEventMessageProc, NULL);

	usleep(2*1000*1000);
	LOG_INFO("onvif init success! compile time:%s %s, ver:%s", __DATE__, __TIME__, ONVIF_LIB_VERSION);
	return 0;
}

int OnvifUnInit() {
	pthread_cancel(kOnvifMng.web_server_id);
	pthread_join(kOnvifMng.web_server_id, NULL);

	pthread_cancel(kOnvifMng.discorvery_id);
	pthread_join(kOnvifMng.discorvery_id, NULL);

	pthread_cancel(kOnvifMng.event_msg_id);
	pthread_join(kOnvifMng.event_msg_id, NULL);
	
	return 0;
}

void OnvifOperationRegister(OnvifOperationType type, void* cb) {
	OnvifOperationRegisterCb(type, cb);
}

void OnvifEventUplaod(OnvifEventInfo* info) {
	CHECK_POINTER(info, return );
	CHECK_POINTER(info->val, return );

	OnvifEventUpload(info);
} 