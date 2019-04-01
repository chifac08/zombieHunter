typedef enum 
{ 
    ERROR=0,
    CRIT=1,
    WARN=2,
    INFO=3,
    DEBUG=4
} LogLevel;

static void pushLog(char* cpTime, char* cpMessage);

extern void logIt(LogLevel logLevel, char* cpMessage);

