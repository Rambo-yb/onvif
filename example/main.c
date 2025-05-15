#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

#include "onvif.h"

typedef struct {
    OnvifConfigCameraVideoEncodeInfos video_encode_infos;  
    OnvifConfigCameraAudioEncodeInfo audio_encode_info;  
	OnvifConfigCameraImageInfos image_infos;
	OnvifConfigCameraOsdInfos osd_infos;
	OnvifConfigPtzPresets ptz_presets;
}Mng;
static Mng kMng = {
	.video_encode_infos = {
		.cam_num = 2, 
		.stream_num = {1, 1},
		.encode_info[0][0] = {
			.stream_type = ONVIF_CONFIG_CAMERA_VIDEO_STREAM_MAIN, 
			.resolution = {.width = 640, .height = 512},
			.fps = 25,
			.gop = 30,
			.quality = ONVIF_CONFIG_CAMERA_VIDEO_QUALITY_MID,
			.bitrate_max = 2048,
			.bitrate_type = ONVIF_CONFIG_CAMERA_VIDEO_BITRATE_CBR,
			.encode_type = ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_H264
		},
		.encode_info[1][0] = {
			.stream_type = ONVIF_CONFIG_CAMERA_VIDEO_STREAM_MAIN, 
			.resolution = {.width = 1024, .height = 768},
			.fps = 30,
			.gop = 30,
			.quality = ONVIF_CONFIG_CAMERA_VIDEO_QUALITY_MID,
			.bitrate_max = 2048,
			.bitrate_type = ONVIF_CONFIG_CAMERA_VIDEO_BITRATE_CBR,
			.encode_type = ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_H264
		}
	},
	.audio_encode_info = {
		.channel = 1, 
		.encode_type = ONVIF_CONFIG_CAMERA_AUDIO_ENCODE_G711, 
		.bitrate = 8, 
		.sample_rate = 16000
	},
	.image_infos = {
		.num = 2,
		.image_info[0] = {
			.brightness = 50.0,
			.contrast = 70.0,
			.saturation = 50.0,
			.sharpness = 60.0
		},
		.image_info[1] = {
			.brightness = 80.0,
			.contrast = 50.0,
			.saturation = 50.0,
			.sharpness = 30.0
		}
	},
	.osd_infos = {
		.num = 2,
		.osd_info[0] = {
			.camera_name = "camera_0",
			.camera_name_pos = {.x = 0, .y = 0},
			.show_camera_name = true,

			.show_date = true,
			.date_pos = {.x = -1, .y = 0},

			.string_overlay[0] = {
				.show = true, .pos = {.x = 0, .y = -0.7}, .value = "1122"
			}
		},
		.osd_info[1] = {
			.camera_name = "camera_1",
			.camera_name_pos = {.x = 0, .y = 0},
			.show_camera_name = true,

			.show_date = true,
			.date_pos = {.x = -1, .y = 0.7},

			.string_overlay[0] = {
				.show = true, .pos = {.x = -0.9, .y = -0.8}, .value = "11223"
			}
		}
	},
	.ptz_presets = {
		.preset_num = 2,
		.preset[0].name = {"preset_0"},
		.preset[1].name = {"preset_1"}
	}
};

int GetLocalAddr(const char* ethname, char* addr, int size) {
    int socket_fd = socket(AF_INET,SOCK_DGRAM,0); 
    if(socket_fd < 0) {
        perror("socket error!\n");
        return -1;
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(struct ifreq));
    strcpy(ifr.ifr_name, ethname);
    if(ioctl(socket_fd, SIOCGIFADDR, &ifr) < 0)
    {
        perror("ioctl error\n");
        close(socket_fd);
        return -1;
    }
    
    struct sockaddr_in *sin = (struct sockaddr_in *)&(ifr.ifr_addr);
    snprintf(addr, size, "%s", inet_ntoa(sin->sin_addr));
    close(socket_fd);
    return 0;
}

int SystemRequest(OnvifSystem* sys) {
	switch (sys->type)
	{
	case ONVIF_SYSTEM_DEVICE_INFO:
		snprintf(sys->out.device_info.soft_version, ONVIF_SYSTEM_DEVICE_INFO_MAX, "v1.0.0");
		snprintf(sys->out.device_info.hard_version, ONVIF_SYSTEM_DEVICE_INFO_MAX, "v1.0");
		snprintf(sys->out.device_info.serial_number, ONVIF_SYSTEM_DEVICE_INFO_MAX, "0123456789012345");
		snprintf(sys->out.device_info.device_type, ONVIF_SYSTEM_DEVICE_INFO_MAX, "onvif_test");
		break;
	case ONVIF_SYSTEM_GET_TIME:
		sys->out.time = time(NULL);
		break;
	case ONVIF_SYSTEM_SET_TIME:{
		struct timeval tv;
		memset(&tv, 0, sizeof(struct timeval));
		tv.tv_sec = sys->in.time;
		settimeofday(&tv, NULL);
		break;
	}
	case ONVIF_SYSTEM_RTSP_INFO:
		if (sys->cam == 0) {
			snprintf(sys->out.rtsp_url.main_stream, ONVIF_SYSTEM_URL_MAX, "rtsp://192.168.110.223:554/streaming/001?auth=admin:123456");
		} else {
			snprintf(sys->out.rtsp_url.main_stream, ONVIF_SYSTEM_URL_MAX, "rtsp://192.168.110.223:554/streaming/101?auth=admin:123456");
		}
		break;
	default:
		break;
	}
	return 0;
}

