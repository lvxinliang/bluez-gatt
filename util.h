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

#endif