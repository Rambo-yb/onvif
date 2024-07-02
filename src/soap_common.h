#ifndef __SOAP_COMMON_H__
#define __SOAP_COMMON_H__

#define SOAP_SET_NUMBER(s, p, l, v) \
    do { \
        p = soap_malloc(s, l); \
        *(p) = v; \
    }while(0);

#endif