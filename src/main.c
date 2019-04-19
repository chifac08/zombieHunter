#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <proc/procps.h>
#include <dirent.h>
#include "SCLogger.h"
#include "procutils.h"
#include "configutils.h"


int main(int argc, char **argv)
{
	//TODO: ifdef DEBUG
	setvbuf(stdout, NULL, _IONBF, 0);
    int* processList = NULL;
    CONFIG config;

    config = parseConfig();

    initLogging(config.logConfig);
    logIt(INFO, "Test");
    logIt(INFO, "test2");

    while(1)
    {
        /*
        processList = getProcessList(PROCESS_DIR);
        printf("%d", *(processList+0));

        checkProcessState(processList);

        processList = NULL;
         */
        sleep(10);
    }

    destroyLogging();

	return 0;
}
