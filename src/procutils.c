#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <procutils.h>
#include <time.h>
#include <errno.h>
#include "SCLogger.h"
#include "basement.h"

//vars



/**
 * @brief Loads all active processes from a given dir (usually /proc)
 * @author chifac08
 * @param cpDir ... Process dir
 * @return list with PIDs
 */
int* getProcessList(const char* cpDir)
{
    //TODO: dynamic allocated process list
    int iCounter = 0;
    static int processList[PROCESS_ARRAY_SIZE];
    DIR* dir = NULL;
    struct dirent* de = NULL;
    char szLogMessage[1024] = {0};
    
    dir = opendir(cpDir);
    
    if(dir == NULL)
    {
    	formatLog(szLogMessage, sizeof(szLogMessage), "Could not open directory: %s. Error: [%d] - %s", cpDir, errno, strerror(errno));
    	logIt(ERROR, szLogMessage);
        return NULL;
    }
    
    while((de = readdir(dir)) != NULL)
    {
        if(atol(de->d_name))
        {
            processList[iCounter] = atol(de->d_name);
            iCounter++;
        }
    }
    
    if(closedir(dir) < 0)
    {
    	formatLog(szLogMessage, sizeof(szLogMessage), "Could not close directory: %s. Error: [%d] - %s", cpDir, errno, strerror(errno));
    	logIt(ERROR, szLogMessage);
    }

    return processList;
}

/**
 * @brief checks the state of a process and backups the status file if process is a zombie
 * @author chifac08
 * @param processList ... list with pids
 */
void checkProcessState(int* processList)
{
    FILE* procFile = NULL;
    char szPath[512] = {0};
    char szBuffer[1024] = {0};
    char szLogMessage[1024] = {0};
    bool bIsZombie = false;
    int iLineCount = 0;
    char szDestFile[64] = {0};
    int iCopyRet = -1;
    
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
            	formatLog(szLogMessage, sizeof(szLogMessage), "Zombie found: %d",*(processList+i));
                logIt(WARN, szLogMessage);
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
        	memset(szDestFile, 0, sizeof(szDestFile));
        	createDir(TEMP_FILE_DIR, S_IRWXU);
        	snprintf(szDestFile, sizeof(szDestFile)-1, "%s/%d_%d", TEMP_FILE_DIR, *(processList+i), (int)time(NULL));
        	iCopyRet = copyFile(szPath, szDestFile);
        	//TODO: handle copy error (maybe for stats)
        }
    }
}
