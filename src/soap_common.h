#ifndef __SOAP_COMMON_H__
#define __SOAP_COMMON_H__

#define SOAP_SET_NUMBER(s, p, l, v) \
    do { \
        p = soap_malloc(s, l); \
        *(p) = v; \
    }while(0)

#define SOAP_SET_RANGE(s, p, l, min, max) \
    do { \
        p = soap_malloc(s, l); \
        p->Min = min; \
        p->Max = max; \
    }while(0)

#define SOAP_SET_NUMBER_LIST(s, p, l, n, v) \
    do { \
        p = soap_malloc(s, l*n); \
        memset(p, 0, l*n); \
        for (int i = 0; i < n; i++) { \
            p[i] = v[i]; \
        } \
    }while(0)

#define SOAP_SET_RANGE_FROM_JSON(s, p, l, j, k) \
	do {	\
		cJSON* sub = cJSON_GetObjectItemCaseSensitive(j, k);	\
		if (sub != NULL) {	\
			int min = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(sub, "min"));	\
			int max = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(sub, "max"));	\
			SOAP_SET_RANGE(s, p, l, min, max);	\
		}	\
	}while(0)

#define SOAP_CJSON_GET_STRING(json, soap, key, param) \
    do { \
        cJSON* root = cJSON_GetObjectItemCaseSensitive(json, key); \
        if (root != NULL && cJSON_IsString(root)) { \
            param = soap_strdup(soap, cJSON_GetStringValue(root)); \
        } else { \
            LOG_WRN("%s not exist or not string!", key); \
            param = soap_strdup(soap, ""); \
        } \
    }while(0)

#define SOAP_CJSON_GET_NUMBER(json, soap, key, param) \
    do { \
        cJSON* root = cJSON_GetObjectItemCaseSensitive(json, key); \
        if (root != NULL && cJSON_IsNumber(root)) { \
            param = cJSON_GetNumberValue(root); \
        } else { \
            LOG_WRN("%s not exist or not number!", key); \
            param = 0; \
        } \
    }while(0)

#define SOAP_ERROR_REPLAY(soap, resp, code) \
    do { \
        soap_sender_fault(soap, resp, NULL); \
        return code; \
    } while (0)
    
#endif