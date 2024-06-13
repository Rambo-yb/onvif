#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "soapH.h"
#include "wsseapi.h"
#include "wsdd.nsmap"
#include "onvif_oper.h"
#include "onvif.h"

#define MULTICAST_ADDR ("239.255.255.250")
#define MULTICAST_PORT (3702)

typedef struct {
    int web_port;
    char web_addr[16];
    pthread_t discorvery_id;
    pthread_t web_server_id;
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
    ser_soap.port = kOnvifMng.web_port;
    ser_soap.bind_flags = SO_REUSEADDR;
    soap_set_namespaces(&ser_soap, namespaces);
    
    soap_register_plugin(&ser_soap, soap_wsse);

    if(!soap_valid_socket(soap_bind(&ser_soap, kOnvifMng.web_addr, kOnvifMng.web_port, 10))) {
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

int OnvifInit(char* addr, int port, OnvifDevInfo dev_info) {
    kOnvifMng.web_port = port;
    snprintf(kOnvifMng.web_addr, sizeof(kOnvifMng.web_addr), "%s", addr);

    OnvifOperDeviceInfo oper_dev_info;
    oper_dev_info.web_server_port = port;
    snprintf(oper_dev_info.wen_server_addr, sizeof(oper_dev_info.wen_server_addr), "%s", addr);
    snprintf(oper_dev_info.menu_facturer, sizeof(oper_dev_info.menu_facturer), "%s", dev_info.menu_facturer);
    snprintf(oper_dev_info.module, sizeof(oper_dev_info.module), "%s", dev_info.module);
    snprintf(oper_dev_info.serial_num, sizeof(oper_dev_info.serial_num), "%s", dev_info.serial_num);
    snprintf(oper_dev_info.firmware_ver, sizeof(oper_dev_info.firmware_ver), "%s", dev_info.firmware_ver);
    snprintf(oper_dev_info.hardware_ver, sizeof(oper_dev_info.hardware_ver), "%s", dev_info.hardware_ver);
    OnvifOperSetDevInfo(&oper_dev_info);

	pthread_create(&kOnvifMng.discorvery_id, NULL, OnvifDiscorveryProc, NULL);
	pthread_create(&kOnvifMng.web_server_id, NULL, OnvifWebServerProc, NULL);

    return 0;
}

int OnvifUnInit() {
    pthread_cancel(kOnvifMng.web_server_id);
    pthread_join(kOnvifMng.web_server_id, NULL);

    pthread_cancel(kOnvifMng.discorvery_id);
    pthread_join(kOnvifMng.discorvery_id, NULL);

    return 0;
}

int OnvifSetDeviceInfo(OnvifDevInfo info) {
    OnvifOperDeviceInfo oper_dev_info;
    OnvifOperGetDevInfo(&oper_dev_info);
    snprintf(oper_dev_info.menu_facturer, sizeof(oper_dev_info.menu_facturer), "%s", info.menu_facturer);
    snprintf(oper_dev_info.module, sizeof(oper_dev_info.module), "%s", info.module);
    snprintf(oper_dev_info.serial_num, sizeof(oper_dev_info.serial_num), "%s", info.serial_num);
    snprintf(oper_dev_info.firmware_ver, sizeof(oper_dev_info.firmware_ver), "%s", info.firmware_ver);
    snprintf(oper_dev_info.hardware_ver, sizeof(oper_dev_info.hardware_ver), "%s", info.hardware_ver);
    OnvifOperSetDevInfo(&oper_dev_info);
    return 0;
}