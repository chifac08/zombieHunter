/**
 * Reading and writing configuration and settings for service
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "basement.h"
#include "configutils.h"

static const char* config_types[] = {
		"loglevel",
		"logfile",
		"logrotate",
		"listenport"
};

static void buildConfig(char* cpSearchString, CONFIG* config)
{
	for(int i = 0; i < sizeof(config_types); i++)
	{
		if(strstr(cpSearchString, config_types[i]))
		{
			switch(i)
			{
				case 0:
					printf("test");
					break;
			}
			break;
		}
	}
}

/**
 * @brief parse config file and return a struct with read information
 * @author chifac08
 * @return
 */
CONFIG* parseConfig(char* cpConfigFile)
{
    FILE* fpConfigFile = NULL;
    char szBuffer[100] = {0};
    int iBufferCount = 100;
    CONFIG config;
    char* cpValue = NULL;

    memset(&config, 0 , sizeof(CONFIG));

    if(!fpConfigFile)
    	fpConfigFile = fopen(cpConfigFile, "r");

    if(!cpConfigFile)
    	getenv("ZH_CONFIG_FILE");

    while(fgets(szBuffer, iBufferCount, fpConfigFile))
    {
    	cpValue = strtok(szBuffer, LOG_FILE_DELIMITER);
    	while(cpValue != NULL)
    	{
    		cpValue = strtok(NULL, LOG_FILE_DELIMITER);
    	}
    }

    fclose(fpConfigFile);
}

void parseValue(char* szBuffer, CONFIG* config)
{

}
