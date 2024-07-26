#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "onvif.h"
#include "log.h"

typedef struct {
    OnvifVideoEncoder video_encoder;  
    OnvifAudioEncoder audio_encoder;  
    OnvifPresets presets;  
}Mng;
static Mng kMng;

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

int Cb(OnvifOperType type, void* name, void* arg) {
    if (type == ONVIF_OPER_GET_CONFIG) {
        if (strcmp(name, "video_encoder") == 0) {
            memcpy(arg, &kMng.video_encoder, sizeof(OnvifVideoEncoder));
        } else if (strcmp(name, "audio_encoder") == 0) {
            memcpy(arg, &kMng.audio_encoder, sizeof(OnvifAudioEncoder));
        } else if (strcmp(name, "presets") == 0) {
            memcpy(arg, &kMng.presets, sizeof(OnvifPresets));
        }
    } else if (type == ONVIF_OPER_PTZ_CTRL) {
        LOG_INFO("ptz ctrl :%d - %s", *(OnvifPtzCtrlType*)name, arg);
    } else if (type == ONVIF_OPER_SET_CONFIG) {
        if (strcmp(name, "presets") == 0) {
            OnvifPresets* presets = arg;
            if (presets->type == ONVIF_PRESET_CTRL_ADD) {
                kMng.presets.preset_num += 1;
                snprintf(kMng.presets.preset[kMng.presets.preset_num - 1], 64, "%s", presets->preset[0]);
            } else if (presets->type == ONVIF_PRESET_CTRL_DEL) {
                for (int i = 0; i < kMng.presets.preset_num; i++) {
                    if (strcmp(presets->preset[0], kMng.presets.preset[i]) == 0) {
                        memset(kMng.presets.preset[i], 0, sizeof(kMng.presets.preset[i]));
                        for (int j = i; j < kMng.presets.preset_num - 1; j++) {
                            memcpy(kMng.presets.preset[j], kMng.presets.preset[j+1], 64);
                        }
                        kMng.presets.preset_num -= 1;
                        break;
                    }
                }
            } else if (presets->type == ONVIF_PRESET_CTRL_SET_HOME) {

            }
        } else {

        }
    }

    return 0;
}

void MainInit() {
    kMng.video_encoder.encoding = ONVIF_VIDEO_ENCODING_H264;
    kMng.video_encoder.gov_length = 25;
    kMng.video_encoder.profile = ONVIF_H264_HIGH;
    kMng.video_encoder.resolution_width = 640;
    kMng.video_encoder.resolution_height = 512;

    kMng.audio_encoder.channels = 1;
    kMng.audio_encoder.encoding = ONVIF_AUDIO_ENCODING_AAC;
    kMng.audio_encoder.bitrate = 2048;
    kMng.audio_encoder.sample_rate = 16000;

    kMng.presets.preset_num = 2;
    for(int i = 0; i < kMng.presets.preset_num; i++) {
        snprintf(kMng.presets.preset[i], 64, "preset_%02d", i);
    }
}

int main(int argc, char** argv) {
    char addr[16];
    GetLocalAddr("eth0", addr, sizeof(addr));

    MainInit();

    OnvifDevInfo dev_info;
    snprintf(dev_info.menu_facturer, sizeof(dev_info.menu_facturer), "ANHG");
    snprintf(dev_info.module, sizeof(dev_info.module), "QJHW");
    snprintf(dev_info.serial_num, sizeof(dev_info.serial_num), "1234567891234560");
    snprintf(dev_info.firmware_ver, sizeof(dev_info.firmware_ver), "v1.0.9");
    snprintf(dev_info.hardware_ver, sizeof(dev_info.hardware_ver), "1.0");
    snprintf(dev_info.rtsp_url, sizeof(dev_info.rtsp_url), "rtsp://192.168.110.223:554/mainstream");
    snprintf(dev_info.snap_url, sizeof(dev_info.snap_url), "http://192.168.110.223:8080/dev_api/get_snap");
    OnvifInit(addr, dev_info, Cb);

    int flag = 0;
    int cnt = 0;
    while (1) {
        if ((++cnt)%5 == 0) {
            OnvifEventInfo info;
            snprintf(info.event_type, sizeof(info.event_type), "human_detect");
            info.state = flag ? 0 : 1;
            info.time = time(NULL);
            OnvifEventUplaod(&info);
        }
        flag = !flag;
        sleep(1);
    }

    return 0;
}