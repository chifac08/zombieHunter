#include <stdio.h>

#define DEFAULT_CONFIG_FILE "/opt/etc/zombieHunter/hunter.cfg"
#define DEFAULT_LOG_FILE "/tmp/test.log"
#define DEFAULT_LOG_CACHE 5

typedef enum {
	false=0,
	true=1
} bool;

typedef enum
{
    ERROR=0,
    CRIT=1,
    WARN=2,
    INFO=3,
    DEBUG=4
} LOG_LEVEL;

typedef struct
{
	LOG_LEVEL logLevel;
	unsigned int listenPort;
	unsigned int logRotate;
	char logFile[4048];
	unsigned int logBuffer;
} CONFIG;
