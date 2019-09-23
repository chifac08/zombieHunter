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

typedef struct {
	char name[64];
	int age;
} TEST;

int main(int argc, char **argv)
{
	//TODO: ifdef DEBUG
	setvbuf(stdout, NULL, _IONBF, 0);
    int* processList = NULL;
    char *cpDir = "/proc/";
    CONFIG config;
    TEST test;

    memset(&test, 0, sizeof(TEST));
    snprintf(test.name, sizeof(test.name)-1, "Fabian");
    test.age = 26;

    memset(&config, 0, sizeof(CONFIG));

    config = parseConfig();

    initLogging(config.logConfig);
    logIt(INFO, "Start");

    createSocket(config);
    int iRet = connectSocket();

    int iRetSend = sendSocket((char*)&test, sizeof(test));

    char szRec[200] = {0};
    int iRec = receiveSocket(szRec, sizeof(szRec));

    logIt(INFO, szRec);

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
