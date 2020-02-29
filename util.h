#ifndef __RASPBERRY_UTIL__
#define __RASPBERRY_UTIL__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */

int get_wlan_address(char *address);

int connect_wifi_by_str(char *str);

void split(char *src, const char *separator, char **dest, int *num);
#endif