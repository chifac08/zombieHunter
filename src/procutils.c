#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <procutils.h>
#include <pthread.h>
#include "SCLogger.h"
#include "basement.h"

//vars



/**
 * @brief Loads all active processes from /proc
 * @author chifac08
 * @param cpDir ... Process dir
 * @return list with PIDs
 */
int* getProcessList(char* cpDir)
{
    //TODO: dynamic allocated process list
    int iCounter = 0;
    static int processList[PROCESS_ARRAY_SIZE];
    DIR* dir = NULL;
    struct dirent* de = NULL;
    
    dir = opendir(cpDir);
    
    if(dir == NULL)
    {
        printf("could not open directory: %s", cpDir);
        return 0;
    }
    
    while((de = readdir(dir)) != NULL)
    {
        if(atol(de->d_name))
        {
            processList[iCounter] = atol(de->d_name);
            iCounter++;
        }
    }
    
    return processList;
}

static void* thread_method(void* args)
{
	int iSize = 0;
	char* cpFileName = NULL;
	char* cpResult = NULL;

	cpFileName = (char*)args;

	iSize = loadFileInMemory(cpFileName, &cpResult);

	return (void*)iSize;
}

/**
 * @brief checks the state of a process
 * @author chifac08
 * @param processList ... list with pids
 */
void checkProcessState(int* processList)
{
    FILE* procFile = NULL;
    char szPath[512] = {0};
    char szBuffer[1024] = {0};
    bool bIsZombie = false;
    int iLineCount = 0;
    int iFileSize = 0;
    pthread_t p1;
    
    if(processList == NULL)
        return;
        
    for(int i = 0; i < PROCESS_ARRAY_SIZE; i++)
    {
    	iLineCount = 1;
    	memset(szBuffer, 0, sizeof(szBuffer));
    	memset(szPath, 0, sizeof(szPath));
    	bIsZombie = false;
        snprintf(szPath, sizeof(szPath)-1, "%s/%d/status", PROCESS_DIR, *(processList+i));
        
        procFile = fopen(szPath, "r");
        
        if(procFile == NULL)
            continue;
        
        while(fgets(szBuffer, sizeof(szBuffer), procFile) != NULL)
        {
            if(strstr(szBuffer, "(zombie)"))
            {
                printf("%d: %s",*(processList+i), szBuffer);
                bIsZombie = true;
                break;
            }  
            else if(iLineCount > PROCESS_STATE_LINE)
            {
            	//end loop if reading pointer is over State line and process is no zombie. saves time
            	break;
            }
            iLineCount++;
            memset(szBuffer, 0, sizeof(szBuffer));
        }

        fclose(procFile);
        procFile = NULL;

        if(bIsZombie)
        {
        	pthread_create(&p1, NULL, thread_method, &szPath);
        	pthread_join(p1, NULL);
        }
    }
}

/*
PROC_NODE* create(FILE* cpProcFile, PROC_NODE* next)
{
	PROC_NODE* node = NULL;

	node = (PROC_NODE*)malloc(sizeof(PROC_NODE));

	if(!node)
	{
		logIt(ERROR, "Error creating a new proc node!");
		return -1;
	}


	node->next = next;

	return node;
}

PROC_NODE* prepend(PROC_NODE* head, FILE* cpProcFile)
{

}

PROCESS_STATUS loadProcessInfo()
{
	char szBuffer[1024] = {0};


	while(fgets(szBuffer, sizeof(szBuffer),
}
*/
