#ifndef __ONVIF_COMMON_H__
#define __ONVIF_COMMON_H__

/*********** 设备信息 ***********/
typedef struct OnvifDevInfo {
    char menu_facturer[64];
    char module[64];
    char serial_num[64];
    char firmware_ver[64];
    char hardware_ver[64];
    char rtsp_url[128];
    char snap_url[128];
}OnvifDevInfo;

/*********** 设备配置 ***********/
typedef enum OnvifVideoEncodingType {
    ONVIF_VIDEO_ENCODING_JPEG,
    ONVIF_VIDEO_ENCODING_MPEG4,
    ONVIF_VIDEO_ENCODING_H264,
}OnvifVideoEncodingType;

typedef enum OnvifMpeg4ProfileType {
    ONVIF_MPEG4_SP,
    ONVIF_MPEG4_APS,
}OnvifMpeg4ProfileType;

typedef enum OnvifH264ProfileType {
    ONVIF_H264_BASELINE,
    ONVIF_H264_MAIN,
    ONVIF_H264_EXTENDED,
    ONVIF_H264_HIGH,
}OnvifH264ProfileType;

typedef struct OnvifVideoEncoder {
    OnvifVideoEncodingType encoding;
    int resolution_width;
    int resolution_height;
    int gov_length;
    int profile;    // encoding为MPEG4，使用enum OnvifMpeg4ProfileType; encoding为H264，使用enum OnvifH264ProfileType;
}OnvifVideoEncoder;

typedef enum OnvifAudioEncodingType {
    ONVIF_AUDIO_ENCODING_G711,
    ONVIF_AUDIO_ENCODING_G726,
    ONVIF_AUDIO_ENCODING_AAC,
}OnvifAudioEncodingType;

typedef struct OnvifAudioEncoder {
    int channels;
    OnvifAudioEncodingType encoding;
    int bitrate;
    int sample_rate;
}OnvifAudioEncoder;

typedef enum OnvifPresetCtrlType {
    ONVIF_PRESET_CTRL_ADD,
    ONVIF_PRESET_CTRL_DEL,
    ONVIF_PRESET_CTRL_SET_HOME,
}OnvifPresetCtrlType;

typedef struct OnvifPresets {
    OnvifPresetCtrlType type;   // 预置点设置类型，仅在设置配置时有效，获取配置时无实际意义
    int preset_num;
    char preset[8][64];
}OnvifPresets;

/*********** 设备控制 ***********/
typedef enum OnvifOperType{
    ONVIF_OPER_GET_CONFIG,
    ONVIF_OPER_SET_CONFIG,
    ONVIF_OPER_PTZ_CTRL,
}OnvifOperType;

typedef enum OnvifPtzCtrlType {
    ONVIF_PTZ_CTRL_UP,
    ONVIF_PTZ_CTRL_DOWN,
    ONVIF_PTZ_CTRL_LEFT,
    ONVIF_PTZ_CTRL_RIGHT,
    ONVIF_PTZ_CTRL_HOME,
    ONVIF_PTZ_CTRL_PRESET, // 转到预置点，回调携带PRESET_NAME
}OnvifPtzCtrlType;

/* 参数格式
 * ONVIF_OPER_GET_CONFIG, string CONFIG_NAME, CONFIG_STRUCT_OUT
 * ONVIF_OPER_SET_CONFIG, string CONFIG_NAME, CONFIG_STRUCT_IN
 * ONVIF_OPER_PTZ_CTRL, enum OnvifPtzCtrlType, NULL/PRESET_NAME
 */
typedef int (*OnvifOperCb)(OnvifOperType, void*, void*);

/*********** 事件上报 ***********/
typedef enum OnvifEventState {
    ONVIF_EVENT_START,
    ONVIF_EVENT_STOP,
}OnvifEventState;

typedef struct OnvifEventInfo {
    char event_type[64];
    long time;
    OnvifEventState state;
}OnvifEventInfo;

#endif