#ifndef __ONVIF_COMMON_H__
#define __ONVIF_COMMON_H__

#include <stdbool.h>

/*********** 系统请求 ***********/
#define ONVIF_SYSTEM_DEVICE_INFO_MAX (32)		// 设备信息长度
typedef struct {
	char soft_version[ONVIF_SYSTEM_DEVICE_INFO_MAX];
	char hard_version[ONVIF_SYSTEM_DEVICE_INFO_MAX];
	char serial_number[ONVIF_SYSTEM_DEVICE_INFO_MAX];
	char device_type[ONVIF_SYSTEM_DEVICE_INFO_MAX];
}OnvifSystemDeviceInfo;

#define ONVIF_SYSTEM_URL_MAX (128)			// 码流地址长度
typedef struct {
	char main_stream[ONVIF_SYSTEM_URL_MAX];
	char sub_stream[ONVIF_SYSTEM_URL_MAX];
	char third_stream[ONVIF_SYSTEM_URL_MAX];
}OnvifSystemRtspUrl;


/*********** 设备配置 ***********/
typedef struct {
	char time_zone[16];
	int timing_type;
}OnvifConfigSystemTimeInfo;


typedef enum {
	ONVIF_CONFIG_CAMERA_VIDEO_STREAM_MAIN,
	ONVIF_CONFIG_CAMERA_VIDEO_STREAM_SUB,
	ONVIF_CONFIG_CAMERA_VIDEO_STREAM_THIRD,
}OnvifConfigCameraVideoStreamType;

typedef enum {
	ONVIF_CONFIG_CAMERA_VIDEO_BITRATE_CBR,
	ONVIF_CONFIG_CAMERA_VIDEO_BITRATE_VBR,
}OnvifConfigCameraVideoBitRateType;

typedef enum {
	ONVIF_CONFIG_CAMERA_VIDEO_QUALITY_LOW,
	ONVIF_CONFIG_CAMERA_VIDEO_QUALITY_MID,
	ONVIF_CONFIG_CAMERA_VIDEO_QUALITY_HIGH,
}OnvifConfigCameraVideoQualityType;

typedef enum {
	ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_H264,
	ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_H265,
}OnvifConfigCameraVideoEncodeType;

typedef struct {
	int width;
	int height;
}OnvifConfigCameraVideoResolutionInfo;

typedef struct {
	int stream_type;
	OnvifConfigCameraVideoResolutionInfo resolution;
	int bitrate_type;
	int quality;
	int fps;
	int bitrate_max;
	int encode_type;
	int gop;
}OnvifConfigCameraVideoEncodeInfo;

#define ONVIF_CONFIG_CAMERA_MAX_NUM (4)		// 最大摄像头数量
#define ONVIF_CONFIG_STREAM_MAX_NUM (3)		// 单个摄像头最大流数量

typedef struct {
	int deal_cam;
	int deal_stream;

	int cam_num;
	int stream_num[ONVIF_CONFIG_CAMERA_MAX_NUM];
	OnvifConfigCameraVideoEncodeInfo encode_info[ONVIF_CONFIG_CAMERA_MAX_NUM][ONVIF_CONFIG_STREAM_MAX_NUM];
}OnvifConfigCameraVideoEncodeInfos;

typedef enum {
	ONVIF_CONFIG_CAMERA_AUDIO_ENCODE_AAC,
	ONVIF_CONFIG_CAMERA_AUDIO_ENCODE_G711,
}OnvifConfigCameraAudioEncodeType;

typedef struct {
	int encode_type;
	int sample_rate;
	int bitrate;
	int channel;
}OnvifConfigCameraAudioEncodeInfo;

typedef struct {
	float brightness;
	float saturation;
	float contrast;
	float sharpness;
}OnvifConfigCameraImageInfo;

typedef struct {
	int deal_cam;
	int num;
	OnvifConfigCameraImageInfo image_info[ONVIF_CONFIG_CAMERA_MAX_NUM];
}OnvifConfigCameraImageInfos;

#define ONVIF_CONFIG_OSD_INFO_STRING_OVERLAY_MAX (8)	// 字符叠加最大个数
typedef struct {
	float x;
	float y;
}OnvifConfigPoint;

typedef struct {
	int show;				// 显示
	char value[64];			// 字符串
	OnvifConfigPoint pos;	// 位置
}OnvifConfigCameraStringOverlay;

