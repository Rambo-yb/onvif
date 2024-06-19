#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "onvif.h"

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

int main(int argc, char** argv) {
    char addr[16];
    GetLocalAddr("eth0", addr, sizeof(addr));

    OnvifDevInfo dev_info;
    snprintf(dev_info.menu_facturer, sizeof(dev_info.menu_facturer), "ANHG");
    snprintf(dev_info.module, sizeof(dev_info.module), "QJHW");
    snprintf(dev_info.serial_num, sizeof(dev_info.serial_num), "1234567891234560");
    snprintf(dev_info.firmware_ver, sizeof(dev_info.firmware_ver), "v1.0.9");
    snprintf(dev_info.hardware_ver, sizeof(dev_info.hardware_ver), "1.0");
    OnvifInit(addr, dev_info);

    while (1) {
        sleep(1);
    }

    return 0;
}