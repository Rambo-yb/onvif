#include <stdio.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include "wsseapi.h"
#include "wsse2api.h"
#include "httpda.h"
#include "onvif_auth.h"
#include "onvif_conf.h"
#include "soapH.h"
#include "onvif_operation.h"
#include "log.h"
#include "cjson_common.h"
#include "check_common.h"

static int OnvifAuthUserByWsUsernameToken(struct soap *soap) {
	const char *username = soap_wsse_get_Username(soap);
	if (username == NULL) {
		LOG_ERR("username is null");
		return -1;
	}

	cJSON* users_json = OnvifConfGetConfig("users");
	CHECK_POINTER(users_json, return -1);
	CHECK_BOOL(cJSON_IsArray(users_json), return -1);

	int result = 0;
	for(int i = 0; i < cJSON_GetArraySize(users_json); i++) {
		cJSON* item = cJSON_GetArrayItem(users_json, i);
		if (item == NULL || !cJSON_IsObject(item)) {
			continue;
		}

		if (strcmp(username, cJSON_GetStringValue(cJSON_GetObjectItem(item, "username"))) == 0
			&& !soap_wsse_verify_Password(soap, cJSON_GetStringValue(cJSON_GetObjectItem(item, "password")))) {
			result = 1;
			break;
		}
	}

	soap_wsse_delete_Security(soap);
	return result ? 0 : -1;
}

static int OnvifAuthUserByHttpDigest(struct soap *soap) {
	cJSON* users_json = OnvifConfGetConfig("users");
	CHECK_POINTER(users_json, return -1);
	CHECK_BOOL(cJSON_IsArray(users_json), return -1);

	int result = 0;
	for(int i = 0; i < cJSON_GetArraySize(users_json); i++) {
		cJSON* item = cJSON_GetArrayItem(users_json, i);
		if (item == NULL || !cJSON_IsObject(item)) {
			continue;
		}

		if (strcmp(soap->userid, cJSON_GetStringValue(cJSON_GetObjectItem(item, "username"))) == 0
			&& strcmp(soap->authrealm, "cam") == 0
			&& !http_da_verify_post(soap, cJSON_GetStringValue(cJSON_GetObjectItem(item, "password")))) {
			result = 1;
			break;
		}
	}

	soap->authrealm = "cam";
	return result ? 0 : -1;
}

int OnvifAuthUser(struct soap *soap) {
	if (soap_wsse_get_Username(soap) != NULL) {
		return OnvifAuthUserByWsUsernameToken(soap);
	} else if (soap->authrealm != NULL && soap->userid != NULL) {
		return OnvifAuthUserByHttpDigest(soap);
	} else {
		soap->authrealm = "cam";
		return -1;
	}
}
