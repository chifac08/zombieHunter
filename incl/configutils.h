#include <logutils.h>

//defines
#define DEFAULT_LISTEN_PORT 5557
#define DEFAULT_LOG_ROTATE 10
#define LOG_FILE_DELIMITER "="

//vars
typedef struct
{
	LOG_LEVEL logLevel;
	unsigned int listenPort;
	unsigned int logRotate;
	char logFile[4048];
} CONFIG;

//Methods
extern CONFIG* parseConfig(char* cpConfigFile);
