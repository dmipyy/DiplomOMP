#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdbool.h>

#define maxMessageLength 1024
#define WR_DATA _IOW('a','a',char*)
#define RD_DATA _IOR('a','b',char*)

struct IoctlBuffer //структура данных для функции ioctl
{
	char message[maxMessageLength];
};

