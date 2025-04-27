#include <sys/time.h>
#include <time.h>
#include "soapStub.h"
#include "soapH.h"
#include "onvif_auth.h"
#include "onvif_conf.h"
#include "onvif_operation.h"
#include "log.h"
#include "check_common.h"
#include "cjson_common.h"
#include "soap_common.h"

#define LOG_DIR_PATH "/oem/logs.tar.gz"
#define UPGRADE_FILE_PATH "/data/fota/upgrade.bin"

/** Web service one-way operation 'SOAP_ENV__Fault_ex' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 SOAP_ENV__Fault_ex(struct soap* soap, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}

static void GetServiceCapabilitiesNetwork(struct soap* soap, struct tds__NetworkCapabilities* capabilities) {
	SOAP_SET_NUMBER(soap, capabilities->IPFilter, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->ZeroConfiguration, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->IPVersion6, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->DynDNS, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->Dot11Configuration, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->Dot1XConfigurations, sizeof(int), 0);
	SOAP_SET_NUMBER(soap, capabilities->HostnameFromDHCP, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->NTP, sizeof(int), 1);
	SOAP_SET_NUMBER(soap, capabilities->DHCPv6, sizeof(enum xsd__boolean), xsd__boolean__true_);
}

static void GetServiceCapabilitiesSecurity(struct soap* soap, struct tds__SecurityCapabilities* capabilities) {
	SOAP_SET_NUMBER(soap, capabilities->TLS1_x002e0, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->TLS1_x002e1, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->TLS1_x002e2, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->OnboardKeyGeneration, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->AccessPolicyConfig, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->DefaultAccessPolicy, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->Dot1X, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->RemoteUserHandling, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->X_x002e509Token, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->SAMLToken, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->KerberosToken, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->UsernameToken, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->HttpDigest, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->RELToken, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->JsonWebToken, sizeof(enum xsd__boolean), xsd__boolean__false_);
	capabilities->SupportedEAPMethods = soap_strdup(soap, "0");
	SOAP_SET_NUMBER(soap, capabilities->MaxUsers, sizeof(int), 32);
	SOAP_SET_NUMBER(soap, capabilities->MaxUserNameLength, sizeof(int), 32);
	SOAP_SET_NUMBER(soap, capabilities->MaxPasswordLength, sizeof(int), 64);
	// capabilities->SecurityPolicies = soap_strdup(soap, "AuthFailureWarnings");
	// SOAP_SET_NUMBER(soap, capabilities->MaxPasswordHistory, sizeof(int), 1);
	// capabilities->HashingAlgorithms = soap_strdup(soap, "SHA-256");
}

static void GetServiceCapabilitiesSystem(struct soap* soap, struct tds__SystemCapabilities* capabilities) {
	SOAP_SET_NUMBER(soap, capabilities->DiscoveryResolve, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->DiscoveryBye, sizeof(enum xsd__boolean), xsd__boolean__true_);
	SOAP_SET_NUMBER(soap, capabilities->RemoteDiscovery, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->SystemBackup, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->SystemLogging, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->FirmwareUpgrade, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->HttpFirmwareUpgrade, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->HttpSystemBackup, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->HttpSystemLogging, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->HttpSupportInformation, sizeof(enum xsd__boolean), xsd__boolean__false_);
	SOAP_SET_NUMBER(soap, capabilities->StorageConfiguration, sizeof(enum xsd__boolean), xsd__boolean__true_);
	// SOAP_SET_NUMBER(soap, capabilities->MaxStorageConfigurations, sizeof(int), 32);
	// SOAP_SET_NUMBER(soap, capabilities->GeoLocationEntries, sizeof(int), 32);
	// capabilities->AutoGeo = NULL;
	// capabilities->StorageTypesSupported = NULL;
	// SOAP_SET_NUMBER(soap, capabilities->DiscoveryNotSupported, sizeof(enum xsd__boolean), xsd__boolean__false_);
	// SOAP_SET_NUMBER(soap, capabilities->NetworkConfigNotSupported, sizeof(enum xsd__boolean), xsd__boolean__false_);
	// SOAP_SET_NUMBER(soap, capabilities->UserConfigNotSupported, sizeof(enum xsd__boolean), xsd__boolean__false_);
	// capabilities->Addons = NULL;
}

static void GetDeviceServiceCapabilities(struct soap* soap, struct tds__DeviceServiceCapabilities* capabilities) {
	capabilities->Network = (struct tds__NetworkCapabilities*)soap_malloc(soap, sizeof(struct tds__NetworkCapabilities));
	memset(capabilities->Network, 0, sizeof(struct tds__NetworkCapabilities));
	GetServiceCapabilitiesNetwork(soap, capabilities->Network);

	capabilities->Security = (struct tds__SecurityCapabilities*)soap_malloc(soap, sizeof(struct tds__SecurityCapabilities));
	memset(capabilities->Security, 0, sizeof(struct tds__SecurityCapabilities));
	GetServiceCapabilitiesSecurity(soap, capabilities->Security);

	capabilities->System = (struct tds__SystemCapabilities*)soap_malloc(soap, sizeof(struct tds__SystemCapabilities));
	memset(capabilities->System, 0, sizeof(struct tds__SystemCapabilities));
	GetServiceCapabilitiesSystem(soap, capabilities->System);

	capabilities->Misc = (struct tds__MiscCapabilities*)soap_malloc(soap, sizeof(struct tds__MiscCapabilities));
	capabilities->Misc->AuxiliaryCommands = NULL;
}

extern void GetMediaV10ServiceCapabilities(struct soap* soap, struct trt__Capabilities* capabilities);
extern void GetPtzServiceCapabilities(struct soap* soap, struct tptz__Capabilities* capabilities);
extern void GetImagingServiceCapabilities(struct soap* soap, struct timg__Capabilities* capabilities);

typedef struct {
	char* namespace;
	char* uri;
	int major;
	int minor;
}ServiceInfo;
static ServiceInfo kServiceInfo[] = {
	{.namespace = "http://www.onvif.org/ver10/device/wsdl/devicemgmt.wsdl", .uri = "onvif/device_service", .major = 1, .minor = 0},
	{.namespace = "http://www.onvif.org/ver10/media/wsdl/media.wsdl", .uri = "onvif/media_service", .major = 1, .minor = 0},
	{.namespace = "http://www.onvif.org/ver20/ptz/wsdl/ptz.wsdl", .uri = "onvif/ptz_service", .major = 1, .minor = 0},
	{.namespace = "http://www.onvif.org/ver10/events/wsdl/event.wsdl", .uri = "onvif/event_service", .major = 1, .minor = 0},
	{.namespace = "http://www.onvif.org/ver20/imaging/wsdl/imaging.wsdl", .uri = "onvif/imaging_service", .major = 1, .minor = 0},
	{.namespace = "http://www.onvif.org/ver10/deviceio.wsdl", .uri = "onvif/device_io_service", .major = 1, .minor = 0},
	{.namespace = "http://www.onvif.org/ver10/recording.wsdl", .uri = "onvif/recording_service", .major = 1, .minor = 0},
	{.namespace = "http://www.onvif.org/ver10/display.wsdl", .uri = "onvif/display_service", .major = 1, .minor = 0},
};
/** Web service operation '__tds__GetServices' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServices(struct soap* soap, struct _tds__GetServices *tds__GetServices, struct _tds__GetServicesResponse *tds__GetServicesResponse) {
	OnvifConfigNetworkTcpIp network_tcpip;
	memset(&network_tcpip, 0, sizeof(OnvifConfigNetworkTcpIp));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

	tds__GetServicesResponse->__sizeService = sizeof(kServiceInfo)/sizeof(ServiceInfo);
	tds__GetServicesResponse->Service = (struct tds__Service*)soap_malloc(soap, sizeof(struct tds__Service)*tds__GetServicesResponse->__sizeService);
	memset(tds__GetServicesResponse->Service, 0, sizeof(struct tds__Service)*tds__GetServicesResponse->__sizeService);
	for(int i = 0; i < tds__GetServicesResponse->__sizeService; i++) {
		tds__GetServicesResponse->Service[i].Namespace = soap_strdup(soap, kServiceInfo[i].namespace);
		char url[512] = {0};
		snprintf(url, sizeof(url), "http://%s:%d/%s", network_tcpip.ipv4_addr, 3333, kServiceInfo[i].uri);
		tds__GetServicesResponse->Service[i].XAddr = soap_strdup(soap, url);
		tds__GetServicesResponse->Service[i].Capabilities = (struct _tds__Service_Capabilities*)soap_malloc(soap, sizeof(struct _tds__Service_Capabilities));
		memset(tds__GetServicesResponse->Service[i].Capabilities, 0, sizeof(struct _tds__Service_Capabilities));
		if (i == 0) {
			tds__GetServicesResponse->Service[i].Capabilities->tds__Capabilities = (struct tds__DeviceServiceCapabilities*)soap_malloc(soap, sizeof(struct tds__DeviceServiceCapabilities));
			memset(tds__GetServicesResponse->Service[i].Capabilities->tds__Capabilities, 0, sizeof(struct tds__DeviceServiceCapabilities));
			GetDeviceServiceCapabilities(soap, tds__GetServicesResponse->Service[i].Capabilities->tds__Capabilities);
		} else if (i == 1) {
			tds__GetServicesResponse->Service[i].Capabilities->trt__Capabilities = (struct trt__Capabilities*)soap_malloc(soap, sizeof(struct trt__Capabilities));
			memset(tds__GetServicesResponse->Service[i].Capabilities->trt__Capabilities, 0, sizeof(struct trt__Capabilities));
			GetMediaV10ServiceCapabilities(soap, tds__GetServicesResponse->Service[i].Capabilities->trt__Capabilities);
		} else if (i == 2) {
			tds__GetServicesResponse->Service[i].Capabilities->tptz__Capabilities = (struct tptz__Capabilities*)soap_malloc(soap, sizeof(struct tptz__Capabilities));
			memset(tds__GetServicesResponse->Service[i].Capabilities->tptz__Capabilities, 0, sizeof(struct tptz__Capabilities));
			GetPtzServiceCapabilities(soap, tds__GetServicesResponse->Service[i].Capabilities->tptz__Capabilities);
		} else if (i == 4) {
			tds__GetServicesResponse->Service[i].Capabilities->timg__Capabilities = (struct timg__Capabilities*)soap_malloc(soap, sizeof(struct timg__Capabilities));
			memset(tds__GetServicesResponse->Service[i].Capabilities->timg__Capabilities, 0, sizeof(struct timg__Capabilities));
			GetImagingServiceCapabilities(soap, tds__GetServicesResponse->Service[i].Capabilities->timg__Capabilities);
		}
		tds__GetServicesResponse->Service[i].Version = (struct tt__OnvifVersion *)soap_malloc(soap, sizeof(struct tt__OnvifVersion));
		tds__GetServicesResponse->Service[i].Version->Major = kServiceInfo[i].major;
		tds__GetServicesResponse->Service[i].Version->Minor = kServiceInfo[i].minor;
	}
	return 0;
}
/** Web service operation '__tds__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServiceCapabilities(struct soap* soap, struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities, struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse) {
	tds__GetServiceCapabilitiesResponse->Capabilities = (struct tds__DeviceServiceCapabilities*)soap_malloc(soap, sizeof(struct tds__DeviceServiceCapabilities));
	memset(tds__GetServiceCapabilitiesResponse->Capabilities, 0, sizeof(struct tds__DeviceServiceCapabilities));

	GetDeviceServiceCapabilities(soap, tds__GetServiceCapabilitiesResponse->Capabilities);

	return 0;
}
/** Web service operation '__tds__GetDeviceInformation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDeviceInformation(struct soap* soap, struct _tds__GetDeviceInformation *tds__GetDeviceInformation, struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifSystem system_request;
	system_request.type = ONVIF_SYSTEM_DEVICE_INFO;
	OnvifOperationSystemRequest(&system_request);

	tds__GetDeviceInformationResponse->Manufacturer = soap_strdup(soap, "ONVIF");
	tds__GetDeviceInformationResponse->Model = soap_strdup(soap, system_request.out.device_info.device_type);
	tds__GetDeviceInformationResponse->FirmwareVersion = soap_strdup(soap, system_request.out.device_info.soft_version);
	tds__GetDeviceInformationResponse->SerialNumber = soap_strdup(soap, system_request.out.device_info.serial_number);
	tds__GetDeviceInformationResponse->HardwareId = soap_strdup(soap, system_request.out.device_info.hard_version);
	return 0;
}
/** Web service operation '__tds__SetSystemDateAndTime' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemDateAndTime(struct soap* soap, struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse) {
	// CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigSystemTimeInfo system_time_info;
	memset(&system_time_info, 0, sizeof(OnvifConfigSystemTimeInfo));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_SYSTEM_TIME_INFO, &system_time_info, sizeof(OnvifConfigSystemTimeInfo));
	CHECK_LT(ret, 0, return 500);

	system_time_info.timing_type = tds__SetSystemDateAndTime->DateTimeType;
	if (strstr(tds__SetSystemDateAndTime->TimeZone->TZ, "GMT") != NULL) {
		snprintf(system_time_info.time_zone, sizeof(system_time_info.time_zone), "%s", tds__SetSystemDateAndTime->TimeZone->TZ);
	} else {
		SOAP_ERROR_REPLAY(soap, "Invalid data", 500);
	}

	ret = OnvifOperationSetConfig(ONVIF_CONFIG_SYSTEM_TIME_INFO, &system_time_info, sizeof(OnvifConfigSystemTimeInfo));
	CHECK_LT(ret, 0, return 500);

	if (tds__SetSystemDateAndTime->DateTimeType == tt__SetDateTimeType__Manual) {
		LOG_INFO("set time:%04d/%02d/%02d %02d:%02d:%02d", 
			tds__SetSystemDateAndTime->UTCDateTime->Date->Year, tds__SetSystemDateAndTime->UTCDateTime->Date->Month, tds__SetSystemDateAndTime->UTCDateTime->Date->Day, 
			tds__SetSystemDateAndTime->UTCDateTime->Time->Hour, tds__SetSystemDateAndTime->UTCDateTime->Time->Minute, tds__SetSystemDateAndTime->UTCDateTime->Time->Second);

		struct tm tm;
		memset(&tm, 0, sizeof(struct tm));
		tm.tm_sec = tds__SetSystemDateAndTime->UTCDateTime->Time->Second;
		tm.tm_min = tds__SetSystemDateAndTime->UTCDateTime->Time->Minute;
		tm.tm_hour = tds__SetSystemDateAndTime->UTCDateTime->Time->Hour;
		tm.tm_mday = tds__SetSystemDateAndTime->UTCDateTime->Date->Day;
		tm.tm_mon = tds__SetSystemDateAndTime->UTCDateTime->Date->Month - 1;
		tm.tm_year = tds__SetSystemDateAndTime->UTCDateTime->Date->Year -1900;

		OnvifSystem system_request;
		system_request.type = ONVIF_SYSTEM_SET_TIME;
		system_request.in.time = mktime(&tm);
		ret = OnvifOperationSystemRequest(&system_request);
		CHECK_LT(ret, 0, return 500);
	}

	return 0;
}
/** Web service operation '__tds__GetSystemDateAndTime' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemDateAndTime(struct soap* soap, struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse) {
	OnvifSystem system_request;
	system_request.type = ONVIF_SYSTEM_GET_TIME;
	int ret = OnvifOperationSystemRequest(&system_request);
	CHECK_LT(ret, 0, return 500);

	OnvifConfigSystemTimeInfo system_time_info;
	memset(&system_time_info, 0, sizeof(OnvifConfigSystemTimeInfo));
	ret = OnvifOperationGetConfig(ONVIF_CONFIG_SYSTEM_TIME_INFO, &system_time_info, sizeof(OnvifConfigSystemTimeInfo));
	CHECK_LT(ret, 0, return 500);

	tds__GetSystemDateAndTimeResponse->SystemDateAndTime = (struct tt__SystemDateTime *)soap_malloc(soap, sizeof(struct tt__SystemDateTime));
	memset(tds__GetSystemDateAndTimeResponse->SystemDateAndTime, 0, sizeof(struct tt__SystemDateTime));

	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->DateTimeType = system_time_info.timing_type;
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->TimeZone = (struct tt__TimeZone *)soap_malloc(soap, sizeof(struct tt__TimeZone));
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->TimeZone->TZ = soap_strdup(soap, system_time_info.time_zone);

	time_t utc_sec = system_request.out.time;
	struct tm* tm = gmtime(&utc_sec);
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->UTCDateTime = (struct tt__DateTime *)soap_malloc(soap, sizeof(struct tt__DateTime));
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->UTCDateTime->Date = (struct tt__Date *)soap_malloc(soap, sizeof(struct tt__Date));
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->UTCDateTime->Date->Year = tm->tm_year+1900;
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->UTCDateTime->Date->Month = tm->tm_mon+1;
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->UTCDateTime->Date->Day = tm->tm_mday;
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->UTCDateTime->Time = (struct tt__Time *)soap_malloc(soap, sizeof(struct tt__Time));
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->UTCDateTime->Time->Hour = tm->tm_hour;
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->UTCDateTime->Time->Minute = tm->tm_min;
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->UTCDateTime->Time->Second = tm->tm_sec;

	time_t loc_sec = system_request.out.time + 8*60*60;
	tm = gmtime(&loc_sec);
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->LocalDateTime = (struct tt__DateTime *)soap_malloc(soap, sizeof(struct tt__DateTime));
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->LocalDateTime->Date = (struct tt__Date *)soap_malloc(soap, sizeof(struct tt__Date));
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->LocalDateTime->Date->Year = tm->tm_year+1900;
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->LocalDateTime->Date->Month = tm->tm_mon+1;
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->LocalDateTime->Date->Day = tm->tm_mday;
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->LocalDateTime->Time = (struct tt__Time *)soap_malloc(soap, sizeof(struct tt__Time));
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->LocalDateTime->Time->Hour = tm->tm_hour;
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->LocalDateTime->Time->Minute = tm->tm_min;
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime->LocalDateTime->Time->Second = tm->tm_sec;

	return 0;
}
/** Web service operation '__tds__SetSystemFactoryDefault' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemFactoryDefault(struct soap* soap, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__UpgradeSystemFirmware' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__UpgradeSystemFirmware(struct soap* soap, struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	if (tds__UpgradeSystemFirmware->Firmware != NULL 
		&& tds__UpgradeSystemFirmware->Firmware->xop__Include.__ptr != NULL 
		&& tds__UpgradeSystemFirmware->Firmware->xop__Include.__size > 0) {
		FILE* fp = fopen(UPGRADE_FILE_PATH, "w+");
		CHECK_POINTER(fp, return 500);

		int ret = fwrite(tds__UpgradeSystemFirmware->Firmware->xop__Include.__ptr, 1, tds__UpgradeSystemFirmware->Firmware->xop__Include.__size, fp);
		CHECK_EQ(ret, tds__UpgradeSystemFirmware->Firmware->xop__Include.__size, fclose(fp);return 500);

		fclose(fp);
	}
	return 0;
}
/** Web service operation '__tds__SystemReboot' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SystemReboot(struct soap* soap, struct _tds__SystemReboot *tds__SystemReboot, struct _tds__SystemRebootResponse *tds__SystemRebootResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);
	
	OnvifSystem system_request;
	system_request.type = ONVIF_SYSTEM_REBOOT;
	OnvifOperationSystemRequest(&system_request);
	
	system("reboot");
	return 0;
}
/** Web service operation '__tds__RestoreSystem' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RestoreSystem(struct soap* soap, struct _tds__RestoreSystem *tds__RestoreSystem, struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetSystemBackup' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemBackup(struct soap* soap, struct _tds__GetSystemBackup *tds__GetSystemBackup, struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetSystemLog' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemLog(struct soap* soap, struct _tds__GetSystemLog *tds__GetSystemLog, struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);
	CHECK_EQ(access(LOG_DIR_PATH, F_OK), 0, return 0);
	
	FILE* fp = fopen(LOG_DIR_PATH, "rb");
	CHECK_POINTER(fp, return 500);

	fseek(fp, 0, SEEK_END);
	int file_size = ftell(fp);

	tds__GetSystemLogResponse->SystemLog = (struct tt__SystemLog *)soap_malloc(soap, sizeof(struct tt__SystemLog));
	memset(tds__GetSystemLogResponse->SystemLog, 0, sizeof(struct tt__SystemLog));

	tds__GetSystemLogResponse->SystemLog->Binary = (struct tt__AttachmentData *)soap_malloc(soap, sizeof(struct tt__AttachmentData));
	memset(tds__GetSystemLogResponse->SystemLog->Binary, 0, sizeof(struct tt__AttachmentData));

	tds__GetSystemLogResponse->SystemLog->Binary->xop__Include.__ptr = soap_malloc(soap, file_size);
	tds__GetSystemLogResponse->SystemLog->Binary->xop__Include.__size = file_size;

	fseek(fp, 0, SEEK_SET);
	int ret = fwrite(tds__GetSystemLogResponse->SystemLog->Binary->xop__Include.__ptr, 1, file_size, fp);
	CHECK_EQ(ret, file_size, fclose(fp);return 500);

	fclose(fp);
	return 0;
}
/** Web service operation '__tds__GetSystemSupportInformation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemSupportInformation(struct soap* soap, struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetScopes(struct soap* soap, struct _tds__GetScopes *tds__GetScopes, struct _tds__GetScopesResponse *tds__GetScopesResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	cJSON* scopes_json = OnvifConfGetConfig("scopes");
	CHECK_POINTER(scopes_json, return 500);
	CHECK_BOOL(cJSON_IsArray(scopes_json), cJSON_free(scopes_json);return 500);

	tds__GetScopesResponse->__sizeScopes = cJSON_GetArraySize(scopes_json);
	tds__GetScopesResponse->Scopes = (struct tt__Scope *)soap_malloc(soap, sizeof(struct tt__Scope) * tds__GetScopesResponse->__sizeScopes);
	memset(tds__GetScopesResponse->Scopes, 0, sizeof(struct tt__Scope) * tds__GetScopesResponse->__sizeScopes);
	for(int i = 0; i < tds__GetScopesResponse->__sizeScopes; i++) {
		cJSON* item = cJSON_GetArrayItem(scopes_json, i);
		if (item == NULL || !cJSON_IsObject(item)) {
			continue;
		}

		SOAP_CJSON_GET_STRING(item, soap, "item", tds__GetScopesResponse->Scopes[i].ScopeItem);
		SOAP_CJSON_GET_NUMBER(item, soap, "def", tds__GetScopesResponse->Scopes[i].ScopeDef);
	}

	cJSON_free(scopes_json);
	return 0;
}
/** Web service operation '__tds__SetScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetScopes(struct soap* soap, struct _tds__SetScopes *tds__SetScopes, struct _tds__SetScopesResponse *tds__SetScopesResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	cJSON* scopes_json = OnvifConfGetConfig("scopes");
	CHECK_POINTER(scopes_json, return 500);
	CHECK_BOOL(cJSON_IsArray(scopes_json), cJSON_free(scopes_json);return 500);

	while(cJSON_GetArraySize(scopes_json) > 0) {
		cJSON_DeleteItemFromArray(scopes_json, 0);
	}

	for(int i = 0; i < tds__SetScopes->__sizeScopes; i++) {
		cJSON* new_json = cJSON_CreateObject();
		CHECK_POINTER(new_json, continue);

		CJSON_SET_STRING(new_json, "item", tds__SetScopes->Scopes[i], cJSON_free(new_json);continue);
		CJSON_SET_NUMBER(new_json, "def", 1, cJSON_free(new_json);continue);

		CHECK_BOOL(cJSON_AddItemToArray(scopes_json, new_json), cJSON_free(new_json);continue);
	}

	OnvifConfSetConfig("scopes", scopes_json);
	cJSON_free(scopes_json);
	return 0;
}
/** Web service operation '__tds__AddScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddScopes(struct soap* soap, struct _tds__AddScopes *tds__AddScopes, struct _tds__AddScopesResponse *tds__AddScopesResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__RemoveScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveScopes(struct soap* soap, struct _tds__RemoveScopes *tds__RemoveScopes, struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDiscoveryMode(struct soap* soap, struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode, struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDiscoveryMode(struct soap* soap, struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode, struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetRemoteDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteDiscoveryMode(struct soap* soap, struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetRemoteDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteDiscoveryMode(struct soap* soap, struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetDPAddresses' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDPAddresses(struct soap* soap, struct _tds__GetDPAddresses *tds__GetDPAddresses, struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetEndpointReference' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetEndpointReference(struct soap* soap, struct _tds__GetEndpointReference *tds__GetEndpointReference, struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetRemoteUser' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteUser(struct soap* soap, struct _tds__GetRemoteUser *tds__GetRemoteUser, struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetRemoteUser' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteUser(struct soap* soap, struct _tds__SetRemoteUser *tds__SetRemoteUser, struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetUsers' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetUsers(struct soap* soap, struct _tds__GetUsers *tds__GetUsers, struct _tds__GetUsersResponse *tds__GetUsersResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	cJSON* users_json = OnvifConfGetConfig("users");
	CHECK_POINTER(users_json, return 500);
	CHECK_BOOL(cJSON_IsArray(users_json), return 500);

	tds__GetUsersResponse->__sizeUser = cJSON_GetArraySize(users_json);
	tds__GetUsersResponse->User = (struct tt__User *)soap_malloc(soap, sizeof(struct tt__User) * tds__GetUsersResponse->__sizeUser);
	memset(tds__GetUsersResponse->User, 0, sizeof(struct tt__User) * tds__GetUsersResponse->__sizeUser);
	for(int i = 0; i < tds__GetUsersResponse->__sizeUser; i++) {
		cJSON* item = cJSON_GetArrayItem(users_json, i);
		if (item == NULL || !cJSON_IsObject(item)) {
			continue;
		}

		SOAP_CJSON_GET_STRING(item, soap, "username", tds__GetUsersResponse->User[i].Username);
		SOAP_CJSON_GET_STRING(item, soap, "password", tds__GetUsersResponse->User[i].Password);
		SOAP_CJSON_GET_NUMBER(item, soap, "user_type", tds__GetUsersResponse->User[i].UserLevel);
	}
	cJSON_free(users_json);

	return 0;
}
/** Web service operation '__tds__CreateUsers' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateUsers(struct soap* soap, struct _tds__CreateUsers *tds__CreateUsers, struct _tds__CreateUsersResponse *tds__CreateUsersResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);
	
	cJSON* users_json = OnvifConfGetConfig("users");
	CHECK_POINTER(users_json, return 500);
	CHECK_BOOL(cJSON_IsArray(users_json), return 500);

	int ret = 0;
	for(int i = 0; i < tds__CreateUsers->__sizeUser; i++) {
		cJSON* user_json = cJSON_CreateObject();
		CHECK_POINTER(user_json, goto end);
		
		CJSON_SET_STRING(user_json, "username", tds__CreateUsers->User[i].Username, goto end);
		CJSON_SET_STRING(user_json, "password", tds__CreateUsers->User[i].Password, goto end);
		CJSON_SET_NUMBER(user_json, "user_type", tds__CreateUsers->User[i].UserLevel, goto end);

		CHECK_BOOL(cJSON_AddItemToArray(users_json, user_json), goto end);
		continue;
end:
		ret = 500;
		if (user_json != NULL) {
			cJSON_free(user_json);
		}
		break;
	}

	OnvifConfSetConfig("users", users_json);
	cJSON_free(users_json);

	return ret;
}

/** Web service operation '__tds__DeleteUsers' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteUsers(struct soap* soap, struct _tds__DeleteUsers *tds__DeleteUsers, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);
	
	cJSON* users_json = OnvifConfGetConfig("users");
	CHECK_POINTER(users_json, return 500);
	CHECK_BOOL(cJSON_IsArray(users_json), return 500);

	for(int i = 0; i < tds__DeleteUsers->__sizeUsername; i++) {
		if (strcmp("admin", tds__DeleteUsers->Username[i]) == 0) {
			SOAP_ERROR_REPLAY(soap, "The user cannot be deleted !", 400);
		}

		for(int j = 0; j < cJSON_GetArraySize(users_json); j++) {
			char username[64] = {0};
			CJSON_GET_STRING(cJSON_GetArrayItem(users_json, j), "username", username, sizeof(username), ERR_OPT_NULL);

			if (strcmp(tds__DeleteUsers->Username[i], username) == 0) {
				cJSON_DeleteItemFromArray(users_json, j);
				break;
			}
		}
	}

	OnvifConfSetConfig("users", users_json);
	cJSON_free(users_json);

	return 0;
}
/** Web service operation '__tds__SetUser' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetUser(struct soap* soap, struct _tds__SetUser *tds__SetUser, struct _tds__SetUserResponse *tds__SetUserResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);
	
	cJSON* users_json = OnvifConfGetConfig("users");
	CHECK_POINTER(users_json, return 500);
	CHECK_BOOL(cJSON_IsArray(users_json), return 500);

	int ret = 500;
	for(int i = 0; i < tds__SetUser->__sizeUser; i++) {
		for(int j = 0; j < cJSON_GetArraySize(users_json); j++) {
			char username[64] = {0};
			CJSON_GET_STRING(cJSON_GetArrayItem(users_json, j), "username", username, sizeof(username), ERR_OPT_NULL);

			if (strcmp(tds__SetUser->User[i].Username, username) == 0) {
				cJSON* user_json = cJSON_CreateObject();
				CHECK_POINTER(user_json, goto end);
				
				CJSON_SET_STRING(user_json, "username", tds__SetUser->User[i].Username, goto end);
				CJSON_SET_STRING(user_json, "password", tds__SetUser->User[i].Password, goto end);
				CJSON_SET_NUMBER(user_json, "level", tds__SetUser->User[i].UserLevel, goto end);

				CHECK_BOOL(cJSON_ReplaceItemInArray(users_json, j, user_json), goto end);
				ret = 0;
				break;
end:
				if (user_json != NULL) {
					cJSON_free(user_json);
				}
				break;
			}
		}
	}

	OnvifConfSetConfig("users", users_json);
	cJSON_free(users_json);

	return ret;
}
/** Web service operation '__tds__GetWsdlUrl' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetWsdlUrl(struct soap* soap, struct _tds__GetWsdlUrl *tds__GetWsdlUrl, struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetPasswordComplexityOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPasswordComplexityOptions(struct soap* soap, struct _tds__GetPasswordComplexityOptions *tds__GetPasswordComplexityOptions, struct _tds__GetPasswordComplexityOptionsResponse *tds__GetPasswordComplexityOptionsResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetPasswordComplexityConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPasswordComplexityConfiguration(struct soap* soap, struct _tds__GetPasswordComplexityConfiguration *tds__GetPasswordComplexityConfiguration, struct _tds__GetPasswordComplexityConfigurationResponse *tds__GetPasswordComplexityConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetPasswordComplexityConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetPasswordComplexityConfiguration(struct soap* soap, struct _tds__SetPasswordComplexityConfiguration *tds__SetPasswordComplexityConfiguration, struct _tds__SetPasswordComplexityConfigurationResponse *tds__SetPasswordComplexityConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetPasswordHistoryConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPasswordHistoryConfiguration(struct soap* soap, struct _tds__GetPasswordHistoryConfiguration *tds__GetPasswordHistoryConfiguration, struct _tds__GetPasswordHistoryConfigurationResponse *tds__GetPasswordHistoryConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetPasswordHistoryConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetPasswordHistoryConfiguration(struct soap* soap, struct _tds__SetPasswordHistoryConfiguration *tds__SetPasswordHistoryConfiguration, struct _tds__SetPasswordHistoryConfigurationResponse *tds__SetPasswordHistoryConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetAuthFailureWarningOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAuthFailureWarningOptions(struct soap* soap, struct _tds__GetAuthFailureWarningOptions *tds__GetAuthFailureWarningOptions, struct _tds__GetAuthFailureWarningOptionsResponse *tds__GetAuthFailureWarningOptionsResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetAuthFailureWarningConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAuthFailureWarningConfiguration(struct soap* soap, struct _tds__GetAuthFailureWarningConfiguration *tds__GetAuthFailureWarningConfiguration, struct _tds__GetAuthFailureWarningConfigurationResponse *tds__GetAuthFailureWarningConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetAuthFailureWarningConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAuthFailureWarningConfiguration(struct soap* soap, struct _tds__SetAuthFailureWarningConfiguration *tds__SetAuthFailureWarningConfiguration, struct _tds__SetAuthFailureWarningConfigurationResponse *tds__SetAuthFailureWarningConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}

static void GetCapabilities(struct soap* soap, enum tt__CapabilityCategory category, struct tt__Capabilities *capabilities) {
	OnvifConfigNetworkTcpIp network_tcpip;
	memset(&network_tcpip, 0, sizeof(OnvifConfigNetworkTcpIp));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return );

	char buff[512] = {0};
	switch (category)
	{
	case tt__CapabilityCategory__Analytics:
		capabilities->Analytics = (struct tt__AnalyticsCapabilities *)soap_malloc(soap, sizeof(struct tt__AnalyticsCapabilities));
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/analytics_service", network_tcpip.ipv4_addr, 3333);
		capabilities->Analytics->XAddr = soap_strdup(soap, buff);
		capabilities->Analytics->RuleSupport = xsd__boolean__false_;
		capabilities->Analytics->AnalyticsModuleSupport = xsd__boolean__false_;
		break;
	case tt__CapabilityCategory__Device:
		capabilities->Device = (struct tt__DeviceCapabilities *)soap_malloc(soap, sizeof(struct tt__DeviceCapabilities));
		memset(capabilities->Device, 0, sizeof(struct tt__DeviceCapabilities));
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/device_service", network_tcpip.ipv4_addr, 3333);
		capabilities->Device->XAddr = soap_strdup(soap, buff);

		capabilities->Device->Network = (struct tt__NetworkCapabilities *)soap_malloc(soap, sizeof(struct tt__NetworkCapabilities));
		SOAP_SET_NUMBER(soap, capabilities->Device->Network->IPFilter, sizeof(enum xsd__boolean), xsd__boolean__false_);
		SOAP_SET_NUMBER(soap, capabilities->Device->Network->ZeroConfiguration, sizeof(enum xsd__boolean), xsd__boolean__false_);
		SOAP_SET_NUMBER(soap, capabilities->Device->Network->IPVersion6, sizeof(enum xsd__boolean), xsd__boolean__false_);
		SOAP_SET_NUMBER(soap, capabilities->Device->Network->DynDNS, sizeof(enum xsd__boolean), xsd__boolean__false_);
		capabilities->Device->Network->Extension = (struct tt__NetworkCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__NetworkCapabilitiesExtension));
		SOAP_SET_NUMBER(soap, capabilities->Device->Network->Extension->Dot11Configuration, sizeof(enum xsd__boolean), xsd__boolean__false_);

		capabilities->Device->System = (struct tt__SystemCapabilities *)soap_malloc(soap, sizeof(struct tt__SystemCapabilities));
		capabilities->Device->System->DiscoveryResolve = xsd__boolean__false_;
		capabilities->Device->System->DiscoveryBye = xsd__boolean__false_;
		capabilities->Device->System->RemoteDiscovery = xsd__boolean__true_;
		capabilities->Device->System->SystemBackup = xsd__boolean__false_;
		capabilities->Device->System->SystemLogging = xsd__boolean__false_;
		capabilities->Device->System->FirmwareUpgrade = xsd__boolean__true_;
		capabilities->Device->System->__sizeSupportedVersions = 1;
		capabilities->Device->System->SupportedVersions = (struct tt__OnvifVersion *)soap_malloc(soap, sizeof(struct tt__OnvifVersion));
		capabilities->Device->System->SupportedVersions->Major = 1;
		capabilities->Device->System->SupportedVersions->Minor = 10;
		capabilities->Device->System->Extension = (struct tt__SystemCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__SystemCapabilitiesExtension));
		SOAP_SET_NUMBER(soap, capabilities->Device->System->Extension->HttpFirmwareUpgrade, sizeof(enum xsd__boolean), xsd__boolean__true_);
		SOAP_SET_NUMBER(soap, capabilities->Device->System->Extension->HttpSystemBackup, sizeof(enum xsd__boolean), xsd__boolean__false_);
		SOAP_SET_NUMBER(soap, capabilities->Device->System->Extension->HttpSystemLogging, sizeof(enum xsd__boolean), xsd__boolean__false_);
		SOAP_SET_NUMBER(soap, capabilities->Device->System->Extension->HttpSupportInformation, sizeof(enum xsd__boolean), xsd__boolean__true_);

		capabilities->Device->IO = (struct tt__IOCapabilities *)soap_malloc(soap, sizeof(struct tt__IOCapabilities));
		SOAP_SET_NUMBER(soap, capabilities->Device->IO->InputConnectors, sizeof(int), 1);
		SOAP_SET_NUMBER(soap, capabilities->Device->IO->RelayOutputs, sizeof(int), 0);
		capabilities->Device->IO->Extension = (struct tt__IOCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__IOCapabilitiesExtension));
		SOAP_SET_NUMBER(soap, capabilities->Device->IO->Extension->Auxiliary, sizeof(enum xsd__boolean), xsd__boolean__false_);
		capabilities->Device->IO->Extension->__sizeAuxiliaryCommands = 0;

		capabilities->Device->Security = (struct tt__SecurityCapabilities *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilities));
		memset(capabilities->Device->Security, 0, sizeof(struct tt__SecurityCapabilities));
		capabilities->Device->Security->TLS1_x002e1 = xsd__boolean__false_;
		capabilities->Device->Security->TLS1_x002e2 = xsd__boolean__false_;
		capabilities->Device->Security->OnboardKeyGeneration = xsd__boolean__false_;
		capabilities->Device->Security->AccessPolicyConfig = xsd__boolean__false_;
		capabilities->Device->Security->X_x002e509Token = xsd__boolean__false_;
		capabilities->Device->Security->SAMLToken = xsd__boolean__false_;
		capabilities->Device->Security->KerberosToken = xsd__boolean__false_;
		capabilities->Device->Security->RELToken = xsd__boolean__false_;
		capabilities->Device->Security->Extension = (struct tt__SecurityCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilitiesExtension));
		capabilities->Device->Security->Extension->TLS1_x002e0 = xsd__boolean__false_;
		capabilities->Device->Security->Extension->Extension = (struct tt__SecurityCapabilitiesExtension2 *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilitiesExtension2));
		memset(capabilities->Device->Security->Extension->Extension, 0, sizeof(struct tt__SecurityCapabilitiesExtension2));
		capabilities->Device->Security->Extension->Extension->Dot1X = xsd__boolean__false_;
		capabilities->Device->Security->Extension->Extension->RemoteUserHandling = xsd__boolean__false_;
		break;
	case tt__CapabilityCategory__Events:
		capabilities->Events = (struct tt__EventCapabilities *)soap_malloc(soap, sizeof(struct tt__EventCapabilities));
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/event_service", network_tcpip.ipv4_addr, 3333);
		capabilities->Events->XAddr = soap_strdup(soap, buff);
		capabilities->Events->WSPausableSubscriptionManagerInterfaceSupport = xsd__boolean__true_;
		capabilities->Events->WSPullPointSupport = xsd__boolean__true_;
		capabilities->Events->WSSubscriptionPolicySupport = xsd__boolean__false_;
		break;
	case tt__CapabilityCategory__Imaging:
		capabilities->Imaging = (struct tt__ImagingCapabilities *)soap_malloc(soap, sizeof(struct tt__ImagingCapabilities));
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/imaging_service", network_tcpip.ipv4_addr, 3333);
		capabilities->Imaging->XAddr = soap_strdup(soap, buff);
		break;
	case tt__CapabilityCategory__Media:
		capabilities->Media = (struct tt__MediaCapabilities *)soap_malloc(soap, sizeof(struct tt__MediaCapabilities));
		memset(capabilities->Media, 0, sizeof(struct tt__MediaCapabilities));
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/media_service", network_tcpip.ipv4_addr, 3333);
		capabilities->Media->XAddr = soap_strdup(soap, buff);
		capabilities->Media->StreamingCapabilities = (struct tt__RealTimeStreamingCapabilities *)soap_malloc(soap, sizeof(struct tt__RealTimeStreamingCapabilities));
		SOAP_SET_NUMBER(soap, capabilities->Media->StreamingCapabilities->RTPMulticast, sizeof(enum xsd__boolean), xsd__boolean__false_);
		SOAP_SET_NUMBER(soap, capabilities->Media->StreamingCapabilities->RTP_USCORETCP, sizeof(enum xsd__boolean), xsd__boolean__true_);
		SOAP_SET_NUMBER(soap, capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP, sizeof(enum xsd__boolean), xsd__boolean__true_);
		capabilities->Media->Extension = (struct tt__MediaCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__MediaCapabilitiesExtension));
		capabilities->Media->Extension->ProfileCapabilities = (struct tt__ProfileCapabilities *)soap_malloc(soap, sizeof(struct tt__ProfileCapabilities));
		capabilities->Media->Extension->ProfileCapabilities->MaximumNumberOfProfiles = 1;
		break;
	case tt__CapabilityCategory__PTZ:
		capabilities->PTZ = (struct tt__PTZCapabilities *)soap_malloc(soap, sizeof(struct tt__PTZCapabilities));
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/ptz_service", network_tcpip.ipv4_addr, 3333);
		capabilities->PTZ->XAddr = soap_strdup(soap, buff);
		break;
	default:
		capabilities->Extension = (struct tt__CapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__CapabilitiesExtension));
		memset(capabilities->Extension, 0, sizeof(struct tt__CapabilitiesExtension));

		capabilities->Extension->DeviceIO = (struct tt__DeviceIOCapabilities *)soap_malloc(soap, sizeof(struct tt__DeviceIOCapabilities)); 
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/device_io_service", network_tcpip.ipv4_addr, 3333);
		capabilities->Extension->DeviceIO->XAddr = soap_strdup(soap, buff);
		capabilities->Extension->DeviceIO->VideoSources = 1;
		capabilities->Extension->DeviceIO->VideoOutputs = 1;
		capabilities->Extension->DeviceIO->AudioSources = 1;
		capabilities->Extension->DeviceIO->AudioOutputs = 1;
		capabilities->Extension->DeviceIO->RelayOutputs = 0;

		capabilities->Extension->Display = (struct tt__DisplayCapabilities *)soap_malloc(soap, sizeof(struct tt__DisplayCapabilities)); 
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/display_service", network_tcpip.ipv4_addr, 3333);
		capabilities->Extension->Display->XAddr = soap_strdup(soap, buff);
		capabilities->Extension->Display->FixedLayout = xsd__boolean__true_;

		capabilities->Extension->Recording = (struct tt__RecordingCapabilities *)soap_malloc(soap, sizeof(struct tt__RecordingCapabilities));
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/recording_service", network_tcpip.ipv4_addr, 3333);
		capabilities->Extension->Recording->XAddr = soap_strdup(soap, buff);
		capabilities->Extension->Recording->ReceiverSource = xsd__boolean__false_;
		capabilities->Extension->Recording->MediaProfileSource = xsd__boolean__false_;
		capabilities->Extension->Recording->DynamicRecordings = xsd__boolean__false_;
		capabilities->Extension->Recording->DynamicTracks = xsd__boolean__false_;
		capabilities->Extension->Recording->MaxStringLength = 0;

		capabilities->Extension->Search = (struct tt__SearchCapabilities *)soap_malloc(soap, sizeof(struct tt__SearchCapabilities)); 
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/search_service", network_tcpip.ipv4_addr, 3333);
		capabilities->Extension->Search->XAddr = soap_strdup(soap, buff);
		capabilities->Extension->Search->MetadataSearch = xsd__boolean__false_;

		capabilities->Extension->Replay = (struct tt__ReplayCapabilities *)soap_malloc(soap, sizeof(struct tt__ReplayCapabilities)); 
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/replay_service", network_tcpip.ipv4_addr, 3333);
		capabilities->Extension->Replay->XAddr = soap_strdup(soap, buff);

		capabilities->Extension->Receiver = (struct tt__ReceiverCapabilities *)soap_malloc(soap, sizeof(struct tt__ReceiverCapabilities)); 
		snprintf(buff, sizeof(buff), "http://%s:%d/onvif/receiver_service", network_tcpip.ipv4_addr, 3333);
		capabilities->Extension->Receiver->XAddr = soap_strdup(soap, buff);
		capabilities->Extension->Receiver->RTP_USCOREMulticast = xsd__boolean__false_;
		capabilities->Extension->Receiver->RTP_USCORETCP = xsd__boolean__false_;
		capabilities->Extension->Receiver->RTP_USCORERTSP_USCORETCP = xsd__boolean__false_;
		capabilities->Extension->Receiver->SupportedReceivers = 0;
		capabilities->Extension->Receiver->MaximumRTSPURILength = 1;

		// capabilities->Extension->AnalyticsDevice = (struct tt__AnalyticsDeviceCapabilities *)soap_malloc(soap, sizeof(struct tt__AnalyticsDeviceCapabilities)); 
		// snprintf(buff, sizeof(buff), "http://%s:%d/onvif/analytics_device_service", dev_info.device_addr, dev_info.web_server_port);
		// capabilities->Extension->AnalyticsDevice->XAddr = soap_strdup(soap, buff);
		// SOAP_SET_NUMBER(soap, capabilities->Extension->AnalyticsDevice->RuleSupport, sizeof(enum xsd__boolean), xsd__boolean__false_);
		break;
	}
}
/** Web service operation '__tds__GetCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCapabilities(struct soap* soap, struct _tds__GetCapabilities *tds__GetCapabilities, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse) {
	tds__GetCapabilitiesResponse->Capabilities = (struct tt__Capabilities *)soap_malloc(soap, sizeof(struct tt__Capabilities));
	memset(tds__GetCapabilitiesResponse->Capabilities, 0, sizeof(struct tt__Capabilities));
	for(int i = 0; i < tds__GetCapabilities->__sizeCategory; i++) {
		if (tds__GetCapabilities->Category[i] == tt__CapabilityCategory__All) {
			// GetCapabilities(soap, tt__CapabilityCategory__Analytics, tds__GetCapabilitiesResponse->Capabilities);
			GetCapabilities(soap, tt__CapabilityCategory__Device, tds__GetCapabilitiesResponse->Capabilities);
			GetCapabilities(soap, tt__CapabilityCategory__Events, tds__GetCapabilitiesResponse->Capabilities);
			GetCapabilities(soap, tt__CapabilityCategory__Imaging, tds__GetCapabilitiesResponse->Capabilities);
			GetCapabilities(soap, tt__CapabilityCategory__Media, tds__GetCapabilitiesResponse->Capabilities);
			GetCapabilities(soap, tt__CapabilityCategory__PTZ, tds__GetCapabilitiesResponse->Capabilities);
			GetCapabilities(soap, 0, tds__GetCapabilitiesResponse->Capabilities);
		} else {
			GetCapabilities(soap, tds__GetCapabilities->Category[i], tds__GetCapabilitiesResponse->Capabilities);
		}
	}
	return 0;
}
/** Web service operation '__tds__SetDPAddresses' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDPAddresses(struct soap* soap, struct _tds__SetDPAddresses *tds__SetDPAddresses, struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetHostname' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetHostname(struct soap* soap, struct _tds__GetHostname *tds__GetHostname, struct _tds__GetHostnameResponse *tds__GetHostnameResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetHostname' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostname(struct soap* soap, struct _tds__SetHostname *tds__SetHostname, struct _tds__SetHostnameResponse *tds__SetHostnameResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetHostnameFromDHCP' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostnameFromDHCP(struct soap* soap, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDNS(struct soap* soap, struct _tds__GetDNS *tds__GetDNS, struct _tds__GetDNSResponse *tds__GetDNSResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigNetworkTcpIp network_tcpip;
	memset(&network_tcpip, 0, sizeof(OnvifConfigNetworkTcpIp));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

	tds__GetDNSResponse->DNSInformation = (struct tt__DNSInformation*)soap_malloc(soap, sizeof(struct tt__DNSInformation));
	memset(tds__GetDNSResponse->DNSInformation, 0, sizeof(struct tt__DNSInformation));
	tds__GetDNSResponse->DNSInformation->FromDHCP = network_tcpip.dhcp_enable;

	tds__GetDNSResponse->DNSInformation->__sizeDNSManual = 0;
	if (strlen(network_tcpip.dns) != 0) {
		tds__GetDNSResponse->DNSInformation->__sizeDNSManual += 1;
	}

	if (strlen(network_tcpip.alt_dns) != 0) {
		tds__GetDNSResponse->DNSInformation->__sizeDNSManual += 1;
	}
	
	tds__GetDNSResponse->DNSInformation->DNSManual = (struct tt__IPAddress*)soap_malloc(soap, sizeof(struct tt__IPAddress)*tds__GetDNSResponse->DNSInformation->__sizeDNSManual);
	memset(tds__GetDNSResponse->DNSInformation->DNSManual, 0, sizeof(struct tt__IPAddress)*tds__GetDNSResponse->DNSInformation->__sizeDNSManual);

	if (strlen(network_tcpip.dns) != 0) {
		tds__GetDNSResponse->DNSInformation->DNSManual[0].Type = tt__IPType__IPv4;
		tds__GetDNSResponse->DNSInformation->DNSManual[0].IPv4Address = soap_strdup(soap, network_tcpip.dns);
	}

	if (strlen(network_tcpip.alt_dns) != 0) {
		tds__GetDNSResponse->DNSInformation->DNSManual[1].Type = tt__IPType__IPv4;
		tds__GetDNSResponse->DNSInformation->DNSManual[1].IPv4Address = soap_strdup(soap, network_tcpip.alt_dns);
	}

	return 0;
}
/** Web service operation '__tds__SetDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDNS(struct soap* soap, struct _tds__SetDNS *tds__SetDNS, struct _tds__SetDNSResponse *tds__SetDNSResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigNetworkTcpIp network_tcpip;
	memset(&network_tcpip, 0, sizeof(OnvifConfigNetworkTcpIp));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

	for(int i = 0; i < tds__SetDNS->__sizeDNSManual && i < 2; i++) {
		if (tds__SetDNS->DNSManual[i].Type == tt__IPType__IPv4 && tds__SetDNS->DNSManual[i].IPv4Address != NULL) {
			if (i == 0) {
				strncpy(network_tcpip.dns, tds__SetDNS->DNSManual[i].IPv4Address, sizeof(network_tcpip.dns)-1);
			} else if (i == 1) {
				strncpy(network_tcpip.alt_dns, tds__SetDNS->DNSManual[i].IPv4Address, sizeof(network_tcpip.alt_dns)-1);
			}
		}
	}

	ret = OnvifOperationSetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

	return 0;
}
/** Web service operation '__tds__GetNTP' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNTP(struct soap* soap, struct _tds__GetNTP *tds__GetNTP, struct _tds__GetNTPResponse *tds__GetNTPResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigNetworkNtpInfo network_ntp_info;
	memset(&network_ntp_info, 0, sizeof(OnvifConfigNetworkNtpInfo));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_NTP_INFO, &network_ntp_info, sizeof(OnvifConfigNetworkNtpInfo));
	CHECK_LT(ret, 0, return 500);

	tds__GetNTPResponse->NTPInformation = (struct tt__NTPInformation*)soap_malloc(soap, sizeof(struct tt__NTPInformation));
	memset(tds__GetNTPResponse->NTPInformation, 0, sizeof(struct tt__NTPInformation));

	if (strlen(network_ntp_info.server_addr) != 0) {
		tds__GetNTPResponse->NTPInformation->__sizeNTPManual = 1;
		tds__GetNTPResponse->NTPInformation->NTPManual = (struct tt__NetworkHost*)soap_malloc(soap, sizeof(struct tt__NetworkHost)*tds__GetNTPResponse->NTPInformation->__sizeNTPManual);
		memset(tds__GetNTPResponse->NTPInformation->NTPManual, 0, sizeof(struct tt__NetworkHost)*tds__GetNTPResponse->NTPInformation->__sizeNTPManual);
		tds__GetNTPResponse->NTPInformation->NTPManual[0].Type = tt__NetworkHostType__DNS;
		tds__GetNTPResponse->NTPInformation->NTPManual[0].DNSname = soap_strdup(soap, network_ntp_info.server_addr);
	}

	return 0;
}
/** Web service operation '__tds__SetNTP' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNTP(struct soap* soap, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigNetworkNtpInfo network_ntp_info;
	memset(&network_ntp_info, 0, sizeof(OnvifConfigNetworkNtpInfo));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_NTP_INFO, &network_ntp_info, sizeof(OnvifConfigNetworkNtpInfo));
	CHECK_LT(ret, 0, return 500);

	if (tds__SetNTP->__sizeNTPManual != 0 && tds__SetNTP->NTPManual != NULL && tds__SetNTP->NTPManual[0].DNSname != NULL) {
		strncpy(network_ntp_info.server_addr, tds__SetNTP->NTPManual[0].DNSname, sizeof(network_ntp_info.server_addr)-1);
	}

	ret = OnvifOperationSetConfig(ONVIF_CONFIG_NETWORK_NTP_INFO, &network_ntp_info, sizeof(OnvifConfigNetworkNtpInfo));
	CHECK_LT(ret, 0, return 500);

	return 0;
}
/** Web service operation '__tds__GetDynamicDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDynamicDNS(struct soap* soap, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetDynamicDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDynamicDNS(struct soap* soap, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
static int MaskToNetworkByte(char* mask) {
	int mask_num[4] = {0};
	sscanf(mask, "%d.%d.%d.%d", &mask_num[0], &mask_num[1], &mask_num[2], &mask_num[3]);
	int net_byte = 0;
	for(int i = 0; i < 4; i++) {
		if (mask_num[i] == 0xff) {
			net_byte += 8;
		} else {
			for(int j = 0; j < 8; j++) {
				if (mask_num[i] & (1 << j)) {
					net_byte += (8 - j);
					break;
				}
			}
			break;
		}
	}
	return net_byte;
}
/** Web service operation '__tds__GetNetworkInterfaces' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkInterfaces(struct soap* soap, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigNetworkTcpIp network_tcpip;
	memset(&network_tcpip, 0, sizeof(OnvifConfigNetworkTcpIp));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

	tds__GetNetworkInterfacesResponse->__sizeNetworkInterfaces = 1;
	tds__GetNetworkInterfacesResponse->NetworkInterfaces = (struct tt__NetworkInterface*)soap_malloc(soap, sizeof(struct tt__NetworkInterface)*tds__GetNetworkInterfacesResponse->__sizeNetworkInterfaces);
	memset(tds__GetNetworkInterfacesResponse->NetworkInterfaces, 0, sizeof(struct tt__NetworkInterface)*tds__GetNetworkInterfacesResponse->__sizeNetworkInterfaces);
	for(int i = 0; i < tds__GetNetworkInterfacesResponse->__sizeNetworkInterfaces; i++) {
		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].token = soap_strdup(soap, "network_interfaces_t");
		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].Enabled = xsd__boolean__true_;

		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].Info = (struct tt__NetworkInterfaceInfo*)soap_malloc(soap, sizeof(struct tt__NetworkInterfaceInfo));
		memset(tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].Info, 0, sizeof(struct tt__NetworkInterfaceInfo));
		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].Info->Name = soap_strdup(soap, "eth0");
		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].Info->HwAddress = soap_strdup(soap, network_tcpip.mac);

		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv4 = (struct tt__IPv4NetworkInterface*)soap_malloc(soap, sizeof(struct tt__IPv4NetworkInterface));
		memset(tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv4, 0, sizeof(struct tt__IPv4NetworkInterface));
		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv4->Enabled = network_tcpip.ipv4_enable;
		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv4->Config = (struct tt__IPv4Configuration*)soap_malloc(soap, sizeof(struct tt__IPv4Configuration));
		memset(tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv4->Config, 0, sizeof(struct tt__IPv4Configuration));

		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv4->Config->DHCP = network_tcpip.dhcp_enable;

		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv4->Config->FromDHCP = (struct tt__PrefixedIPv4Address*)soap_malloc(soap, sizeof(struct tt__PrefixedIPv4Address));
		memset(tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv4->Config->FromDHCP, 0, sizeof(struct tt__PrefixedIPv4Address));
		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv4->Config->FromDHCP->Address = soap_strdup(soap, network_tcpip.ipv4_addr);
		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv4->Config->FromDHCP->PrefixLength = MaskToNetworkByte(network_tcpip.ipv4_mask);

		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv6 = (struct tt__IPv6NetworkInterface*)soap_malloc(soap, sizeof(struct tt__IPv6NetworkInterface));
		memset(tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv6, 0, sizeof(struct tt__IPv6NetworkInterface));
		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv6->Enabled = network_tcpip.ipv6_enable;

		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv6->Config = (struct tt__IPv6Configuration*)soap_malloc(soap, sizeof(struct tt__IPv6Configuration));
		memset(tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv6->Config, 0, sizeof(struct tt__IPv6Configuration));

		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv6->Config->DHCP = network_tcpip.dhcp_enable;

		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv6->Config->__sizeFromDHCP = 1;
		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv6->Config->FromDHCP = (struct tt__PrefixedIPv6Address*)soap_malloc(soap, sizeof(struct tt__PrefixedIPv6Address));
		memset(tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv6->Config->FromDHCP, 0, sizeof(struct tt__PrefixedIPv6Address));

		tds__GetNetworkInterfacesResponse->NetworkInterfaces[i].IPv6->Config->FromDHCP->Address = soap_strdup(soap, network_tcpip.ipv6_addr);
	}

	return 0;
}

static void NetworkByteToMask(int net_byte, char* mask, int mask_size) {
	int num = 0;
	for(int i = 0; i < 32; i++) {
		num = (i < net_byte) ? (num << 1) | 1 : num << 1;
	}

	char mask_num[4] = {0};
	memcpy(mask_num, &num, 4);
	snprintf(mask, mask_size, "%d.%d.%d.%d", mask_num[3], mask_num[2], mask_num[1], mask_num[0]);
}
/** Web service operation '__tds__SetNetworkInterfaces' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkInterfaces(struct soap* soap, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigNetworkTcpIp network_tcpip;
	memset(&network_tcpip, 0, sizeof(OnvifConfigNetworkTcpIp));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

	if(tds__SetNetworkInterfaces->NetworkInterface != NULL && tds__SetNetworkInterfaces->NetworkInterface->IPv4 != NULL) {
		if (tds__SetNetworkInterfaces->NetworkInterface->IPv4->Enabled != NULL) {
			network_tcpip.ipv4_enable = *(tds__SetNetworkInterfaces->NetworkInterface->IPv4->Enabled);
		}

		if (tds__SetNetworkInterfaces->NetworkInterface->IPv4->DHCP != NULL) {
			network_tcpip.dhcp_enable = *(tds__SetNetworkInterfaces->NetworkInterface->IPv4->DHCP);
		}

		if (tds__SetNetworkInterfaces->NetworkInterface->IPv4->__sizeManual > 0 && tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual != NULL) {
			strcpy(network_tcpip.ipv4_addr, tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual[0].Address);
			NetworkByteToMask(tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual[0].PrefixLength, network_tcpip.ipv4_mask, sizeof(network_tcpip.ipv4_mask));
		}
	}
	
	if(tds__SetNetworkInterfaces->NetworkInterface != NULL && tds__SetNetworkInterfaces->NetworkInterface->IPv6 != NULL) {
		if (tds__SetNetworkInterfaces->NetworkInterface->IPv6->Enabled != NULL) {
			network_tcpip.ipv6_enable = *(tds__SetNetworkInterfaces->NetworkInterface->IPv6->Enabled);
		}

		if (tds__SetNetworkInterfaces->NetworkInterface->IPv6->DHCP != NULL) {
			network_tcpip.dhcp_enable = *(tds__SetNetworkInterfaces->NetworkInterface->IPv6->DHCP);
		}

		if (tds__SetNetworkInterfaces->NetworkInterface->IPv6->__sizeManual > 0 && tds__SetNetworkInterfaces->NetworkInterface->IPv6->Manual != NULL) {
			strcpy(network_tcpip.ipv6_addr, tds__SetNetworkInterfaces->NetworkInterface->IPv6->Manual[0].Address);
		}
	}

	ret = OnvifOperationSetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

	return 0;
}
/** Web service operation '__tds__GetNetworkProtocols' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkProtocols(struct soap* soap, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	tds__GetNetworkProtocolsResponse->__sizeNetworkProtocols = 3;
	tds__GetNetworkProtocolsResponse->NetworkProtocols = soap_malloc(soap, sizeof(struct tt__NetworkProtocol)*tds__GetNetworkProtocolsResponse->__sizeNetworkProtocols);
	memset(tds__GetNetworkProtocolsResponse->NetworkProtocols, 0, sizeof(struct tt__NetworkProtocol)*tds__GetNetworkProtocolsResponse->__sizeNetworkProtocols);
	tds__GetNetworkProtocolsResponse->NetworkProtocols[0].Enabled = xsd__boolean__true_;
	tds__GetNetworkProtocolsResponse->NetworkProtocols[0].Name = tt__NetworkProtocolType__HTTP;
	tds__GetNetworkProtocolsResponse->NetworkProtocols[0].__sizePort = 1;
	tds__GetNetworkProtocolsResponse->NetworkProtocols[0].Port = soap_malloc(soap, sizeof(int)*tds__GetNetworkProtocolsResponse->NetworkProtocols[0].__sizePort);
	tds__GetNetworkProtocolsResponse->NetworkProtocols[0].Port[0] = 8080;

	tds__GetNetworkProtocolsResponse->NetworkProtocols[1].Enabled = xsd__boolean__false_;
	tds__GetNetworkProtocolsResponse->NetworkProtocols[1].Name = tt__NetworkProtocolType__HTTPS;
	tds__GetNetworkProtocolsResponse->NetworkProtocols[1].__sizePort = 1;
	tds__GetNetworkProtocolsResponse->NetworkProtocols[1].Port = soap_malloc(soap, sizeof(int)*tds__GetNetworkProtocolsResponse->NetworkProtocols[1].__sizePort);
	tds__GetNetworkProtocolsResponse->NetworkProtocols[1].Port[0] = 8081;

	tds__GetNetworkProtocolsResponse->NetworkProtocols[2].Enabled = xsd__boolean__false_;
	tds__GetNetworkProtocolsResponse->NetworkProtocols[2].Name = tt__NetworkProtocolType__RTSP;
	tds__GetNetworkProtocolsResponse->NetworkProtocols[2].__sizePort = 1;
	tds__GetNetworkProtocolsResponse->NetworkProtocols[2].Port = soap_malloc(soap, sizeof(int)*tds__GetNetworkProtocolsResponse->NetworkProtocols[2].__sizePort);
	tds__GetNetworkProtocolsResponse->NetworkProtocols[2].Port[0] = 554;

	return 0;
}
/** Web service operation '__tds__SetNetworkProtocols' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkProtocols(struct soap* soap, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);
	// TODO
	return 0;
}
/** Web service operation '__tds__GetNetworkDefaultGateway' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkDefaultGateway(struct soap* soap, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigNetworkTcpIp network_tcpip;
	memset(&network_tcpip, 0, sizeof(OnvifConfigNetworkTcpIp));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

	tds__GetNetworkDefaultGatewayResponse->NetworkGateway = (struct tt__NetworkGateway*)soap_malloc(soap, sizeof(struct tt__NetworkGateway));
	memset(tds__GetNetworkDefaultGatewayResponse->NetworkGateway, 0, sizeof(struct tt__NetworkGateway));
	if (network_tcpip.ipv4_enable) {
		tds__GetNetworkDefaultGatewayResponse->NetworkGateway->__sizeIPv4Address = 1;
		tds__GetNetworkDefaultGatewayResponse->NetworkGateway->IPv4Address = (char**)soap_malloc(soap, sizeof(char*));
		tds__GetNetworkDefaultGatewayResponse->NetworkGateway->IPv4Address[0] = soap_strdup(soap, network_tcpip.ipv4_gateway);
	}
	
	if (network_tcpip.ipv6_enable) {
		tds__GetNetworkDefaultGatewayResponse->NetworkGateway->__sizeIPv6Address = 1;
		tds__GetNetworkDefaultGatewayResponse->NetworkGateway->IPv6Address = (char**)soap_malloc(soap, sizeof(char*));
		tds__GetNetworkDefaultGatewayResponse->NetworkGateway->IPv6Address[0] = soap_strdup(soap, network_tcpip.ipv6_gateway);
	}

	return 0;
}
/** Web service operation '__tds__SetNetworkDefaultGateway' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkDefaultGateway(struct soap* soap, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse) {
	CHECK_LT(OnvifAuthUser(soap), 0, return 401);

	OnvifConfigNetworkTcpIp network_tcpip;
	memset(&network_tcpip, 0, sizeof(OnvifConfigNetworkTcpIp));
	int ret = OnvifOperationGetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

	if (tds__SetNetworkDefaultGateway->__sizeIPv4Address > 0 && tds__SetNetworkDefaultGateway->IPv4Address != NULL) {
		strcpy(network_tcpip.ipv4_gateway, tds__SetNetworkDefaultGateway->IPv4Address[0]);
	}

	if (tds__SetNetworkDefaultGateway->__sizeIPv6Address > 0 && tds__SetNetworkDefaultGateway->IPv6Address != NULL) {
		strcpy(network_tcpip.ipv6_gateway, tds__SetNetworkDefaultGateway->IPv6Address[0]);
	}

	ret = OnvifOperationSetConfig(ONVIF_CONFIG_NETWORK_TCP_IP, &network_tcpip, sizeof(OnvifConfigNetworkTcpIp));
	CHECK_LT(ret, 0, return 500);

	return 0;
}
/** Web service operation '__tds__GetZeroConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetZeroConfiguration(struct soap* soap, struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration, struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetZeroConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetZeroConfiguration(struct soap* soap, struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration, struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetIPAddressFilter(struct soap* soap, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetIPAddressFilter(struct soap* soap, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__AddIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddIPAddressFilter(struct soap* soap, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__RemoveIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveIPAddressFilter(struct soap* soap, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetAccessPolicy' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAccessPolicy(struct soap* soap, struct _tds__GetAccessPolicy *tds__GetAccessPolicy, struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetAccessPolicy' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAccessPolicy(struct soap* soap, struct _tds__SetAccessPolicy *tds__SetAccessPolicy, struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__CreateCertificate' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateCertificate(struct soap* soap, struct _tds__CreateCertificate *tds__CreateCertificate, struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetCertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificates(struct soap* soap, struct _tds__GetCertificates *tds__GetCertificates, struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetCertificatesStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificatesStatus(struct soap* soap, struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus, struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetCertificatesStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetCertificatesStatus(struct soap* soap, struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus, struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__DeleteCertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteCertificates(struct soap* soap, struct _tds__DeleteCertificates *tds__DeleteCertificates, struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetPkcs10Request' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPkcs10Request(struct soap* soap, struct _tds__GetPkcs10Request *tds__GetPkcs10Request, struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__LoadCertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificates(struct soap* soap, struct _tds__LoadCertificates *tds__LoadCertificates, struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetClientCertificateMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetClientCertificateMode(struct soap* soap, struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode, struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetClientCertificateMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetClientCertificateMode(struct soap* soap, struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode, struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetRelayOutputs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRelayOutputs(struct soap* soap, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetRelayOutputSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputSettings(struct soap* soap, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetRelayOutputState' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputState(struct soap* soap, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SendAuxiliaryCommand' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SendAuxiliaryCommand(struct soap* soap, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetCACertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCACertificates(struct soap* soap, struct _tds__GetCACertificates *tds__GetCACertificates, struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__LoadCertificateWithPrivateKey' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificateWithPrivateKey(struct soap* soap, struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetCertificateInformation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificateInformation(struct soap* soap, struct _tds__GetCertificateInformation *tds__GetCertificateInformation, struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__LoadCACertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCACertificates(struct soap* soap, struct _tds__LoadCACertificates *tds__LoadCACertificates, struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__CreateDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateDot1XConfiguration(struct soap* soap, struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDot1XConfiguration(struct soap* soap, struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfiguration(struct soap* soap, struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetDot1XConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfigurations(struct soap* soap, struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__DeleteDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteDot1XConfiguration(struct soap* soap, struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetDot11Capabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Capabilities(struct soap* soap, struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities, struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetDot11Status' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Status(struct soap* soap, struct _tds__GetDot11Status *tds__GetDot11Status, struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__ScanAvailableDot11Networks' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__ScanAvailableDot11Networks(struct soap* soap, struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetSystemUris' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemUris(struct soap* soap, struct _tds__GetSystemUris *tds__GetSystemUris, struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__StartFirmwareUpgrade' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartFirmwareUpgrade(struct soap* soap, struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__StartSystemRestore' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartSystemRestore(struct soap* soap, struct _tds__StartSystemRestore *tds__StartSystemRestore, struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetStorageConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetStorageConfigurations(struct soap* soap, struct _tds__GetStorageConfigurations *tds__GetStorageConfigurations, struct _tds__GetStorageConfigurationsResponse *tds__GetStorageConfigurationsResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__CreateStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateStorageConfiguration(struct soap* soap, struct _tds__CreateStorageConfiguration *tds__CreateStorageConfiguration, struct _tds__CreateStorageConfigurationResponse *tds__CreateStorageConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetStorageConfiguration(struct soap* soap, struct _tds__GetStorageConfiguration *tds__GetStorageConfiguration, struct _tds__GetStorageConfigurationResponse *tds__GetStorageConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetStorageConfiguration(struct soap* soap, struct _tds__SetStorageConfiguration *tds__SetStorageConfiguration, struct _tds__SetStorageConfigurationResponse *tds__SetStorageConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__DeleteStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteStorageConfiguration(struct soap* soap, struct _tds__DeleteStorageConfiguration *tds__DeleteStorageConfiguration, struct _tds__DeleteStorageConfigurationResponse *tds__DeleteStorageConfigurationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__GetGeoLocation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetGeoLocation(struct soap* soap, struct _tds__GetGeoLocation *tds__GetGeoLocation, struct _tds__GetGeoLocationResponse *tds__GetGeoLocationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetGeoLocation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetGeoLocation(struct soap* soap, struct _tds__SetGeoLocation *tds__SetGeoLocation, struct _tds__SetGeoLocationResponse *tds__SetGeoLocationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__DeleteGeoLocation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteGeoLocation(struct soap* soap, struct _tds__DeleteGeoLocation *tds__DeleteGeoLocation, struct _tds__DeleteGeoLocationResponse *tds__DeleteGeoLocationResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
/** Web service operation '__tds__SetHashingAlgorithm' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHashingAlgorithm(struct soap* soap, struct _tds__SetHashingAlgorithm *tds__SetHashingAlgorithm, struct _tds__SetHashingAlgorithmResponse *tds__SetHashingAlgorithmResponse) {
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}
