#ifndef __UTIL__
#define __UTIL__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "util.h"

int get_wlan_address(char *address)
{
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ - 1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    /* display result */
    strncpy(address, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), 128);
    printf("%s\n", address);

    return 0;
}

int connect_wifi_by_str(char *str)
{
    char cmd[256] = {0};
    char *revbuf[8] = {0}; //存放分割后的子字符串
    int num = 0;
    split(str, ",", revbuf, &num);
    sprintf(cmd, "wpa_cli -i wlan0 set_network 0 ssid \\\"%s\\\"", revbuf[0]);
    printf("cmd:%s\n", cmd);
    system(cmd);
    sprintf(cmd, "wpa_cli -i wlan0 set_network 0 psk \\\"%s\\\"", revbuf[1]);
    printf("cmd:%s\n", cmd);
    system(cmd);
    sprintf(cmd, "wpa_cli -i wlan0 save_config");
    printf("cmd:%s\n", cmd);
    system(cmd);
    sprintf(cmd, "wpa_cli -i wlan0 enable_network 0");
    printf("cmd:%s\n", cmd);
    system(cmd);
    sprintf(cmd, "wpa_cli -i wlan0 reconfigure");
    printf("cmd:%s\n", cmd);
    system(cmd);
    return 0;
}

void split(char *src, const char *separator, char **dest, int *num)
{
    /*
        src 源字符串的首地址(buf的地址) 
        separator 指定的分割字符
        dest 接收子字符串的数组
        num 分割后子字符串的个数
    */
    char *pNext;
    int count = 0;

    if (src == NULL || strlen(src) == 0) //如果传入的地址为空或长度为0，直接终止
        return;

    if (separator == NULL || strlen(separator) == 0) //如未指定分割的字符串，直接终止
        return;

    pNext = (char *)strtok(src, separator); //必须使用(char *)进行强制类型转换(虽然不写有的编译器中不会出现指针错误)
    while (pNext != NULL)
    {
        *dest++ = pNext;
        ++count;
        pNext = (char *)strtok(NULL, separator); //必须使用(char *)进行强制类型转换
    }
    *num = count;
}

#endif