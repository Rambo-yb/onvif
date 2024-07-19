#include "soapStub.h"
#include "log.h"
#include "check_common.h"
#include "soap_common.h"
#include "auth.h"

time_t kTerminationTime = 0;

/** Web service operation '__tev__PullMessages' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__PullMessages(struct soap* soap, struct _tev__PullMessages *tev__PullMessages, struct _tev__PullMessagesResponse *tev__PullMessagesResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);

    soap->header->wsa__Action = soap_strdup(soap, "http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/PullMessagesResponse");

    tev__PullMessagesResponse->CurrentTime = time(NULL);
    tev__PullMessagesResponse->TerminationTime = kTerminationTime;
    sleep(5);

    tev__PullMessagesResponse->__sizeNotificationMessage = 1;
    tev__PullMessagesResponse->wsnt__NotificationMessage = soap_malloc(soap, sizeof(struct wsnt__NotificationMessageHolderType)*tev__PullMessagesResponse->__sizeNotificationMessage);
    memset(tev__PullMessagesResponse->wsnt__NotificationMessage, 0, sizeof(struct wsnt__NotificationMessageHolderType)*tev__PullMessagesResponse->__sizeNotificationMessage);
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Topic = soap_malloc(soap, sizeof(struct wsnt__TopicExpressionType));
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Topic->Dialect = soap_strdup(soap, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Topic->__mixed = soap_strdup(soap, "tns1:RuleEngine/LineDetector/Crossed");
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message = soap_malloc(soap, sizeof(struct _tt__Message));
    memset(tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message, 0, sizeof(struct _tt__Message));
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->UtcTime = time(NULL);
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Source = soap_malloc(soap, sizeof(struct tt__ItemList));
    memset(tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Source, 0, sizeof(struct tt__ItemList));
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Source->__sizeSimpleItem = 3;
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Source->SimpleItem = soap_malloc(soap, sizeof(struct _tt__ItemList_SimpleItem)*3);
    memset(tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Source->SimpleItem, 0, sizeof(struct _tt__ItemList_SimpleItem)*3);
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Source->SimpleItem[0].Name = soap_strdup(soap, "VideoSource");
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Source->SimpleItem[0].Value = soap_strdup(soap, "1");
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Source->SimpleItem[1].Name = soap_strdup(soap, "AnalyticsConfiguration");
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Source->SimpleItem[1].Value = soap_strdup(soap, "2");
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Source->SimpleItem[2].Name = soap_strdup(soap, "Rule");
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Source->SimpleItem[2].Value = soap_strdup(soap, "MyImportantFencel");
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Data = soap_malloc(soap, sizeof(struct tt__ItemList));
    memset(tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Data, 0, sizeof(struct tt__ItemList));
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Data->__sizeSimpleItem = 1;
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Data->SimpleItem = soap_malloc(soap, sizeof(struct _tt__ItemList_SimpleItem));
    memset(tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Data->SimpleItem, 0, sizeof(struct _tt__ItemList_SimpleItem));
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Data->SimpleItem[0].Name = soap_strdup(soap, "ObjectId");
    tev__PullMessagesResponse->wsnt__NotificationMessage[0].Message.tt__Message->Data->SimpleItem[0].Value = soap_strdup(soap, "15");

    return 0;
}
/** Web service operation '__tev__Seek' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Seek(struct soap* soap, struct _tev__Seek *tev__Seek, struct _tev__SeekResponse *tev__SeekResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__SetSynchronizationPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__SetSynchronizationPoint(struct soap* soap, struct _tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, struct _tev__SetSynchronizationPointResponse *tev__SetSynchronizationPointResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__Unsubscribe' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe(struct soap* soap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);
    soap->header->wsa__Action = soap_strdup(soap, "http://docs.oasis-open.org/wsn/bw-2/SubscriptionManager/UnsubscribeResponse");

    return 0;
}
/** Web service operation '__tev__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetServiceCapabilities(struct soap* soap, struct _tev__GetServiceCapabilities *tev__GetServiceCapabilities, struct _tev__GetServiceCapabilitiesResponse *tev__GetServiceCapabilitiesResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__CreatePullPointSubscription' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__CreatePullPointSubscription(struct soap* soap, struct _tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, struct _tev__CreatePullPointSubscriptionResponse *tev__CreatePullPointSubscriptionResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);
    
    LOG_INFO("InitialTerminationTime:%s", tev__CreatePullPointSubscription->InitialTerminationTime);

    soap->header->wsa__Action = soap_strdup(soap, "http://www.onvif.org/ver10/events/wsdl/EventPortType/CreatePullPointSubscriptionResponse");

    memset(tev__CreatePullPointSubscriptionResponse, 0, sizeof(struct wsa5__EndpointReferenceType));
    tev__CreatePullPointSubscriptionResponse->SubscriptionReference.Address = soap_strdup(soap, "http://192.168.110.223:3334/Subscription?Idx=0");
    tev__CreatePullPointSubscriptionResponse->wsnt__CurrentTime = time(NULL);
    kTerminationTime = time(NULL) + 600;
    tev__CreatePullPointSubscriptionResponse->wsnt__TerminationTime = kTerminationTime;

    return 0;
}
/** Web service operation '__tev__GetEventProperties' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetEventProperties(struct soap* soap, struct _tev__GetEventProperties *tev__GetEventProperties, struct _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);
    
    soap->header->wsa__Action = soap_strdup(soap, "http://www.onvif.org/ver10/events/wsdl/EventPortType/GetEventPropertiesResponse");

    tev__GetEventPropertiesResponse->__sizeTopicNamespaceLocation = 1;
    tev__GetEventPropertiesResponse->TopicNamespaceLocation = soap_malloc(soap, sizeof(char*)*tev__GetEventPropertiesResponse->__sizeTopicNamespaceLocation);
    tev__GetEventPropertiesResponse->TopicNamespaceLocation[0] = soap_strdup(soap, "http://www.onvif.org/onvif/ver10/topics/topicns.xml");
    tev__GetEventPropertiesResponse->wsnt__FixedTopicSet = xsd__boolean__true_;
    // tev__GetEventPropertiesResponse->wstop__TopicSet = soap_malloc(soap, sizeof(struct wstop__TopicSetType));
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
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__DeleteEventBroker' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__DeleteEventBroker(struct soap* soap, struct _tev__DeleteEventBroker *tev__DeleteEventBroker, struct _tev__DeleteEventBrokerResponse *tev__DeleteEventBrokerResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__GetEventBrokers' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetEventBrokers(struct soap* soap, struct _tev__GetEventBrokers *tev__GetEventBrokers, struct _tev__GetEventBrokersResponse *tev__GetEventBrokersResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__Renew' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Renew(struct soap* soap, struct _wsnt__Renew *wsnt__Renew, struct _wsnt__RenewResponse *wsnt__RenewResponse) {
    CHECK_LT(AuthUser(soap), 0, return 401);
    
    kTerminationTime = time(NULL)+60;
    wsnt__RenewResponse->TerminationTime = kTerminationTime;
    SOAP_SET_NUMBER(soap, wsnt__RenewResponse->CurrentTime, sizeof(time_t), time(NULL));
    return 0;
}
/** Web service operation '__tev__Unsubscribe_' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe_(struct soap* soap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__Subscribe' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Subscribe(struct soap* soap, struct _wsnt__Subscribe *wsnt__Subscribe, struct _wsnt__SubscribeResponse *wsnt__SubscribeResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__GetCurrentMessage' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetCurrentMessage(struct soap* soap, struct _wsnt__GetCurrentMessage *wsnt__GetCurrentMessage, struct _wsnt__GetCurrentMessageResponse *wsnt__GetCurrentMessageResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service one-way operation '__tev__Notify' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Notify(struct soap* soap, struct _wsnt__Notify *wsnt__Notify) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__GetMessages' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__GetMessages(struct soap* soap, struct _wsnt__GetMessages *wsnt__GetMessages, struct _wsnt__GetMessagesResponse *wsnt__GetMessagesResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__DestroyPullPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__DestroyPullPoint(struct soap* soap, struct _wsnt__DestroyPullPoint *wsnt__DestroyPullPoint, struct _wsnt__DestroyPullPointResponse *wsnt__DestroyPullPointResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service one-way operation '__tev__Notify_' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Notify_(struct soap* soap, struct _wsnt__Notify *wsnt__Notify) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__CreatePullPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__CreatePullPoint(struct soap* soap, struct _wsnt__CreatePullPoint *wsnt__CreatePullPoint, struct _wsnt__CreatePullPointResponse *wsnt__CreatePullPointResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__Renew_' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Renew_(struct soap* soap, struct _wsnt__Renew *wsnt__Renew, struct _wsnt__RenewResponse *wsnt__RenewResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__Unsubscribe__' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe__(struct soap* soap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__PauseSubscription' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__PauseSubscription(struct soap* soap, struct _wsnt__PauseSubscription *wsnt__PauseSubscription, struct _wsnt__PauseSubscriptionResponse *wsnt__PauseSubscriptionResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}
/** Web service operation '__tev__ResumeSubscription' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tev__ResumeSubscription(struct soap* soap, struct _wsnt__ResumeSubscription *wsnt__ResumeSubscription, struct _wsnt__ResumeSubscriptionResponse *wsnt__ResumeSubscriptionResponse) {
    printf("%s:%d\n", __func__, __LINE__);
    return 0;
}