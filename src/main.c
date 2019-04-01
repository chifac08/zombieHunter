#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <proc/procps.h>
#include <dirent.h>
#include <procutils.h>
#include <logutils.h>

#define CONFIG_FILE "/etc/opt/zombieHunter/service/hunter.conf"
#define LOG_FILE "/home/flichten/test.log"

int main(int argc, char **argv)
{
	//TODO: ifdef DEBUG
	setvbuf(stdout, NULL, _IONBF, 0);
    int* processList = NULL;
    
    initLogging(DEBUG, LOG_FILE);
    logIt(INFO, "Test");
    logIt(INFO, "Test2");
    logIt(INFO, "Test3");
    logIt(INFO, "Test4");
    logIt(INFO, "Test5");

    logIt(INFO, "TEST6");
    logIt(INFO, "Test7");
    logIt(INFO, "Test8");
    logIt(INFO, "Test9");
    logIt(INFO, "Test10");

    logIt(INFO, "TEST6");


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
