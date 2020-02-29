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

#endif