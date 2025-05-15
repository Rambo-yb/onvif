#include <sys/time.h>
#include <time.h>
#include "soapStub.h"
#include "soapH.h"
#include "log.h"
#include "check_common.h"
#include "cJSON.h"
#include "soap_common.h"
#include "onvif_auth.h"
#include "onvif_event.h"
#include "onvif_operation.h"

time_t kTerminationTime = 0;

static void GetPullMessageProcessorUsage(struct soap* soap, struct _tt__Message* message, OnvifEventMsg msg) {
	message->UtcTime = time(NULL);

	message->Source = soap_malloc(soap, sizeof(struct tt__ItemList));
	memset(message->Source, 0, sizeof(struct tt__ItemList));
	message->Source->__sizeSimpleItem = 1;
	message->Source->SimpleItem = soap_malloc(soap, sizeof(struct _tt__ItemList_SimpleItem));
	memset(message->Source->SimpleItem, 0, sizeof(struct _tt__ItemList_SimpleItem));
	message->Source->SimpleItem[0].Name = soap_strdup(soap, "Token");
	message->Source->SimpleItem[0].Value = soap_strdup(soap, "Processor_Usage");

	message->Data = soap_malloc(soap, sizeof(struct tt__ItemList));
	memset(message->Data, 0, sizeof(struct tt__ItemList));
	message->Data->__sizeSimpleItem = 1;
	message->Data->SimpleItem = soap_malloc(soap, sizeof(struct _tt__ItemList_SimpleItem));
	memset(message->Data->SimpleItem, 0, sizeof(struct _tt__ItemList_SimpleItem));
	message->Data->SimpleItem[0].Name = soap_strdup(soap, "Value");

	char buff[32] = {0};
	snprintf(buff, sizeof(buff), "%.2f", msg.msg.minitioring_processor_usage.usage);
	message->Data->SimpleItem[0].Value = soap_strdup(soap, buff);

	SOAP_SET_NUMBER(soap, message->PropertyOperation, sizeof(enum tt__PropertyOperation), msg.status);
}

static void GetPullMessageUploadStatus(struct soap* soap, struct _tt__Message* message, OnvifEventMsg msg) {
	message->UtcTime = time(NULL);

	message->Data = soap_malloc(soap, sizeof(struct tt__ItemList));
	memset(message->Data, 0, sizeof(struct tt__ItemList));
	message->Data->__sizeSimpleItem = 1;
	message->Data->SimpleItem = soap_malloc(soap, sizeof(struct _tt__ItemList_SimpleItem));
	memset(message->Data->SimpleItem, 0, sizeof(struct _tt__ItemList_SimpleItem));
	message->Data->SimpleItem[0].Name = soap_strdup(soap, "Status");

	char buff[32] = {0};
	snprintf(buff, sizeof(buff), "%.2f", msg.msg.minitioring_upload_status.status);
	message->Data->SimpleItem[0].Value = soap_strdup(soap, buff);

	SOAP_SET_NUMBER(soap, message->PropertyOperation, sizeof(enum tt__PropertyOperation), msg.status);
}

static void GetPullMessageDefinedLimitReached(struct soap* soap, struct _tt__Message* message, OnvifEventMsg msg) {
	message->UtcTime = time(NULL);

	message->Data = soap_malloc(soap, sizeof(struct tt__ItemList));
	memset(message->Data, 0, sizeof(struct tt__ItemList));
	message->Data->__sizeSimpleItem = 1;
	message->Data->SimpleItem = soap_malloc(soap, sizeof(struct _tt__ItemList_SimpleItem));
	memset(message->Data->SimpleItem, 0, sizeof(struct _tt__ItemList_SimpleItem));
	message->Data->SimpleItem[0].Name = soap_strdup(soap, "Status");
	message->Data->SimpleItem[0].Value = soap_strdup(soap, "ture");
}

