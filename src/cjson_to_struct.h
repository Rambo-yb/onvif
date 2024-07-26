#ifndef __CJSON_TO_STRUCT_H__
#define __CJSON_TO_STRUCT_H__

#include "cJSON.h"

int CjsonToStructVideoEncoder(cJSON* json, void* st);
int CjsonToStructAudioEncoder(cJSON* json, void* st);
int CjsonToStructPresets(cJSON* json, void* st);

#endif