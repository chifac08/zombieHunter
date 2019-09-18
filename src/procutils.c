#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <procutils.h>

#define PROCESS_DIR "/proc"
#define PROCESS_ARRAY_SIZE 1000

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
    
    if(processList == NULL)
        return;
        
    for(int i = 0; i < PROCESS_ARRAY_SIZE; i++)
    {
    	memset(szPath, 0, sizeof(szPath));
        snprintf(szPath, sizeof(szPath)-1, "%s/%d/status", PROCESS_DIR, *(processList+i));
        
        procFile = fopen(szPath, "r");
        
        if(procFile == NULL)
            continue;
        
        while(fgets(szBuffer, sizeof(szBuffer), procFile) != NULL)
        {
            if(strstr(szBuffer, "(zombie)"))
            {
                printf("%d: %s",*(processList+i), szBuffer);
                break;
            }  
            memset(szBuffer, 0, sizeof(szBuffer));
        }
        
        fclose(procFile);
        procFile = NULL;
    }
}
