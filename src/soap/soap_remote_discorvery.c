#include "soapStub.h"
#include "soapH.h"
#include "wsaapi.h"
#include "wsddapi.h"
#include "onvif_operation.h"
#include "onvif_conf.h"
#include "log.h"
#include "check_common.h"
#include "cjson_common.h"

/** Web service one-way operation 'SOAP_ENV__Fault' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 SOAP_ENV__Fault(struct soap* soap, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail) {
    // LOG_WRN("unrealized");
    return 0;
}
/** Web service one-way operation '__wsdd__Hello' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Hello(struct soap* soap, struct wsdd__HelloType *wsdd__Hello) {
    // LOG_WRN("unrealized");
    return 0;
}
/** Web service one-way operation '__wsdd__Bye' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Bye(struct soap* soap, struct wsdd__ByeType *wsdd__Bye) {
    // LOG_WRN("unrealized");
    return 0;
}

/** Web service one-way operation '__wsdd__Probe' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Probe(struct soap* soap, struct wsdd__ProbeType *wsdd__Probe) {
	OnvifConfigNetworkTcpIp network_tcpip;
	memset(&network_tcpip, 0, sizeof(OnvifConfigNetworkTcpIp));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

	char scopes_message[1024*2] = {0};

	cJSON* scopes_json = OnvifConfGetConfig("scopes");
	CHECK_POINTER(scopes_json, return 500);
	CHECK_BOOL(cJSON_IsArray(scopes_json), cJSON_free(scopes_json);return 500);

	for(int i = 0; i < cJSON_GetArraySize(scopes_json); i++) {
		cJSON* item = cJSON_GetArrayItem(scopes_json, i);
		if (item == NULL || !cJSON_IsObject(item)) {
			continue;
		}

		strcpy(scopes_message + strlen(scopes_message), cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(item, "item")));
		strcpy(scopes_message + strlen(scopes_message), "\r\n");
	}

	cJSON_free(scopes_json);
        
    static char uuid[64] = {0};
    if(strlen(uuid) == 0) {
        snprintf(uuid, sizeof(uuid), "%s", soap_strdup(soap, soap_rand_uuid(soap, "urn:uuid:")));
        LOG_INFO("%s", uuid);
    }
    // verify scropes

    // response ProbeMatches
    struct wsdd__ProbeMatchType* probe_match_type = (struct wsdd__ProbeMatchType*)soap_malloc(soap, sizeof(struct wsdd__ProbeMatchType));
    soap_default_wsdd__ProbeMatchType(soap, probe_match_type);

    char buff[256] = {0};
    snprintf(buff, sizeof(buff), "http://%s:%d/onvif/device_service", network_tcpip.ipv4_addr, 3333);
    probe_match_type->XAddrs = soap_strdup(soap, buff);
    if( wsdd__Probe->Types && strlen(wsdd__Probe->Types) )
        probe_match_type->Types  = soap_strdup(soap, wsdd__Probe->Types);
    else
        probe_match_type->Types  = soap_strdup(soap, "dn:NetworkVideoTransmitter tds:Device");

    probe_match_type->MetadataVersion = 1;

    // Build Scopes Message
    struct wsdd__ScopesType* scopes_type = (struct wsdd__ScopesType *)soap_malloc(soap, sizeof(struct wsdd__ScopesType));
    soap_default_wsdd__ScopesType(soap, scopes_type);
    scopes_type->MatchBy = NULL;
    scopes_type->__item  = soap_strdup(soap, scopes_message);
    probe_match_type->Scopes = scopes_type;

    probe_match_type->wsa__EndpointReference.Address = soap_strdup(soap, uuid);

    // Build SOAP Header
    struct wsa__Relationship* relation_ship = (struct wsa__Relationship*)soap_malloc(soap, sizeof(struct wsa__Relationship));
    soap_default__wsa__RelatesTo(soap, relation_ship);
    relation_ship->__item = soap->header->wsa__MessageID;
    soap->header->wsa__RelatesTo = relation_ship;
    soap->header->wsa__Action      = soap_strdup(soap, "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches");
    soap->header->wsa__To          = soap_strdup(soap, "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous");

    struct wsdd__ProbeMatchesType wsdd__ProbeMatches = {.__sizeProbeMatch = 1, .ProbeMatch = probe_match_type};
    soap_send___wsdd__ProbeMatches(soap, "http://", NULL, &wsdd__ProbeMatches);

	return SOAP_OK;
}
/** Web service one-way operation '__wsdd__ProbeMatches' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ProbeMatches(struct soap* soap, struct wsdd__ProbeMatchesType *wsdd__ProbeMatches) {
    // LOG_WRN("unrealized");
    return 0;
}
/** Web service one-way operation '__wsdd__Resolve' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Resolve(struct soap* soap, struct wsdd__ResolveType *wsdd__Resolve) {
    // LOG_WRN("unrealized");
    return 0;
}
/** Web service one-way operation '__wsdd__ResolveMatches' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ResolveMatches(struct soap* soap, struct wsdd__ResolveMatchesType *wsdd__ResolveMatches) {
    // LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tdn__Hello' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Hello(struct soap* soap, struct wsdd__HelloType tdn__Hello, struct wsdd__ResolveType *tdn__HelloResponse) {
    // LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tdn__Bye' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Bye(struct soap* soap, struct wsdd__ByeType tdn__Bye, struct wsdd__ResolveType *tdn__ByeResponse) {
    // LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tdn__Probe' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Probe(struct soap* soap, struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType *tdn__ProbeResponse) {
    // LOG_WRN("unrealized");
    return 0;
}