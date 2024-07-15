#ifndef __CJSON_COMMON_H__
#define __CJSON_COMMON_H__

#include "check_common.h"
#include "cJSON.h"

#define CJSON_GET_STRING(json, key, val, size, err_opt) \
    do {    \
        cJSON* root = cJSON_GetObjectItemCaseSensitive(json, key); \
        CHECK_POINTER(root, err_opt); \
        CHECK_BOOL(cJSON_IsString(root), err_opt); \
        snprintf(val, size, "%s", cJSON_GetStringValue(root));  \
    }while(0);

#define CJSON_GET_NUMBER(json, key, val, size, err_opt) \
    do {    \
        cJSON* root = cJSON_GetObjectItemCaseSensitive(json, key); \
        CHECK_POINTER(root, err_opt); \
        CHECK_BOOL(cJSON_IsNumber(root), err_opt); \
        val = cJSON_GetNumberValue(root); \
    }while(0);

#define CJSON_GET_NUMBER_LIST(json, key, val_list, list_len, err_opt) \
    do { \
        cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, key); \
        CHECK_POINTER(arr, err_opt); \
        CHECK_BOOL(cJSON_IsArray(arr), err_opt); \
        for (int ii = 0; ii < cJSON_GetArraySize(arr) && ii < list_len; ii++) { \
            val_list[ii] = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, ii)); \
        } \
    }while(0);

#define CJSON_SET_STRING(json, key, val, err_opt)   \
    do {    \
        cJSON* root = cJSON_CreateString(val);  \
        CHECK_POINTER(root, err_opt); \
        if (!cJSON_AddItemToObject(json, key, root)) {  \
            cJSON_free(root);   \
            err_opt; \
        }   \
    } while (0);

#define CJSON_SET_NUMBER(json, key, val, err_opt)   \
    do {    \
        cJSON* root = cJSON_CreateNumber(val);  \
        CHECK_POINTER(root, err_opt); \
        if (!cJSON_AddItemToObject(json, key, root)) {  \
            cJSON_free(root);   \
            err_opt; \
        }   \
    } while (0);

#define CJSON_SET_NUMBER_LIST(json, key, val_list, list_len, err_opt) \
    do { \
        cJSON* arr = cJSON_CreateArray(); \
        CHECK_POINTER(arr, err_opt); \
        for(int ii = 0; ii < list_len; ii++) { \
            if (!cJSON_AddItemToArray(arr, cJSON_CreateNumber(val_list[ii]))) { \
                cJSON_free(arr); \
                err_opt; \
            } \
        } \
        if (!cJSON_AddItemToObject(json, key, arr)) { \
            cJSON_free(arr); \
            err_opt; \
        } \
    } while(0);

#endif