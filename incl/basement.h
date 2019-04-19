#include <stdio.h>
#include "LogTypes.h"

#define DEFAULT_CONFIG_FILE "/opt/etc/zombieHunter/hunter.cfg"
#define DEFAULT_LOG_FILE "/tmp/test.log"
#define DEFAULT_LOG_CACHE 5

typedef enum {
	false=0,
	true=1
} bool;

typedef struct
{
	LOG_CONFIG logConfig;
	unsigned int listenPort;
} CONFIG;
