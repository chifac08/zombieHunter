/**
 * Logging funtionality for service
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "logutils.h"

//defines
#define DATE_SIZE 26
#define LOG_MESSAGE_SIZE 4048

//vars
typedef struct node
{
    char time[DATE_SIZE];
    char message[LOG_MESSAGE_SIZE];
    struct node* next;
} logNode;

static LOG_LEVEL CURRENT_LOG_LEVEL = ERROR;
static int iListSize = 0;
static logNode* logList = NULL;
static logNode* lastNode = NULL;
static CONFIG logConfig;

static const char* logLevelString[] = {
		"ERROR",
		"CRIT",
		"WARN",
		"INFO",
		"DEBUG"
};

/**
 * @brief return log level as string
 * @author chifac08
 */
static char* getLogLevel(LOG_LEVEL logLevel)
{
	return logLevelString[logLevel];
}

/*
 * @brief create a new logger node
 * @param cpTime ... log time
 * @param cpMessage ... log message
 * @author chifac08
 */
static logNode* createNode(char* cpTime, char* cpMessage)
{
    logNode* newNode = (logNode*) malloc(sizeof(logNode));
    memset(newNode->message, 0, sizeof(newNode->message));
    memset(newNode->time, 0, sizeof(newNode->time));
    snprintf(newNode->time, sizeof(newNode->time)-1, cpTime);
    snprintf(newNode->message, sizeof(newNode->message)-1, cpMessage);

    return newNode;
}

/**
 * @brief store a log message
 * @param cpTime ... log time
 * @param cpMessage ... log message
 */
static void pushLog(char* cpTime, char* cpMessage)
{
	logNode* newNode = createNode(cpTime, cpMessage);
    newNode->next = logList;
    logList = newNode;

    //increment list size counter
    iListSize++;
}

/**
 * @brief append log node to log list
 * @param cpTime ... log time
 * @param cpMessage ... log message
 * @author chifac08
 */
static void appendLog(char* cpTime, char* cpMessage)
{
	logNode* newNode = createNode(cpTime, cpMessage);

	if(logList == NULL)
	{
		logList = newNode;
		lastNode = logList;
	}

	//link new node to the end
	lastNode->next=newNode;

	//link new node to null
	newNode->next=NULL;

	//point to last node
	lastNode=newNode;

	//increment list size counter
	iListSize++;
}

/**
 * @brief remove specific log from linked list
 * @param index ... index for log node
 * @author flichtenegger
 */
static void removeLog(int iIndex)
{
	//store head node
	logNode* temp = logList;

    // If head needs to be removed
    if (iIndex == 0)
    {
        logList = temp->next;   // Change head
        free(temp);               // free old head
        return;
    }

    for(; iIndex > 1; iIndex--)
    	temp = temp->next;

    logNode* next = temp->next;
    temp->next = next->next;
    free(next);

    //decrement list size counter
    iListSize--;
}

/**
 * @brief print log list
 * @author chifac08
 */
static void printList()
{
    while(logList != NULL)
    {
        printf("%s %s\n", logList->time, logList->message);
        logList=logList->next;
    }
}

/**
 * @brief flush the whole linked list
 * @author chifac08
 */
static void flushList()
{
	logNode* next = NULL;

	while(logList != NULL)
	{
		next = logList->next;
		free(logList);
		logList=next;
	}
	iListSize=0;
	logList = NULL;
}

/**
 * @brief writes to log file and stdout (when debugging mode is active)
 * @author chifac08
 */
static void writeLog()
{
	FILE* logFile = NULL;
	int iFullMsgLength = DATE_SIZE+1+LOG_MESSAGE_SIZE+1;
	char* cpFullMessage = NULL;
	int iIndex = 0;

	logFile = fopen(logConfig.logFile, "a+");

	if(!logFile)
	{
		printf("Could not open logfile: %s", logConfig.logFile);
		return;
	}

	if(!cpFullMessage)
		cpFullMessage = (char*)malloc(iFullMsgLength);

	while(logList != NULL)
	{
		memset(cpFullMessage, 0, iFullMsgLength);
		snprintf(cpFullMessage, iFullMsgLength-1, "%s %s", logList->time, logList->message);
		printf("%s\n", cpFullMessage);
		fprintf(logFile, cpFullMessage);
		logList = logList->next;
	}

	//flush current list
	flushList();

	if(cpFullMessage)
		free(cpFullMessage);

	fclose(logFile);
}

/**
 * @brief initialize logging
 * @param logLevel
 * @param cpLogFile ... absolute path for log file
 * @author chifac08
 */
void initLogging(CONFIG config)
{
	memset(&logConfig, 0, sizeof(CONFIG));

	memcpy(&logConfig, &config, sizeof(CONFIG));

    CURRENT_LOG_LEVEL = config.logLevel;
}

/**
 * @brief log message to log file
 * @param cpMessage
 * @author chifac08
 */
void logIt(LOG_LEVEL logLevel, char* cpMessage)
{
    char szLogMessage[LOG_MESSAGE_SIZE] = {0};
    char szTime[DATE_SIZE] = {0};
    time_t now;
    struct tm* timeinfo;
    
    if(logLevel <= CURRENT_LOG_LEVEL)
    {
    	if(iListSize >= logConfig.logBuffer)
    		writeLog();

        time(&now);
        timeinfo = localtime(&now);
        //TODO parse loglevel
        strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", timeinfo);
        snprintf(szLogMessage, sizeof(szLogMessage)-1, "[%s]: %s", getLogLevel(logLevel), cpMessage);
        appendLog(szTime, szLogMessage);
    }
}

/**
 * @brief return current set log level
 * @author chifac08
 */
char* getCurrentLogLevel()
{
	return getLogLevel(CURRENT_LOG_LEVEL);
}

/*
 * @brief Free memory
 * @author chifac08
 */
void destroyLogging()
{
	flushList();
}

/**
 * @brief convert string to log level
 * @author chifac08
 */
LOG_LEVEL parseLogLevel(char* cpLogLevel)
{
	LOG_LEVEL logLevel = CURRENT_LOG_LEVEL;

	for(int i = 0; i < sizeof(logLevelString);i++)
	{
		if(strcmp(cpLogLevel, logLevelString[i]) == 0)
		{
			logLevel = i;
			break;
		}
	}

	return logLevel;
}
