#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <proc/procps.h>
#include <dirent.h>
#include <pthread.h>
#include "SCLogger.h"
#include "procutils.h"
#include "configutils.h"
#include "tcpcomm.h"
#include "basement.h"
#include "watcher.h"

/**
 * List of exit codes:
 *
 * 0  ... OK
 * -1 ... could not delete a file
 */

int main(int argc, char **argv)
{
    CONFIG config;
    ZOMBIE_NODE* head = NULL;
    char szLogMessage[1024] = {0};
    pthread_t watcherId;
    int iWatcher = -1;
    int* processList = NULL;
    int iRet = 0;

    memset(&config, 0, sizeof(CONFIG));

    config = parseConfig();

    initLogging(config.logConfig);
    logIt(INFO, "Start");

    if(config.checkIntervall == 0)
    	config.checkIntervall = DEFAULT_CHECK_INTERVALL;

    //create dir for zombie process data
	iRet = createDir(TEMP_FILE_DIR, S_IRWXU);

	//TODO: implement cleanup method

    //init file watcher
    iWatcher = initWatcher();
    addDirectory(iWatcher, TEMP_FILE_DIR);
    pthread_create(&watcherId, NULL, watch, (void*)(intptr_t)iWatcher);

    while(1)
    {
    	if(checkStopFlag(STOP_FLAG_FILE))
    	{
    		break;
    	}

        processList = getProcessList(DEFAULT_PROCESS_DIR);

        checkProcessState(processList);

        processList = NULL;

        sleep(config.checkIntervall);
    }

    logIt(INFO, "End");

    //end procedures
	pthread_cancel(watcherId);

	if(removeFile(STOP_FLAG_FILE) != 0)
	{
		destroyLogging();
		return -1;
	}

	destroyLogging();

	return 0;
}
