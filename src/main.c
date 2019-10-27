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
    FILE_WATCHER_ARG fWatcherArg;
    char szLogMessage[1024] = {0};
    pthread_t watcherId, commId;
    int* processList = NULL;
    int iRet = 0;

    memset(&config, 0, sizeof(CONFIG));
    memset(&fWatcherArg, 0, sizeof(fWatcherArg));

    config = parseConfig();

    initLogging(config.logConfig);
    logIt(INFO, "Start");

    if(config.checkIntervall == 0)
    	config.checkIntervall = DEFAULT_CHECK_INTERVALL;

    //create dir for zombie process data
	iRet = createDir(TEMP_FILE_DIR, S_IRWXU);

	if(iRet != 0)
		goto cleanup;

    //init file watcher
    fWatcherArg.iWatcher = initWatcher();
    fWatcherArg.zombie_process_head = NULL;
    fWatcherArg.zombie_process_tail = NULL;
    addDirectory(fWatcherArg.iWatcher, TEMP_FILE_DIR);

    //thread for file watcher
    pthread_create(&watcherId, NULL, watch, &fWatcherArg);

    //thread for backoffice communication
    pthread_create(&commId, NULL, test, &fWatcherArg);

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
	cleanup:
		pthread_cancel(watcherId);

		deleteList(fWatcherArg.zombie_process_head);

		if(removeFile(STOP_FLAG_FILE) != 0)
		{
			destroyLogging();
			return -1;
		}

		destroyLogging();

	return 0;
}
