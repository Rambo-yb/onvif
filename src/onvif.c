#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "soapH.h"
#include "wsseapi.h"
#include "wsdd.nsmap"
#include "onvif.h"
#include "onvif_operation.h"
#include "log.h"

#define ONVIF_LIB_VERSION ("V1.0.0")
#define MULTICAST_ADDR ("239.255.255.250")
#define MULTICAST_PORT (3702)
#define WEB_SERVER_PORT (3333)
#define EVENT_MESSAGE_PORT (3334)

typedef struct {
    int web_port;
    char web_addr[16];
    pthread_t discorvery_id;
    pthread_t web_server_id;
    pthread_t event_msg_id;
}OnvifMng;
static OnvifMng kOnvifMng;

static void* OnvifDiscorveryProc(void* arg) {
    struct soap probe_soap;
    soap_init2(&probe_soap, SOAP_IO_UDP | SOAP_IO_FLUSH, SOAP_IO_UDP|SOAP_IO_FLUSH);
    probe_soap.bind_flags = SO_REUSEADDR;
    soap_set_namespaces(&probe_soap, namespaces);
    
    if(!soap_valid_socket(soap_bind(&probe_soap, NULL, MULTICAST_PORT, 10))) {
        soap_print_fault(&probe_soap, stderr);
        return NULL;
    }

    struct ip_mreq mcast;
    mcast.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    mcast.imr_interface.s_addr = htonl(INADDR_ANY);
    if(setsockopt(probe_soap.master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)) < 0) {
        printf("setsockopt error! error code = %d,err string = %s\n",errno,strerror(errno));
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
    struct soap ser_soap;
    soap_init2(&ser_soap, SOAP_ENC_MTOM | SOAP_ENC_MIME, 0);
    ser_soap.port = WEB_SERVER_PORT;
    ser_soap.bind_flags = SO_REUSEADDR;
    soap_set_namespaces(&ser_soap, namespaces);
    
    soap_register_plugin(&ser_soap, soap_wsse);

    if(!soap_valid_socket(soap_bind(&ser_soap, kOnvifMng.web_addr, WEB_SERVER_PORT, 10))) {
        soap_print_fault(&ser_soap, stderr);
        return NULL;
    }

    while(1) {
        if (!soap_valid_socket(soap_accept(&ser_soap))) {
			soap_print_fault(&ser_soap, stderr);
            break;
        }
        
        if( soap_serve(&ser_soap) != SOAP_OK) {
            soap_print_fault(&ser_soap, stderr);
        }

        soap_destroy(&ser_soap);
        soap_end(&ser_soap);
    }

    soap_done(&ser_soap);
    return NULL;
}

static void* OnvifEventMessageProc(void* arg) {
    struct soap ser_soap;
    soap_init2(&ser_soap, SOAP_ENC_MTOM | SOAP_ENC_MIME, 0);
    ser_soap.port = EVENT_MESSAGE_PORT;
    ser_soap.bind_flags = SO_REUSEADDR;
    soap_set_namespaces(&ser_soap, namespaces);
    
    soap_register_plugin(&ser_soap, soap_wsse);

    if(!soap_valid_socket(soap_bind(&ser_soap, kOnvifMng.web_addr, EVENT_MESSAGE_PORT, 10))) {
        soap_print_fault(&ser_soap, stderr);
        return NULL;
    }

    while(1) {
        if (!soap_valid_socket(soap_accept(&ser_soap))) {
			soap_print_fault(&ser_soap, stderr);
            break;
        }
        
        if( soap_serve(&ser_soap) != SOAP_OK) {
            soap_print_fault(&ser_soap, stderr);
        }

        soap_destroy(&ser_soap);
        soap_end(&ser_soap);
    }

    soap_done(&ser_soap);
    return NULL;
}

int OnvifInit(char* addr, OnvifDevInfo dev_info, OnvifOperCb func) {
    log_init("/tmp/onvif.log", 512*1024, 3);
    snprintf(kOnvifMng.web_addr, sizeof(kOnvifMng.web_addr), "%s", addr);

    OnvifOperationDeviceInfo conf_dev_info;
    conf_dev_info.web_server_port = WEB_SERVER_PORT;
    conf_dev_info.event_message_port = EVENT_MESSAGE_PORT;
    snprintf(conf_dev_info.device_addr, sizeof(conf_dev_info.device_addr), "%s", addr);
    memcpy(&conf_dev_info.dev_info, &dev_info, sizeof(OnvifDevInfo));
    OnvifOperationInit(conf_dev_info, func);


	pthread_create(&kOnvifMng.discorvery_id, NULL, OnvifDiscorveryProc, NULL);
	pthread_create(&kOnvifMng.web_server_id, NULL, OnvifWebServerProc, NULL);
	pthread_create(&kOnvifMng.event_msg_id, NULL, OnvifEventMessageProc, NULL);

    LOG_INFO("onvif init success! compile time:%s %s, ver:%s", __DATE__, __TIME__, ONVIF_LIB_VERSION);
    return 0;
}

int OnvifUnInit() {
    pthread_cancel(kOnvifMng.web_server_id);
    pthread_join(kOnvifMng.web_server_id, NULL);

    pthread_cancel(kOnvifMng.discorvery_id);
    pthread_join(kOnvifMng.discorvery_id, NULL);

    return 0;
}

void OnvifEventUplaod(OnvifEventInfo* info) {
    OnvifOperationEventUpload(info);
} 