int GetConfig(int type, void* st, int size) {
	if (type == ONVIF_CONFIG_NETWORK_TCP_IP) {
		OnvifConfigNetworkTcpIp* network_tcp_ip = (OnvifConfigNetworkTcpIp*)st;
		network_tcp_ip->ipv4_enable = 1;
		GetLocalAddr("eth0", network_tcp_ip->ipv4_addr, sizeof(network_tcp_ip->ipv4_addr));
		strcpy(network_tcp_ip->ipv4_mask, "225.225.225.0");
		strcpy(network_tcp_ip->ipv4_gateway, "192.168.110.1");
		strcpy(network_tcp_ip->dns, "192.168.110.1");
		strcpy(network_tcp_ip->mac, "AE:A1:EB:19:79:3C");
		strcpy(network_tcp_ip->alt_dns, "8.8.8.8");
	} else if (type == ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_INFO) {
		OnvifConfigCameraVideoEncodeInfos* video_encode_info = (OnvifConfigCameraVideoEncodeInfos*)st;
		if (video_encode_info->deal_cam != -1 && video_encode_info->deal_stream != -1) {
			video_encode_info->cam_num = 1;
			video_encode_info->stream_num[0] = 1;
			memcpy(&video_encode_info->encode_info[0][0], &kMng.video_encode_infos.encode_info[video_encode_info->deal_cam][video_encode_info->deal_stream], sizeof(OnvifConfigCameraVideoEncodeInfo));
		} else {
			video_encode_info->cam_num = kMng.video_encode_infos.cam_num;
			memcpy(video_encode_info->stream_num, kMng.video_encode_infos.stream_num, sizeof(kMng.video_encode_infos.stream_num));
			memcpy(video_encode_info->encode_info, kMng.video_encode_infos.encode_info, sizeof(video_encode_info->encode_info));
		}
	} else if (type == ONVIF_CONFIG_CAMERA_AUDEO_ENCODE_INFO) {
		OnvifConfigCameraAudioEncodeInfo* audio_encode_info = (OnvifConfigCameraAudioEncodeInfo*)st;
		memcpy(audio_encode_info, &kMng.audio_encode_info, sizeof(OnvifConfigCameraAudioEncodeInfo));

	} else if (type == ONVIF_CONFIG_CAMERA_IMAGE_INFO) {
		OnvifConfigCameraImageInfos* image_infos = (OnvifConfigCameraImageInfos*)st;
		if (image_infos->deal_cam != -1) {
			image_infos->num = 1;
			memcpy(&image_infos->image_info[0], &kMng.image_infos.image_info[image_infos->deal_cam], sizeof(OnvifConfigCameraImageInfo));
		} else {
			image_infos->num = kMng.image_infos.num;
			memcpy(image_infos->image_info, kMng.image_infos.image_info, sizeof(kMng.image_infos.image_info));
		}	
	} else if (type == ONVIF_CONFIG_CAMERA_OSD_INFO) {
		OnvifConfigCameraOsdInfos* osd_infos = (OnvifConfigCameraOsdInfos*)st;
		if (osd_infos->deal_num != -1) {
			osd_infos->num = 1;
			memcpy(&osd_infos->osd_info[0], &kMng.osd_infos.osd_info[osd_infos->deal_num], sizeof(OnvifConfigCameraOsdInfo));
		} else {
			osd_infos->num = kMng.osd_infos.num;
			memcpy(osd_infos->osd_info, kMng.osd_infos.osd_info, sizeof(kMng.osd_infos.osd_info));
		}	
	} else if (type == ONVIF_CONFIG_PTZ_PRESET) {
		OnvifConfigPtzPresets* presets = (OnvifConfigPtzPresets*)st;
		memcpy(presets, &kMng.ptz_presets, sizeof(OnvifConfigPtzPresets));

	}
    return 0;
}

