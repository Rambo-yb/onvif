#include <stdio.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include "wsseapi.h"
#include "auth.h"
#include "soapH.h"
#include "config.h"
#include "log.h"
#include "cjson_common.h"
#include "check_common.h"

static int AuthCompareUsername(char* username, const char* key) {
    return strcmp(username, key);
}

int AuthUser(struct soap *soap) {
    const char *username = soap_wsse_get_Username(soap);
    if (username == NULL) {
        LOG_ERR("username is null");
        return -1;
    }

    cJSON* users_json = OnvifGetConfig("users");
    CHECK_POINTER(users_json, return -1);
    CHECK_BOOL(cJSON_IsArray(users_json), return -1);

    int result = 0;
    for(int i = 0; i < cJSON_GetArraySize(users_json); i++) {
        cJSON* item = cJSON_GetArrayItem(users_json, i);
        if (item == NULL || !cJSON_IsObject(item)) {
            continue;
        }

        if (!AuthCompareUsername(cJSON_GetStringValue(cJSON_GetObjectItem(item, "username")), username) 
            && !soap_wsse_verify_Password(soap, cJSON_GetStringValue(cJSON_GetObjectItem(item, "password")))) {
            result = 1;
            break;
        }
    }

    soap_wsse_delete_Security(soap);
    return result ? 0 : -1;
}
