#include <stdio.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include "wsseapi.h"
#include "auth.h"
#include "soapH.h"
#include "config.h"
#include "log.h"

static int AuthCompareUsername(char* username, const char* key) {
    return strcmp(username, key);
}

int AuthUser(struct soap *soap) {
    const char *username = soap_wsse_get_Username(soap);
    if (username == NULL) {
        LOG_ERR("username is null");
        return -1;
    }

    OnvifConfigUsersInfo* info = OnvifConfigGet("users_info");
    if (info == NULL) {
        LOG_ERR("device users info abnormal");
        return -1;
    }

    int result = 0;
    for (int i = 0; i < info->num; i++) {
        if (!AuthCompareUsername(info->user_info[i].username, username) 
            && !soap_wsse_verify_Password(soap, info->user_info[i].password)) {
            result = 1;
            break;
        }
    }

    soap_wsse_delete_Security(soap);
    return result ? 0 : -1;
}
