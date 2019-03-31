/**
 * Logging funtionality for service
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <logutils.h>

#define DATE_SIZE 26
#define LOG_MESSAGE_SIZE 4048

static char* cpLogFilePath = NULL;
static LogLevel CURRENT_LOG_LEVEL = DEBUG;

typedef struct node
{
    char time[DATE_SIZE];
    char message[LOG_MESSAGE_SIZE];
    struct node* next;
} logNode;
static logNode* logList = NULL;
static logNode* lastNode = NULL;

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

	lastNode->next=newNode;
	lastNode=newNode;
}

/**
 * @brief remove specific log from linked list
 * @param index ... index for log node
 * @author flichtenegger
 */
static void removeLog(int index)
{
	//store head node
	logNode* temp = logList;

    // If head needs to be removed
    if (index == 0)
    {
        logList = temp->next;   // Change head
        free(temp);               // free old head
        return;
    }

    // Find previous node of the node to be deleted
    for (int i=0; temp!=NULL && i<index-1; i++)
         temp = temp->next;

    // If position is more than number of ndoes
    if (temp == NULL || temp->next == NULL)
         return;

    // Node temp->next is the node to be deleted
    // Store pointer to the next of node to be deleted
    logNode* next = temp->next->next;

    // Unlink the node from linked list
    free(temp->next);  // Free memory

    temp->next = next;  // Unlink the deleted node from list
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
 * @brief 
 * @param logLevel
 * @param cpLogFile ... absolute path for log file
 * @author chifac08
 */
void initLogging(LogLevel logLevel, char* cpLogFile)
{
    int iLogFileLength = strlen(cpLogFile);

    //copy log file path
    if(!cpLogFilePath)
    	cpLogFilePath = (char*) malloc(iLogFileLength+1);

    memset(cpLogFilePath, 0, sizeof(cpLogFilePath));
    strcpy(cpLogFilePath, cpLogFile);
    
    //init set log level
    CURRENT_LOG_LEVEL = logLevel;
}

/**
 *
 */
void writeToLogFile()
{
	FILE* logFile = NULL;
	int iLogLine = 0;
	int iFullMsgLength = DATE_SIZE+2+LOG_MESSAGE_SIZE+1;
	char* cpFullMessage = NULL;

	logFile = fopen(cpLogFilePath, "a+");

	if(!logFile)
		printf("ERROR");

	if(!cpFullMessage)
		cpFullMessage = (char*)malloc(iFullMsgLength);

	while(logList != NULL)
	{
		memset(cpFullMessage, 0, sizeof(cpFullMessage));
		snprintf(cpFullMessage, sizeof(cpFullMessage)-1, "%s: %s", logList->time, logList->message);
		fprintf(logFile, cpFullMessage, iLogLine);
		iLogLine++;
		logList = logList->next;
	}

	if(cpFullMessage)
		free(cpFullMessage);
}

/**
 * @brief log message to log file
 * @param cpMessage
 * @author chifac08
 */
void logIt(LogLevel logLevel, char* cpMessage)
{
    char szLogMessage[LOG_MESSAGE_SIZE] = {0};
    char szTime[DATE_SIZE] = {0};
    time_t now;
    struct tm* timeinfo;
    
  //  if(logLevel <= CURRENT_LOG_LEVEL)
    if(1)
    {
        time(&now);
        timeinfo = localtime(&now);
        //TODO parse loglevel
        strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", timeinfo);
       // pushLog(szTime, cpMessage);
       //printList();
        appendLog(szTime, cpMessage);
    }
}

void test()
{
	removeLog(1);
	printList();
}

/*
 * @brief Free memory
 * @author chifac08
 */
void destroyLogging()
{
	if(cpLogFilePath)
		free(cpLogFilePath);

	while(logList != NULL)
	{
		free(logList);
		logList=logList->next;
	}
}
