/**
 * Logging funtionality for service
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <logutils.h>

static FILE* fpLogFile = NULL;
static LogLevel CURRENT_LOG_LEVEL = DEBUG;

typedef struct node
{
    char time[26];
    char message[4048];
    struct node* next;
} logNode;
static logNode* logList = NULL;

/**
 * @brief store a log message
 * @param cpMessage ... log message
 */
static void pushLog(char* cpTime, char* cpMessage)
{
    logNode* newNode = (logNode*) malloc(sizeof(logNode));
    memset(newNode->message, 0, sizeof(newNode->message));
    memset(newNode->time, 0, sizeof(newNode->time));
    snprintf(newNode->time, sizeof(newNode->time)-1, cpTime);
    snprintf(newNode->message, sizeof(newNode->message)-1, cpMessage);
    newNode->next = logList;
    logList = newNode;
}

/**
 * @brief print log list
 * @author chifac08
 */
static void printList()
{
    while(logList != NULL)
    {
        printf(logList->time);
        printf(logList->message);
        logList=logList->next;
    }
}

/**
 * @brief 
 * @param cpLogFile
 */
void initLogging(LogLevel logLevel, char* cpLogFile)
{
    char szLogging[1024] = {0};
   
    if(fpLogFile == NULL)
        fpLogFile = fopen(cpLogFile, "a+");
    
    CURRENT_LOG_LEVEL = logLevel;
}

/**
 * @brief log message to log file
 * @param cpMessage
 */
void logIt(LogLevel logLevel, char* cpMessage)
{
    char szLogMessage[4048] = {0};
    char szTime[26] = {0};
    time_t now;
    struct tm* timeinfo;
    
  //  if(logLevel <= CURRENT_LOG_LEVEL)
    if(1)
    {
        time(&now);
        timeinfo = localtime(&now);
        //TODO parse loglevel
        strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", timeinfo);
        pushLog(szTime, cpMessage);
        printList();
    }
}
