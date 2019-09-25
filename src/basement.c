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
#include <sys/types.h>
#include <sys/stat.h>
#include "basement.h"
#include "typvars.h"
#include "tcpcomm.h"

/**
 * @brief Check if list contains element. list must have a fixed size!
 * @author chifac08
 * @return true or false
 */
bool contains(char** list, char* strTarget)
{
	int iListLength = strlen(list);
	bool bFound = false;

	for(int i = 0; i < iListLength;i++)
	{
		if(strcmp(strTarget, *(list+i)) == 0)
		{
			bFound = true;
			break;
		}
	}

	return bFound;
}

/**
 * @brief creates the header node
 * @param proc_stat ... struct with process information
 * @param next      ... pointer to the next node
 * @author chifac08
 * @return header node
 */
ZOMBIE_NODE* create(PROCESS_STATUS proc_stat, ZOMBIE_NODE* next)
{
	ZOMBIE_NODE* node = NULL;

	node = (ZOMBIE_NODE*)malloc(sizeof(ZOMBIE_NODE));

	if(!node)
	{
		logIt(ERROR, "Error creating a new zombie node!");
		return NULL;
	}

	node->proc_stat = proc_stat;
	node->next = next;

	return node;
}

/**
 * @brief insert a new node before the first one (FIFO)
 * @param head      ... current head node
 * @param proc_stat ... struct to insert
 * @author chifac08
 * @return header points to current inserted node
 */
ZOMBIE_NODE* prepend(ZOMBIE_NODE* head, PROCESS_STATUS proc_stat)
{
	ZOMBIE_NODE* node = NULL;

	node = create(proc_stat, head);
	head = node;

	return head;
}

/**
 * @brief counts all elements in a linked list
 * @param head ... pointer to head of linked list
 * @author chifac08
 * @return counted objects
 */
int count(ZOMBIE_NODE* head)
{
	ZOMBIE_NODE* cursor = NULL;
	int iCount = 0;

	cursor = head;
	while(cursor != NULL)
	{
		iCount++;
		cursor = cursor->next;
	}

	return iCount;
}

/*
 * @brief remove node from back of the list
 * @param head ... pointer to head of linked list
 * @author chifac08
 * @return current pointer to head
 */
ZOMBIE_NODE* removeBack(ZOMBIE_NODE* head)
{
	if(!head)
		return NULL;

	ZOMBIE_NODE* cursor = NULL;
	ZOMBIE_NODE* back = NULL;

	cursor = head;
	back = NULL;

	while(cursor->next != NULL)
	{
		back = cursor;
		cursor = cursor->next;
	}

	if(back)
		back->next=NULL;

	/* if this is the last node on the list*/
	if(cursor == head)
		head = NULL;

	if(cursor)
		free(cursor);

	return head;
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
		snprintf(szLogMessage, sizeof(szLogMessage)-1, "Cannot open file %s", cpFileName);
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
		memset(szLogMessage, 0, sizeof(szLogMessage));
		snprintf(szLogMessage, sizeof(szLogMessage)-1, "Cannot read file %s", cpFileName);
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
void createDir(char* cpDir, mode_t mode)
{
	struct stat st = {0};

	if(stat(cpDir, &st) == -1)
		mkdir(cpDir, mode);
}

/**
 * @brief
 * @param
 * @param
 * @author chifac08
 */
int copyFile(char* cpSourcePath, char* cpDestPath)
{
	FILE* cpSourceFile = NULL;
	FILE* cpDestFile = NULL;
	char szLogMessage[1024] = {0};

	cpSourceFile = fopen(cpSourceFile, "r");

	if(!cpSourceFile)
	{
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
