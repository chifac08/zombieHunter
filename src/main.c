#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <proc/procps.h>
#include <dirent.h>
#include "SCLogger.h"
#include "basement.h"
#include "procutils.h"
#include "configutils.h"
#include "tcpcomm.h"
#include "watcher.h"
#include "zombiequeue.h"

/**
 * List of exit codes:
 *
 * 0  ... OK
 * -1 ... could not delete a file
 * -2 ... could not initialize mutex
 */
ZOMBIE_ARG zombieArg;

int main(int argc, char **argv)
{
    CONFIG config;
    char szLogMessage[1024] = {0};
    int* processList = NULL;
    int iRet = 0;

    memset(&config, 0, sizeof(CONFIG));
    memset(&zombieArg, 0, sizeof(zombieArg));

    config = parseConfig();

    initLogging(config.logConfig);
    logIt(INFO, "Start");

    if(config.checkIntervall == 0)
    	config.checkIntervall = DEFAULT_CHECK_INTERVALL;

    //install signal handler
    installSignalHandler();

    //create dir for zombie process data
	iRet = createDir(TEMP_FILE_DIR, S_IRWXU);

	if(iRet != 0)
		goto cleanup;

    //init file watcher
    zombieArg.iWatcher = initWatcher();
    zombieArg.zombie_queue = createQueue(DEFAULT_QUEUE_SIZE);

    //init mutex
    if (pthread_mutex_init(&zombieArg.mutex, NULL) != 0)
    {
        logIt(ERROR, "could not init mutex!");
        iRet = -2;
        goto cleanup;
    }

    //add watcher to directory
    addDirectory(zombieArg.iWatcher, TEMP_FILE_DIR);

    //thread for file watcher
    pthread_create(&zombieArg.watcherId, NULL, watch, &zombieArg);

    //thread for backoffice communication
    pthread_create(&zombieArg.commId, NULL, sendProcessData, &zombieArg);


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
		iRet = cleanup(zombieArg);

	return iRet;
}

/**
 * Handles different UNIX Signals
 *
 * @param iSignal ... signal code
 * @author chifac08
 */
void signalHandler(unsigned int iSignal)
{
	int iRet=0;

	switch(iSignal)
	{
		case SIGINT:
			if(createStopFlag() < 0)
			{
				iRet=cleanup(zombieArg);
				exit(iRet);
			}
			break;
	}
}

/**
 * Installs various signal handler
 *
 * @author chifac08
 */
void installSignalHandler()
{
	signal(SIGINT, signalHandler);
}
