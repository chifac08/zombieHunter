#include <stdio.h>
#include "LogTypes.h"

#define DEFAULT_CONFIG_FILE "/opt/etc/zombieHunter/hunter.cfg"
#define DEFAULT_LOG_FILE "/var/log/zombieHunter/zombieHunter.log"
#define DEFAULT_LOG_CACHE 5

typedef enum {
	false=0,
	true=1
} bool;

typedef struct
{
	LOG_CONFIG logConfig;
	unsigned int listenPort;
	char host[16];
	unsigned int port;
	unsigned int sendTimeout;
} CONFIG;
