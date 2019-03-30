typedef enum 
{ 
    ERROR=1, 
    CRIT=2, 
    WARN=3, 
    INFO=4, 
    DEBUG=5 
} LogLevel;

static void pushLog(char* cpTime, char* cpMessage);

extern void logIt(LogLevel logLevel, char* cpMessage);

