#include "basement.h"

//defines

//Vars

//Methods
extern void initLogging(CONFIG config);
extern void logIt(LOG_LEVEL logLevel, char* cpMessage);
extern char* getCurrentLogLevel();
extern void destroyLogging();
extern LOG_LEVEL parseLogLevel(char* cpLogLevel);
