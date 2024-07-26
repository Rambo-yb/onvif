#ifndef __STRUCT_TO_CJSON_H__
#define __STRUCT_TO_CJSON_H__

#include "cJSON.h"

int StructToCjsonVideoEncoder(void* st, cJSON** enc_json, cJSON** src_json);
int StructToCjsonAudioEncoder(void* st, cJSON** enc_json, cJSON** src_json);
int StructToCjsonPresets(void* st, cJSON** json);

int StructToCjsonEventInfo(void* st, cJSON** json);

#endif