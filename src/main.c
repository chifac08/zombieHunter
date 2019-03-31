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
    
    initLogging(INFO, LOG_FILE);
    logIt(DEBUG, "Test");
    logIt(DEBUG, "Test2");
    logIt(DEBUG, "Test3");
    test();


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