static void GetPullMessageOperatingTime(struct soap* soap, struct _tt__Message* message, OnvifEventMsg msg) {
	message->UtcTime = time(NULL);

	message->Data = soap_malloc(soap, sizeof(struct tt__ItemList));
	memset(message->Data, 0, sizeof(struct tt__ItemList));
	message->Data->__sizeSimpleItem = 1;
	message->Data->SimpleItem = soap_malloc(soap, sizeof(struct _tt__ItemList_SimpleItem));
	memset(message->Data->SimpleItem, 0, sizeof(struct _tt__ItemList_SimpleItem));
	message->Data->SimpleItem[0].Name = soap_strdup(soap, "Status");

	char buff[64] = {0};
	struct tm* tm = gmtime((time_t*)&msg.msg.minitioring_operation_time.time);
	snprintf(buff, sizeof(buff), "%04d-%02d-%02dT%02d:%02d:%02dZ", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	message->Data->SimpleItem[0].Value = soap_strdup(soap, buff);

	SOAP_SET_NUMBER(soap, message->PropertyOperation, sizeof(enum tt__PropertyOperation), msg.status);
}

static void GetPullMessageVideoSource(struct soap* soap, struct _tt__Message* message, OnvifEventMsg msg) {
	message->UtcTime = time(NULL);

	message->Source = soap_malloc(soap, sizeof(struct tt__ItemList));
	memset(message->Source, 0, sizeof(struct tt__ItemList));
	message->Source->__sizeSimpleItem = 1;
	message->Source->SimpleItem = soap_malloc(soap, sizeof(struct _tt__ItemList_SimpleItem));
	memset(message->Source->SimpleItem, 0, sizeof(struct _tt__ItemList_SimpleItem));
	message->Source->SimpleItem[0].Name = soap_strdup(soap, "Source");
	char buff[32] = {0};
	snprintf(buff, sizeof(buff), "ONFVideoSource_%03d", msg.msg.video_source.cam*100+msg.msg.video_source.stream+1);
	message->Source->SimpleItem[0].Value = soap_strdup(soap, buff);

	message->Data = soap_malloc(soap, sizeof(struct tt__ItemList));
	memset(message->Data, 0, sizeof(struct tt__ItemList));
	message->Data->__sizeSimpleItem = 1;
	message->Data->SimpleItem = soap_malloc(soap, sizeof(struct _tt__ItemList_SimpleItem));
	memset(message->Data->SimpleItem, 0, sizeof(struct _tt__ItemList_SimpleItem));
	message->Data->SimpleItem[0].Name = soap_strdup(soap, "State");
	message->Data->SimpleItem[0].Value = soap_strdup(soap, "ture");

	SOAP_SET_NUMBER(soap, message->PropertyOperation, sizeof(enum tt__PropertyOperation), msg.status);
}

static void GetPullMessagePtzPresets(struct soap* soap, struct _tt__Message* message, OnvifEventMsg msg) {
	message->UtcTime = time(NULL);

	message->Source = soap_malloc(soap, sizeof(struct tt__ItemList));
	memset(message->Source, 0, sizeof(struct tt__ItemList));
	message->Source->__sizeSimpleItem = 1;
	message->Source->SimpleItem = soap_malloc(soap, sizeof(struct _tt__ItemList_SimpleItem));
	memset(message->Source->SimpleItem, 0, sizeof(struct _tt__ItemList_SimpleItem));
	message->Source->SimpleItem[0].Name = soap_strdup(soap, "PTZConfigurationToken");
	message->Source->SimpleItem[0].Value = soap_strdup(soap, "PTZToken");

	message->Data = soap_malloc(soap, sizeof(struct tt__ItemList));
	memset(message->Data, 0, sizeof(struct tt__ItemList));
	message->Data->__sizeSimpleItem = 2;
	message->Data->SimpleItem = soap_malloc(soap, sizeof(struct _tt__ItemList_SimpleItem));
	memset(message->Data->SimpleItem, 0, sizeof(struct _tt__ItemList_SimpleItem));
	message->Data->SimpleItem[0].Name = soap_strdup(soap, "PresetToken");
	message->Data->SimpleItem[0].Value = soap_strdup(soap, msg.msg.ptz_presets.name);
	message->Data->SimpleItem[1].Name = soap_strdup(soap, "PresetName");
	message->Data->SimpleItem[1].Value = soap_strdup(soap, msg.msg.ptz_presets.name);

	SOAP_SET_NUMBER(soap, message->PropertyOperation, sizeof(enum tt__PropertyOperation), msg.status);
}

typedef void (*OnvifEventOperationCb)(struct soap* soap, struct _tt__Message* message, OnvifEventMsg msg);

typedef struct {
	int type;
	const char* topic;
	OnvifEventOperationCb cb;
}OnvifEventTopicInfo;

static OnvifEventTopicInfo kOnvifEventTopicInfo[] = {
	// device
	{ONVIF_MONITIORING_PROCESSOR_USAGE, "tns1:Monitoring/ProcessorUsage", GetPullMessageProcessorUsage},
	{-1, "tns1:Monitoring/LinkStatus"},													// 在线状态						
	{ONVIF_MONITIORING_UPLOAD_STATUS, "tns1:Monitoring/UploadStatus", GetPullMessageUploadStatus},
	{ONVIF_MONITIORING_DEFINED_LIMIT_REACHED, "tns1:Monitoring/OperatingTime/DefinedLimitReached", GetPullMessageDefinedLimitReached},
	{-1, "tns1:Monitoring/OperatingTime/MeanTimeBetweenFailuresDefaultLimitReached"},	// 达到MTBF默认限制，MTBF:平均故障间隔时间
	{-1, "tns1:Monitoring/OperatingTime/MeanTimeBetweenFailuresOperationLimitReached"},	// 达到MTBF运行限制
	{ONVIF_MONITIORING_LAST_RESET, "tns1:Monitoring/OperatingTime/LastReset", GetPullMessageOperatingTime},
	{ONVIF_MONITIORING_LAST_REBOOT, "tns1:Monitoring/OperatingTime/LastReboot", GetPullMessageOperatingTime},
	{ONVIF_MONITIORING_LAST_CLOCK_SYNC, "tns1:Monitoring/OperatingTime/LastClockSynchronization", GetPullMessageOperatingTime},
	{-1, "tns1:Monitoring/Maintenance/Last"},											// 上次维护时间
	{-1, "tns1:Monitoring/Maintenance/NextScheduled"},									// 下次维护时间
	{-1, "tns1:Monitoring/Backup/Last"},												// 上次备份时间
	{-1, "tns1:Monitoring/AreaOfOperation/OutsideCertifiedArea"},
	{-1, "tns1:Monitoring/AreaOfOperation/OutsideConfiguredArea"},
	{-1, "tns1:Monitoring/EnvironmentalConditions/RelativeHumidity"},					// 湿度
	{-1, "tns1:Monitoring/EnvironmentalConditions/Temperature"},						// 温度
	{-1, "tns1:Monitoring/BatteryCapacity"},											// 电池电量
	{-1, "tns1:Monitoring/AsynchronousOperationStatus"},								// 异步操作上报结果
	{-1, "tns1:Monitoring/OperationMode/ShutdownInitiated"},
	{-1, "tns1:Monitoring/OperationMode/UploadInitiated"},
	{-1, "tns1:Monitoring/Washer/LiquidLow"},											// 洗衣机水箱水位低于阈值
	{-1, "tns1:Device/HardwareFailure/FanFailure"},										// 风扇异常
	{-1, "tns1:Device/HardwareFailure/StorageFailure"},									// 存储异常
	{-1, "tns1:Device/HardwareFailure/TemperatureCritical"},							// 温度异常
	{-1, "tns1:Monitoring/GeoLocation"},												// 设备位置

	// media
	{-1, "tns1:Configuration/Profile"},
	{-1, "tns1:Configuration/VideoEncoderConfiguration"},
	{-1, "tns1:Configuration/VideoSourceConfiguration/MediaService"},
	{-1, "tns1:Configuration/VideoOutputConfiguration/MediaService"},
	{-1, "tns1:Configuration/AudioEncoderConfiguration"},
	{-1, "tns1:Configuration/AudioSourceConfiguration/MediaService"},
	{-1, "tns1:Configuration/AudioOutputConfiguration/MediaService"},
	{-1, "tns1:Configuration/MetadataConfiguration"},
	{-1, "tns1:Configuration/PTZConfiguration"},
	{-1, "tns1:Configuration/VideoAnalyticsConfiguration"},
	{-1, "tns1:Monitoring/Profile/ActiveConnections"},
	{-1, "tns1:Monitoring/Profile/ActiveConnections"},

	// image
	{ONVIF_VIDEO_SOURCE_TOO_BLURRY, "tns1:VideoSource/ImageTooBlurry/ImagingService", GetPullMessageVideoSource},
	{ONVIF_VIDEO_SOURCE_TOO_DARK, "tns1:VideoSource/ImageTooDark/ImagingService", GetPullMessageVideoSource},
	{ONVIF_VIDEO_SOURCE_TOO_BRIGHT, "tns1:VideoSource/ImageTooBright/ImagingService", GetPullMessageVideoSource},
	{ONVIF_VIDEO_SOURCE_SCENE_CHANGE, "tns1:VideoSource/GlobalSceneChange/ImagingService", GetPullMessageVideoSource},
	{ONVIF_VIDEO_SOURCE_SIGNAL_LOSS, "tns1:VideoSource/SignalLoss", GetPullMessageVideoSource},
	{ONVIF_VIDEO_SOURCE_MOTION_ALARM, "tns1:VideoSource/MotionAlarm", GetPullMessageVideoSource},

	// ptz
	{ONVIF_PTZ_PRESETS_INVOKED, "tns1:PTZController/PTZPresets/Invoked", GetPullMessagePtzPresets},
	{ONVIF_PTZ_PRESETS_REACHED, "tns1:PTZController/PTZPresets/Reached", GetPullMessagePtzPresets},
	{ONVIF_PTZ_PRESETS_ABORTED, "tns1:PTZController/PTZPresets/Aborted", GetPullMessagePtzPresets},
	{ONVIF_PTZ_PRESETS_LEFT, "tns1:PTZController/PTZPresets/Left", GetPullMessagePtzPresets},
	{-1, "tns1:PTZController/PTZPresetTours/Configuration"},							// 轮巡路径修改
};

static int GetEventTopicInfo(int type) {
	for(int i = 0; i < sizeof(kOnvifEventTopicInfo)/sizeof(OnvifEventTopicInfo); i++) {
		if (kOnvifEventTopicInfo[i].type == type) {
			return i;
		}
	}
	
	LOG_ERR("unknown topic type : %d", type);
	return -1;
}

/** Web service operation '__tev__PullMessages' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__PullMessages(struct soap* soap, struct _tev__PullMessages *tev__PullMessages, struct _tev__PullMessagesResponse *tev__PullMessagesResponse) {
    if (OnvifAuthUser(soap) < 0) {
        return 401;
    }

	if (soap->header != NULL) {
    	soap->header->wsa__Action = soap_strdup(soap, "http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/PullMessagesResponse");
	}
    tev__PullMessagesResponse->CurrentTime = time(NULL);
    tev__PullMessagesResponse->TerminationTime = kTerminationTime;

	OnvifEventMsg msg;
	if (OnvifEventGetEvets(&msg, tev__PullMessages->Timeout) < 0) {
		return 0;
	}

	int index = GetEventTopicInfo(msg.type);
	CHECK_LT(index, 0, return 500);

    tev__PullMessagesResponse->__sizeNotificationMessage = 1;
    tev__PullMessagesResponse->wsnt__NotificationMessage = soap_malloc(soap, sizeof(struct wsnt__NotificationMessageHolderType)*tev__PullMessagesResponse->__sizeNotificationMessage);
    memset(tev__PullMessagesResponse->wsnt__NotificationMessage, 0, sizeof(struct wsnt__NotificationMessageHolderType)*tev__PullMessagesResponse->__sizeNotificationMessage);
    for (int i = 0; i < tev__PullMessagesResponse->__sizeNotificationMessage; i++) {
        tev__PullMessagesResponse->wsnt__NotificationMessage[i].Topic = soap_malloc(soap, sizeof(struct wsnt__TopicExpressionType));
        tev__PullMessagesResponse->wsnt__NotificationMessage[i].Topic->Dialect = soap_strdup(soap, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
        tev__PullMessagesResponse->wsnt__NotificationMessage[i].Topic->__mixed = soap_strdup(soap, kOnvifEventTopicInfo[index].topic);

        tev__PullMessagesResponse->wsnt__NotificationMessage[i].Message.tt__Message = soap_malloc(soap, sizeof(struct _tt__Message));
        memset(tev__PullMessagesResponse->wsnt__NotificationMessage[i].Message.tt__Message, 0, sizeof(struct _tt__Message));
		kOnvifEventTopicInfo[i].cb(soap, tev__PullMessagesResponse->wsnt__NotificationMessage[i].Message.tt__Message, msg);
    }
	
    return 0;
}
/** Web service operation '__tev__Seek' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Seek(struct soap* soap, struct _tev__Seek *tev__Seek, struct _tev__SeekResponse *tev__SeekResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__SetSynchronizationPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__SetSynchronizationPoint(struct soap* soap, struct _tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, struct _tev__SetSynchronizationPointResponse *tev__SetSynchronizationPointResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__Unsubscribe' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe(struct soap* soap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse) {
    if (OnvifAuthUser(soap) < 0) {
        return 401;
    }
	if (soap->header != NULL) {
    	soap->header->wsa__Action = soap_strdup(soap, "http://docs.oasis-open.org/wsn/bw-2/SubscriptionManager/UnsubscribeResponse");
	}
    return 0;
}

void GetEventServiceCapabilities(struct soap* soap, struct tev__Capabilities* capabilities) {
	SOAP_SET_NUMBER(soap, capabilities->WSSubscriptionPolicySupport, sizeof(enum xsd__boolean), xsd__boolean__true_);
	// SOAP_SET_NUMBER(soap, capabilities->WSPullPointSupport, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->WSPausableSubscriptionManagerInterfaceSupport, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->MaxNotificationProducers, sizeof(int), 10);
	SOAP_SET_NUMBER(soap, capabilities->MaxPullPoints, sizeof(int), 10);
}
/** Web service operation '__tev__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetServiceCapabilities(struct soap* soap, struct _tev__GetServiceCapabilities *tev__GetServiceCapabilities, struct _tev__GetServiceCapabilitiesResponse *tev__GetServiceCapabilitiesResponse) {
	tev__GetServiceCapabilitiesResponse->Capabilities = (struct tev__Capabilities*)soap_malloc(soap, sizeof(struct tev__Capabilities));
	memset(tev__GetServiceCapabilitiesResponse->Capabilities, 0, sizeof(struct tev__Capabilities));
	GetEventServiceCapabilities(soap, tev__GetServiceCapabilitiesResponse->Capabilities);
    return 0;
}
/** Web service operation '__tev__CreatePullPointSubscription' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__CreatePullPointSubscription(struct soap* soap, struct _tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, struct _tev__CreatePullPointSubscriptionResponse *tev__CreatePullPointSubscriptionResponse) {
    if (OnvifAuthUser(soap) < 0) {
        return 401;
    }
    
	OnvifConfigNetworkTcpIp network_tcpip;
	memset(&network_tcpip, 0, sizeof(OnvifConfigNetworkTcpIp));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

    LONG64 termination_time_ms = 0;
    soap_s2xsd__duration(soap, tev__CreatePullPointSubscription->InitialTerminationTime, &termination_time_ms);

	if (soap->header != NULL) {
    	soap->header->wsa__Action = soap_strdup(soap, "http://www.onvif.org/ver10/events/wsdl/EventPortType/CreatePullPointSubscriptionResponse");
	}

	char address[128] = {0};
	snprintf(address, sizeof(address), "http://%s:3334/Subscription?Idx=0", network_tcpip.ipv4_addr);
    memset(tev__CreatePullPointSubscriptionResponse, 0, sizeof(struct wsa5__EndpointReferenceType));
    tev__CreatePullPointSubscriptionResponse->SubscriptionReference.Address = soap_strdup(soap, address);
    tev__CreatePullPointSubscriptionResponse->wsnt__CurrentTime = time(NULL);
    kTerminationTime = time(NULL) + termination_time_ms/1000;
    tev__CreatePullPointSubscriptionResponse->wsnt__TerminationTime = kTerminationTime;

    return 0;
}
/** Web service operation '__tev__GetEventProperties' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetEventProperties(struct soap* soap, struct _tev__GetEventProperties *tev__GetEventProperties, struct _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse) {
    if (OnvifAuthUser(soap) < 0) {
        return 401;
    }
    
	if (soap->header != NULL) {
    	soap->header->wsa__Action = soap_strdup(soap, "http://www.onvif.org/ver10/events/wsdl/EventPortType/GetEventPropertiesResponse");
	}

    tev__GetEventPropertiesResponse->__sizeTopicNamespaceLocation = 1;
    tev__GetEventPropertiesResponse->TopicNamespaceLocation = soap_malloc(soap, sizeof(char*)*tev__GetEventPropertiesResponse->__sizeTopicNamespaceLocation);
    tev__GetEventPropertiesResponse->TopicNamespaceLocation[0] = soap_strdup(soap, "http://www.onvif.org/onvif/ver10/topics/topicns.xml");
    tev__GetEventPropertiesResponse->wsnt__FixedTopicSet = xsd__boolean__true_;
	
    tev__GetEventPropertiesResponse->wstop__TopicSet = soap_malloc(soap, sizeof(struct wstop__TopicSetType));
    memset(tev__GetEventPropertiesResponse->wstop__TopicSet, 0, sizeof(struct wstop__TopicSetType));
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription = soap_malloc(soap, sizeof(struct tt__MessageDescription));
    // memset(tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription, 0, sizeof(struct tt__MessageDescription));
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Source = soap_malloc(soap, sizeof(struct tt__ItemListDescription));
    // memset(tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Source, 0, sizeof(struct tt__ItemListDescription));
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Source->__sizeSimpleItemDescription = 2;
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Source->SimpleItemDescription = soap_malloc(soap, sizeof(struct _tt__ItemListDescription_SimpleItemDescription)*tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Source->__sizeSimpleItemDescription); 
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Source->SimpleItemDescription[0].Name = soap_strdup(soap, "VideoSourceConfigurationToken");
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Source->SimpleItemDescription[0].Type = soap_strdup(soap, "tt:ReferenceToken");
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Source->SimpleItemDescription[1].Name = soap_strdup(soap, "VideoAnalyticsConfigurationToken");
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Source->SimpleItemDescription[1].Type = soap_strdup(soap, "tt:ReferenceToken");
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Data = soap_malloc(soap, sizeof(struct tt__ItemListDescription));
    // memset(tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Data, 0, sizeof(struct tt__ItemListDescription));
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Data->__sizeSimpleItemDescription = 1;
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Data->SimpleItemDescription = soap_malloc(soap, sizeof(struct _tt__ItemListDescription_SimpleItemDescription)*tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Data->__sizeSimpleItemDescription); 
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Data->SimpleItemDescription[0].Name = soap_strdup(soap, "ObjectId");
    // tev__GetEventPropertiesResponse->wstop__TopicSet->MessageDescription->Data->SimpleItemDescription[0].Type = soap_strdup(soap, "tt:ObjectRefType");

    tev__GetEventPropertiesResponse->__sizeTopicExpressionDialect = 2;
    tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect = soap_malloc(soap, sizeof(char*)*tev__GetEventPropertiesResponse->__sizeTopicNamespaceLocation);
    tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect[0] = soap_strdup(soap, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
    tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect[1] = soap_strdup(soap, "http://docs.oasis-open.org/wsnt/t-1/TopicExpression/ConcreteSet");

    tev__GetEventPropertiesResponse->__sizeMessageContentFilterDialect = 1;
    tev__GetEventPropertiesResponse->MessageContentFilterDialect = soap_malloc(soap, sizeof(char*)*tev__GetEventPropertiesResponse->__sizeMessageContentFilterDialect);
    tev__GetEventPropertiesResponse->MessageContentFilterDialect[0] = soap_strdup(soap, "http://www.onvif.org/ver10/tev/messageContentFilter/ItemFilter");

    tev__GetEventPropertiesResponse->__sizeMessageContentSchemaLocation = 1;
    tev__GetEventPropertiesResponse->MessageContentSchemaLocation = soap_malloc(soap, sizeof(char*)*tev__GetEventPropertiesResponse->__sizeMessageContentSchemaLocation);
    tev__GetEventPropertiesResponse->MessageContentSchemaLocation[0] = soap_strdup(soap, "http://www.onvif.org/onvif/ver10/schema/onvif.xsd");
    return 0;
}
/** Web service operation '__tev__AddEventBroker' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__AddEventBroker(struct soap* soap, struct _tev__AddEventBroker *tev__AddEventBroker, struct _tev__AddEventBrokerResponse *tev__AddEventBrokerResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__DeleteEventBroker' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__DeleteEventBroker(struct soap* soap, struct _tev__DeleteEventBroker *tev__DeleteEventBroker, struct _tev__DeleteEventBrokerResponse *tev__DeleteEventBrokerResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__GetEventBrokers' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetEventBrokers(struct soap* soap, struct _tev__GetEventBrokers *tev__GetEventBrokers, struct _tev__GetEventBrokersResponse *tev__GetEventBrokersResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__Renew' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Renew(struct soap* soap, struct _wsnt__Renew *wsnt__Renew, struct _wsnt__RenewResponse *wsnt__RenewResponse) {
    if (OnvifAuthUser(soap) < 0) {
        return 401;
    }
    
    LONG64 termination_time_ms = 0;
    soap_s2xsd__duration(soap, wsnt__Renew->TerminationTime, &termination_time_ms);

    kTerminationTime = time(NULL) + termination_time_ms / 1000;
    wsnt__RenewResponse->TerminationTime = kTerminationTime;
    SOAP_SET_NUMBER(soap, wsnt__RenewResponse->CurrentTime, sizeof(time_t), time(NULL));
    return 0;
}
/** Web service operation '__tev__Unsubscribe_' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe_(struct soap* soap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__Subscribe' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Subscribe(struct soap* soap, struct _wsnt__Subscribe *wsnt__Subscribe, struct _wsnt__SubscribeResponse *wsnt__SubscribeResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__GetCurrentMessage' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetCurrentMessage(struct soap* soap, struct _wsnt__GetCurrentMessage *wsnt__GetCurrentMessage, struct _wsnt__GetCurrentMessageResponse *wsnt__GetCurrentMessageResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service one-way operation '__tev__Notify' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Notify(struct soap* soap, struct _wsnt__Notify *wsnt__Notify) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__GetMessages' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetMessages(struct soap* soap, struct _wsnt__GetMessages *wsnt__GetMessages, struct _wsnt__GetMessagesResponse *wsnt__GetMessagesResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__DestroyPullPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__DestroyPullPoint(struct soap* soap, struct _wsnt__DestroyPullPoint *wsnt__DestroyPullPoint, struct _wsnt__DestroyPullPointResponse *wsnt__DestroyPullPointResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service one-way operation '__tev__Notify_' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Notify_(struct soap* soap, struct _wsnt__Notify *wsnt__Notify) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__CreatePullPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__CreatePullPoint(struct soap* soap, struct _wsnt__CreatePullPoint *wsnt__CreatePullPoint, struct _wsnt__CreatePullPointResponse *wsnt__CreatePullPointResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__Renew_' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Renew_(struct soap* soap, struct _wsnt__Renew *wsnt__Renew, struct _wsnt__RenewResponse *wsnt__RenewResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__Unsubscribe__' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe__(struct soap* soap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__PauseSubscription' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__PauseSubscription(struct soap* soap, struct _wsnt__PauseSubscription *wsnt__PauseSubscription, struct _wsnt__PauseSubscriptionResponse *wsnt__PauseSubscriptionResponse) {
    LOG_WRN("unrealized");
    return 0;
}
/** Web service operation '__tev__ResumeSubscription' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__ResumeSubscription(struct soap* soap, struct _wsnt__ResumeSubscription *wsnt__ResumeSubscription, struct _wsnt__ResumeSubscriptionResponse *wsnt__ResumeSubscriptionResponse) {
    LOG_WRN("unrealized");
    return 0;
}