typedef struct {
	char color[8];		// 颜色，rgb888字符串
	int size;			// 字体大小
	int flicker;		// 闪烁
	int transparency;	// 透明
}OnvifConfigCameraOsdAttribute;

typedef struct {
	int show_camera_name;				// 显示摄像头名
	char camera_name[64];				// 摄像头名
	OnvifConfigPoint camera_name_pos;	// 摄像头名位置
	
	int show_date;				// 显示日期
	OnvifConfigPoint date_pos;	// 日期位置

	OnvifConfigCameraStringOverlay string_overlay[ONVIF_CONFIG_OSD_INFO_STRING_OVERLAY_MAX];	// 字符叠加

	OnvifConfigCameraOsdAttribute osd_attribute;
}OnvifConfigCameraOsdInfo;

typedef struct {
	int deal_num;	// 处理数组中的某项，【-1：表示全数组】
	int num;		// 数组数
	OnvifConfigCameraOsdInfo osd_info[ONVIF_CONFIG_CAMERA_MAX_NUM];
}OnvifConfigCameraOsdInfos;


typedef struct {
    bool dhcp_enable;
    bool ipv4_enable;
    char ipv4_addr[16];
    char ipv4_mask[16];
    char ipv4_gateway[16];
    bool ipv6_enable;
    char ipv6_addr[128];
    char ipv6_gateway[128];
    char mac[18];
    char dns[16];
    char alt_dns[16];
}OnvifConfigNetworkTcpIp;

typedef struct {
	char server_addr[256];	// 服务器地址
	int port;				// 端口
	int interval;			// 间隔
}OnvifConfigNetworkNtpInfo;


#define ONVIF_PRESET_NAME_MAX_SIZE (64)
#define ONVIF_CONFIG_PRESET_MAX_NUM (256)

typedef struct {
	char name[ONVIF_PRESET_NAME_MAX_SIZE];	// 预置点名
	int num;		// 预置点号
}OnvifConfigPtzPreset;

typedef enum {
	ONVIF_CONFIG_PTZ_PRESET_ADD,
	ONVIF_CONFIG_PTZ_PRESET_DEL,
	ONVIF_CONFIG_PTZ_PRESET_MODIFY,
	ONVIF_CONFIG_PTZ_PRESET_HOME,
}OnvifConfigPtzPresetType;

typedef struct {
	OnvifConfigPtzPresetType type;
	int preset_num;	// 数组数
	OnvifConfigPtzPreset preset[ONVIF_CONFIG_PRESET_MAX_NUM];
}OnvifConfigPtzPresets;


/*********** 控制请求 ***********/
typedef enum {
    ONVIF_CONTORL_PTZ_CTRL_UP,
    ONVIF_CONTORL_PTZ_CTRL_DOWN,
    ONVIF_CONTORL_PTZ_CTRL_LEFT,
    ONVIF_CONTORL_PTZ_CTRL_RIGHT,
    ONVIF_CONTORL_PTZ_CTRL_HOME,
    ONVIF_CONTORL_PTZ_CTRL_PRESET, // 转到预置点，回调携带PRESET_NAME
    ONVIF_CONTORL_PTZ_CTRL_STOP,
}OnvifContorlPtzCtrlMode;

typedef struct {
    int mode;   // enum OnvifContorlPtzCtrlMode   
    char preset_name[ONVIF_PRESET_NAME_MAX_SIZE];  // 预置点名
}OnvifContorlPtzCtrl;


/*********** 系统请求回调 ***********/
typedef enum {
	ONVIF_SYSTEM_REBOOT = 0x21000,
	ONVIF_SYSTEM_RESET,
	ONVIF_SYSTEM_FORMAT,
	ONVIF_SYSTEM_DEVICE_INFO,
	ONVIF_SYSTEM_SET_TIME,
	ONVIF_SYSTEM_GET_TIME,
	ONVIF_SYSTEM_RTSP_INFO,
}OnvifSystemType;
typedef struct {
	int type;
	int cam;

	union {
		int time;
	}in;

	union {
		OnvifSystemDeviceInfo device_info;
		OnvifSystemRtspUrl rtsp_url;
		int time;
	}out;
}OnvifSystem;
typedef int (*OnvifOperationSystemCb)(OnvifSystem*);

