typedef enum 
{ 
    ERROR=1, 
    CRIT=2, 
    WARN=3, 
    INFO=4, 
    DEBUG=5 
} LogLevel;

typedef struct node
{
    char time[26];
    char message[4048];
    struct node* next;
} logNode;

extern void log(LogLevel logLevel, char* cpMessage);