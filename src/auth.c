#include <stdio.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include "wsseapi.h"
#include "auth.h"
#include "soapH.h"

typedef struct {
    char username[32];
    char password[64];
} User;

// 模拟用户数据库
static User kUser[] = {
    {"admin", "123456"},
};

static int AuthCompareUsername(char* username, const char* key) {
    return strcmp(username, key);
}

int AuthUser(struct soap *soap)
{
    const char *username = soap_wsse_get_Username(soap);
    if (username == NULL) {
        printf("username is null\r\n");
        return -1;
    }

    int result = 0;
    for (int i = 0; i < sizeof(kUser) / sizeof(User); i++) {
        if (!AuthCompareUsername(kUser[i].username, username) 
            && !soap_wsse_verify_Password(soap, kUser[i].password)) {
            result = 1;
            break;
        }
    }

    soap_wsse_delete_Security(soap);
    return result ? 0 : -1;
}