/*********** 配置获取/设置回调 ***********/
typedef enum {
	ONVIF_CONFIG_SYSTEM_TIME_INFO = 0x22000,

	ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_INFO,
	ONVIF_CONFIG_CAMERA_AUDEO_ENCODE_INFO,
	ONVIF_CONFIG_CAMERA_IMAGE_INFO,
	ONVIF_CONFIG_CAMERA_OSD_INFO,
	
	ONVIF_CONFIG_NETWORK_TCP_IP,
	ONVIF_CONFIG_NETWORK_NTP_INFO,

	ONVIF_CONFIG_PTZ_PRESET,
}NetworkConfigType;
typedef int (*OnvifOperationGetConfigCb)(int , void* , int );
typedef int (*OnvifOperationSetConfigCb)(int , void* , int );

/*********** 控制请求回调 ***********/
typedef enum {
	ONVIF_CONTORL_PTZ = 0x23000,
}OnvifContorlType;
typedef int (*OnvifOperationControlCb)(int, int, void*, int);

/*********** 回调枚举 ***********/
typedef enum {
    ONVIF_OPERATION_SYSTEM_REQUEST = 0x20000,	// OnvifOperationSystemCb
    ONVIF_OPERATION_GET_CONFIG,					// OnvifOperationGetConfigCb
    ONVIF_OPERATION_SET_CONFIG, 				// OnvifOperationSetConfigCb
    ONVIF_OPERATION_CONTORL_REQUEST,			// OnvifOperationControlCb
}OnvifOperationType;


/*********** 事件上报 ***********/
typedef enum {
	ONVIF_MONITIORING_PROCESSOR_USAGE,					// CPU占用率, OnvifEventMonitioringProcessorUsage
	ONVIF_MONITIORING_UPLOAD_STATUS,					// 升级状态, OnvifEventMonitioringUploadStatus
	ONVIF_MONITIORING_DEFINED_LIMIT_REACHED,			// 达到设备运行时间限制, OnvifEventMonitioringDefinedLimitReached
	ONVIF_MONITIORING_LAST_RESET,						// 上次恢复出厂时间, OnvifEventMonitioringOperatingTime
	ONVIF_MONITIORING_LAST_REBOOT,						// 上次重启时间, OnvifEventMonitioringOperatingTime
	ONVIF_MONITIORING_LAST_CLOCK_SYNC,					// 上次时钟同步时间, OnvifEventMonitioringOperatingTime

	ONVIF_VIDEO_SOURCE_TOO_BLURRY,						// 失焦, OnvifEventVideoSource
	ONVIF_VIDEO_SOURCE_TOO_DARK,						// 画面过暗, OnvifEventVideoSource
	ONVIF_VIDEO_SOURCE_TOO_BRIGHT,						// 画面过亮, OnvifEventVideoSource
	ONVIF_VIDEO_SOURCE_SCENE_CHANGE,					// 视频篡改, OnvifEventVideoSource
	ONVIF_VIDEO_SOURCE_SIGNAL_LOSS,						// 视频丢失(模拟输入), OnvifEventVideoSource
	ONVIF_VIDEO_SOURCE_MOTION_ALARM,					// 移动检测, OnvifEventVideoSource

	ONVIF_PTZ_PRESETS_INVOKED,							// 执行, OnvifEventPtzPresets
	ONVIF_PTZ_PRESETS_REACHED,							// 到达, OnvifEventPtzPresets
	ONVIF_PTZ_PRESETS_ABORTED,							// 异常, OnvifEventPtzPresets
	ONVIF_PTZ_PRESETS_LEFT,								// 丢弃, OnvifEventPtzPresets
}OnvifEventType;

typedef enum {
    ONVIF_EVENT_INITIALIZED,	// 初始
    ONVIF_EVENT_DELETED,		// 删除
    ONVIF_EVENT_CHANGED,		// 改变
}OnvifEventState;

typedef struct OnvifEventInfo {
    int type;		// OnvifEventType
    int state;		// OnvifEventState
	void* val;		// 对应枚举后的结构体
}OnvifEventInfo;

typedef struct {
	float usage;
}OnvifEventMonitioringProcessorUsage;

typedef struct {
	float status;
}OnvifEventMonitioringUploadStatus;

typedef struct {
	char reserve;
}OnvifEventMonitioringDefinedLimitReached;

typedef struct {
	int time;
}OnvifEventMonitioringOperatingTime;

typedef struct {
	int cam;
	int stream;
}OnvifEventVideoSource;

typedef struct {
	char name[64];
}OnvifEventPtzPresets;


#endif