/**
 * Reading and writing configuration and settings for service
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "configutils.h"

static const char* config_types[] = {
		"loglevel",
		"logfile",
		"logrotate",
		"listenport",
		"logbuffer"
};

static void buildConfig(char* cpSearchString, CONFIG* config)
{
	char* cpValue = NULL;
	bool bFound = false;

	cpValue = strtok(cpSearchString, LOG_FILE_DELIMITER);

	for(int i = 0; i < sizeof(config_types) && !bFound; i++)
	{
		bFound=false;
		if(strcmp(cpValue, config_types[i]) == 0)
		{
			cpValue = strtok(NULL, LOG_FILE_DELIMITER);
			cpValue[strlen(cpValue)-1]='\0';
			switch(i)
			{
				case 0:
					config->logLevel = parseLogLevel(cpValue);
					bFound = true;
					break;
				case 1:
					snprintf(config->logFile, sizeof(config->logFile)-1, cpValue);
					bFound = true;
					break;
				case 2:
					config->logRotate=atoi(cpValue);
					bFound = true;
					break;
				case 3:
					config->listenPort=atoi(cpValue);
					bFound = true;
					break;
				case 4:
					config->logBuffer=atoi(cpValue);
					bFound = true;
					break;
			}
		}
	}
}

/**
 * @brief parse config file and return a struct with read information
 * @author chifac08
 * @return config struct
 */
CONFIG parseConfig()
{
    FILE* fpConfigFile = NULL;
    char szBuffer[100] = {0};
    int iBufferCount = 100;
    CONFIG config;
    char* cpConfigFile = NULL;

    memset(&config, 0, sizeof(CONFIG));

    cpConfigFile = getenv("ZH_CONFIG_FILE");

    if(!fpConfigFile && cpConfigFile)
    	fpConfigFile = fopen(cpConfigFile, "r");
    else
    	fpConfigFile = fopen(DEFAULT_CONFIG_FILE, "r");

    while(fgets(szBuffer, iBufferCount, fpConfigFile))
    {
    	buildConfig(szBuffer, &config);
    }

    if(config.logFile[0] == '\0')
    	snprintf(config.logFile, sizeof(config.logFile)-1, DEFAULT_LOG_FILE);

    if(config.logRotate == 0)
    	config.logRotate = DEFAULT_LOG_CACHE;

    fclose(fpConfigFile);

    return config;
}
