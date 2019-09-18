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
    char *cpDir = "/proc/";
    CONFIG config;

    config = parseConfig();

    initLogging(config.logConfig);
    logIt(INFO, "Start");

    while(1)
    {
        processList = getProcessList(cpDir);

        checkProcessState(processList);

        processList = NULL;

        sleep(10);
    }

    logIt(INFO, "End");

    destroyLogging();

	return 0;
}
