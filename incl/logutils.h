//defines

//Vars
typedef enum 
{ 
    ERROR=0,
    CRIT=1,
    WARN=2,
    INFO=3,
    DEBUG=4
} LOG_LEVEL;

//Methods
extern void initLogging(LOG_LEVEL logLevel, char* cpLogFile);
extern void logIt(LOG_LEVEL logLevel, char* cpMessage);
extern void destroyLogging();
