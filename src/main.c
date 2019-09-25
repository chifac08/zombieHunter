#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <proc/procps.h>
#include <dirent.h>
#include "SCLogger.h"
#include "procutils.h"
#include "configutils.h"
#include "tcpcomm.h"
#include "basement.h"

int main(int argc, char **argv)
{
    int* processList = NULL;
    char *cpDir = "/proc/";
    CONFIG config;
    ZOMBIE_NODE* head = NULL;
    char szLogMessage[1024] = {0};

    memset(&config, 0, sizeof(CONFIG));

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