int SetConfig(int type, void* st, int size) {
	if (type == ONVIF_CONFIG_NETWORK_TCP_IP) {
		OnvifConfigNetworkTcpIp* network_tcp_ip = (OnvifConfigNetworkTcpIp*)st;
		char cmd[256] = {0};
		snprintf(cmd, sizeof(cmd), "ifconfig eth0 %s netmask %s", network_tcp_ip->ipv4_addr, network_tcp_ip->ipv4_mask);
		system(cmd);
	} else if (type == ONVIF_CONFIG_PTZ_PRESET) {
		OnvifConfigPtzPresets* presets = (OnvifConfigPtzPresets*)st;
		if (presets->type == ONVIF_CONFIG_PTZ_PRESET_ADD) {
			if (kMng.ptz_presets.preset_num >= ONVIF_CONFIG_PRESET_MAX_NUM) {
				return -1;
			}

			strcpy(kMng.ptz_presets.preset[kMng.ptz_presets.preset_num].name, presets->preset[0].name);
			kMng.ptz_presets.preset_num ++;
		} else if(presets->type == ONVIF_CONFIG_PTZ_PRESET_DEL) {
			int i = 0;
			for (; i < kMng.ptz_presets.preset_num; i++) {
				if(strcmp(presets->preset[0].name, kMng.ptz_presets.preset[i].name) == 0) {
					break;
				}
			}

			if (i == ONVIF_CONFIG_PRESET_MAX_NUM) {
				return -1;
			} else if (i == ONVIF_CONFIG_PRESET_MAX_NUM - 1) {
				kMng.ptz_presets.preset_num --;
				memset(&kMng.ptz_presets.preset[255], 0, sizeof(OnvifConfigPtzPreset));
			} else {
				for(; i < kMng.ptz_presets.preset_num; i++) {
					memcpy(&kMng.ptz_presets.preset[i+1], &kMng.ptz_presets.preset[i], sizeof(OnvifConfigPtzPreset));
				}
			}
		}

	} else if (type == ONVIF_CONFIG_CAMERA_VIDEO_ENCODE_INFO) {
		OnvifConfigCameraVideoEncodeInfos* encode_infos = (OnvifConfigCameraVideoEncodeInfos*)st;
		memcpy(&kMng.video_encode_infos.encode_info[encode_infos->deal_cam][encode_infos->deal_stream], &encode_infos->encode_info[0][0], sizeof(OnvifConfigCameraVideoEncodeInfo));
	} else if (type == ONVIF_CONFIG_CAMERA_OSD_INFO) {
		OnvifConfigCameraOsdInfos* osd_infos = (OnvifConfigCameraOsdInfos*)st;
		if (osd_infos->deal_num != -1) {
			memcpy(&kMng.osd_infos.osd_info[osd_infos->deal_num], &osd_infos->osd_info[0], sizeof(OnvifConfigCameraOsdInfo));		
		} else {
			memcpy(&kMng.osd_infos, osd_infos, sizeof(OnvifConfigCameraOsdInfos));
		}
	} else if (type == ONVIF_CONFIG_CAMERA_IMAGE_INFO) {
		OnvifConfigCameraImageInfos* image_infos = (OnvifConfigCameraImageInfos*)st;
		if (image_infos->deal_cam != -1) {
			memcpy(&kMng.image_infos.image_info[image_infos->deal_cam], &image_infos->image_info[0], sizeof(OnvifConfigCameraImageInfo));		
		} else {
			memcpy(&kMng.image_infos, image_infos, sizeof(OnvifConfigCameraOsdInfos));
		}
	}
	
    return 0;
}

int ContorlRequest(int type, int chn, void* st, int size) {
	if (type == ONVIF_CONTORL_PTZ) {
		OnvifContorlPtzCtrl* ptz_ctrl = (OnvifContorlPtzCtrl*)st;
		printf("ptz type:%d\n", ptz_ctrl->mode);
	}
    return 0;
}

int main(int argc, char** argv) {

	OnvifInitialInfo info = {0};
    OnvifInit(&info);

    OnvifOperationRegister(ONVIF_OPERATION_SYSTEM_REQUEST, SystemRequest);
    OnvifOperationRegister(ONVIF_OPERATION_GET_CONFIG, GetConfig);
    OnvifOperationRegister(ONVIF_OPERATION_SET_CONFIG, SetConfig);
    OnvifOperationRegister(ONVIF_OPERATION_CONTORL_REQUEST, ContorlRequest);

    while (1) {
		OnvifEventInfo info;
		info.type = ONVIF_MONITIORING_PROCESSOR_USAGE;
		info.state = ONVIF_EVENT_INITIALIZED;
		OnvifEventMonitioringProcessorUsage usage = {.usage = 50};
		info.val = &usage;
		OnvifEventUplaod(&info);
        sleep(10);
    }

    return 0;
}