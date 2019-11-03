/*
 * basement.c
 *
 * Holds base functions.
 *
 *  Created on: Apr 7, 2019
 *      Author: chifac08
 */
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "basement.h"
#include "SCLogger.h"
#include "tcpcomm.h"

Queue* createQueue(int iMaxElements)
{
	Queue* queue = NULL;
	queue = (Queue*)malloc(sizeof(Queue));
	memset(queue, 0, sizeof(Queue));
	queue->capacity = iMaxElements;
	queue->size = 0;
	queue->front = 0;
	queue->rear = -1;
	queue->elements = (char**)malloc(sizeof(char*)*iMaxElements);
	memset(queue->elements, 0, sizeof(sizeof(char)*iMaxElements));

	return queue;
}

void enqueue(Queue* queue, char* cpElement)
{
	if(queue->size == queue->capacity)
	{
		//TODO error handling
		return;
	}

	queue->size++;
	queue->rear = queue->rear+1;

	if(queue->rear == queue->capacity)
	{
		queue->rear = 0;
	}

	queue->elements[queue->rear] = (char*)malloc(sizeof(cpElement)+1);
	memset(queue->elements[queue->rear], 0, sizeof(cpElement)+1);
	strcpy(queue->elements[queue->rear], cpElement);
}

char* front(Queue* queue)
{
	if(queue->size < 1)
		return NULL;

	return queue->elements[queue->front];
}

char* dequeue(Queue* queue)
{
	char* cpReturn = NULL;

	if(queue->size < 1)
		return NULL;

	cpReturn = queue->elements[queue->front];
	queue->front = (queue->front + 1)%queue->capacity;
	queue->size -= 1;

	return cpReturn;
}

void clearQeue(Queue* queue)
{
	if(queue)
	{
		for(int i = 0; i < queue->size;i++)
		{
			if(queue->elements[i])
				free(queue->elements[i]);
		}

		free(queue);
	}
}

void test(void* arg)
{
	FILE_WATCHER_ARG* fWatcherArg = (FILE_WATCHER_ARG*)arg;
	char szLogMessage[1024] = {0};

	while(1)
	{
		pthread_mutex_lock(&fWatcherArg->mutex);
		char* cpReturn = dequeue(fWatcherArg->zombie_queue);

		if(cpReturn)
		{
			snprintf(szLogMessage, sizeof(szLogMessage), "%s", cpReturn);
			logIt(INFO, szLogMessage);
			free(cpReturn);
		}

		pthread_mutex_unlock(&fWatcherArg->mutex);
		sleep(30);
	}
}

/**
 * @brief loads the whole content of a file into memory
 * @param cpFileName ... absolute path of the file
 * @param szResult ... result pointer
 * @author chifac08
 * @return iSize ... actual size of the file
 */
int loadFileInMemory(char* cpFileName, char** szResult)
{
	int iSize = 0;
	FILE* cpProcFile = NULL;
	char szLogMessage[1024] = {0};

	cpProcFile = fopen(cpFileName, "r");

	if(!cpProcFile)
	{
		*szResult = NULL;
		//write as INFO cause it is not that bad when we can't read the file
		//maybe we will catch it with the next try
		formatLog(szLogMessage, sizeof(szLogMessage), "Cannot open file %s. Error: [%d] - %s", cpFileName, errno, strerror(errno));
		logIt(INFO, szLogMessage);
		return -1;
	}

	fseek(cpProcFile, 0, SEEK_END);
	iSize = ftell(cpProcFile);
	fseek(cpProcFile, 0, SEEK_SET);

	*szResult = (char*)malloc(iSize+1);
	if(iSize != fread(*szResult, sizeof(char), iSize, cpProcFile))
	{
		free(*szResult);
		formatLog(szLogMessage, sizeof(szLogMessage), "Cannot read file %s. Error: [%d] - %s", cpFileName, errno, strerror(errno));
		logIt(INFO, szLogMessage);
		return -2;
	}

	fclose(cpProcFile);
	(*szResult)[iSize] = 0;

	return iSize;
}

/**
 * @brief creates a given directory with the given permissions
 * @param cpDir ... absolute path
 * @param mode ... octal permissions
 * @author chifac08
 */
int createDir(char* cpDir, mode_t mode)
{
	struct stat st = {0};
	int iReturn = 0;
	char szLogMessage[1024] = {0};

	if(stat(cpDir, &st) < 0)
	{
		iReturn = mkdir(cpDir, mode);

		if(iReturn < 0)
		{
			formatLog(szLogMessage, sizeof(szLogMessage), "Could not create directory %s. Error: [%d] - %s", cpDir, errno, strerror(errno));
			logIt(ERROR, szLogMessage);
			return -1;
		}
	}

	return iReturn;
}

/**
 * @brief copy a file from source to destination
 * @param cpSourcePath ... source file
 * @param cpDestPath ... destination file
 * @author chifac08
 * @return 0 ... ok
 *        -1 ... cannot open source file
 *        -2 ... cannot open destination file
 */
int copyFile(char* cpSourcePath, char* cpDestPath)
{
	FILE* cpSourceFile = NULL;
	FILE* cpDestFile = NULL;
	char szLogMessage[1024] = {0};
	char ch;

	cpSourceFile = fopen(cpSourcePath, "r");

	if(!cpSourceFile)
	{
		formatLog(szLogMessage, sizeof(szLogMessage), "Cannot open source file %s", cpSourcePath);
		logIt(ERROR, szLogMessage);
		return -1;
	}

	cpDestFile = fopen(cpDestPath, "w");

	if(!cpDestFile)
	{
		formatLog(szLogMessage, sizeof(szLogMessage), "Cannot open destination file %s", cpSourcePath);
		logIt(ERROR, szLogMessage);
		fclose(cpSourceFile);
		return -2;
	}

	while((ch = fgetc(cpSourceFile)) != EOF)
	{
		fputc(ch, cpDestFile);
	}

	fclose(cpSourceFile);
	fclose(cpDestFile);

	return 0;
}

/**
 * @brief checks if a stop flag file exists
 * @param cpFileName ... absolute path of file
 * @author chifac08
 * @return true ... file exists
 *        false ... file does not exist
 */
bool checkStopFlag(const char* cpFileName)
{
	struct stat buff;

	if(stat(cpFileName, &buff) == 0)
		return true;

	return false;
}

/*
 * @brief removes a file
 * @param cpFileName .. absolute path of file
 * @author chifac08
 * @return 0 ... OK
 *        -1 ... could not delete file
 */
int removeFile(const char* cpFileName)
{
	int iStatus = 0;
	char szLogMessage[1024] = {0};

	iStatus = remove(cpFileName);

	if(iStatus != 0)
	{
		formatLog(szLogMessage, sizeof(szLogMessage), "Could not delete File %s. Return Code: %d", cpFileName, iStatus);
		logIt(ERROR, szLogMessage);
		return -1;
	}

	return 0;
}

/**
 * @brief
 * @param
 * @author chifac08
 * @return
 */
int sendProcessData()
{
	int iRet = -1;



	return iRet;
}
