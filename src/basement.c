/*
 * basement.c
 *
 * Holds base functions.
 *
 *  Created on: Apr 7, 2019
 *      Author: chifac08
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "basement.h"
#include "SCLogger.h"
#include "tcpcomm.h"
#include "zombiequeue.h"

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

void loadAllFilesFromDir()
{
	struct dirent* de;

	DIR* dir = opendir(TEMP_FILE_DIR);

	if(!dir)
	{
		printf("Problem");
		return NULL;
	}

	while((de = readdir(dir)) != NULL)
		printf("%s\n", de->d_name);
}

/**
 *
 * @author chifac08
 */
void sendProcessData(void* arg)
{
	FILE_WATCHER_ARG* fWatcherArg = (FILE_WATCHER_ARG*)arg;
	char szLogMessage[1024] = {0};
	FILE* fZombieFile = NULL;
	char* cpReturn = NULL;
	char szFilePath[512] = {0};
	char szBuffer[READER_BUFFER] = {0};
	char* cpHelper = NULL;
	int iRc = 0;
	Queue* zombieQueueSnap = NULL;

	while(1)
	{
		if(fWatcherArg->zombie_queue->size < 1)
		{
			//TODO switch for time
			sleep(30);
			continue;
		}

		iRc = pthread_mutex_trylock(&fWatcherArg->mutex);

		if(iRc == EBUSY)
		{
			sleep(5);
			continue;
		}

		copyQueue(fWatcherArg->zombie_queue, &zombieQueueSnap);
		clearQueue(fWatcherArg->zombie_queue);

		pthread_mutex_unlock(&fWatcherArg->mutex);

		while(zombieQueueSnap->size > 0)
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			memset(szFilePath, 0, sizeof(szFilePath));

			cpReturn = dequeue(zombieQueueSnap);

			if(!cpReturn)
			{
				logIt(ERROR, "No path for file!");
	//			pthread_mutex_unlock(&fWatcherArg->mutex);
				sleep(5);
				continue;
			}

			snprintf(szFilePath, sizeof(szFilePath)-1, "%s/%s", TEMP_FILE_DIR, cpReturn);

			fZombieFile = fopen(szFilePath, "r");

			if(!fZombieFile)
			{
				formatLog(szLogMessage, sizeof(szLogMessage), "Cannot open file %s. Error: [%d] - %s", szFilePath, errno, strerror(errno));
				logIt(ERROR, szLogMessage);
				continue;
			}

			while(fgets(szBuffer, READER_BUFFER, fZombieFile) != NULL)
			{
				cpHelper = strtok(szBuffer, "\t");
				while(cpHelper != NULL)
				{
					//printf("%s", cpHelper);
					cpHelper = strtok(NULL, "\t");
				}
				cpHelper = NULL;
				memset(szBuffer, 0, sizeof(szBuffer));
			}

			printf("Send file %s\n", szFilePath);

			fclose(fZombieFile);
			fZombieFile = NULL;

			removeFile(szFilePath);

			if(cpReturn)
			{
				free(cpReturn);
				cpReturn = NULL;
			}
		}

		if(zombieQueueSnap)
		{
			free(zombieQueueSnap);
			zombieQueueSnap=NULL;
		}

		sleep(30);
	}
}
