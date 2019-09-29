/*
 * watcher.c
 *
 * Filewatcher functionalities
 *
 *  Created on: Sep 26, 2019
 *      Author: chifac08
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include "watcher.h"
#include "SCLogger.h"
#include "basement.h"

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

/**
 * @brief initialize the inotify watcher
 * @author chifac08
 * @return != -1 ... OK
 *            -1 ... NOK
 */
int initWatcher()
{
	int iFileDescriptor = -1;
	char szLogMessage[1024] = {0};

	iFileDescriptor = inotify_init();

	if(iFileDescriptor < 0)
	{
		formatLog(szLogMessage, "Could not initialize inotify watcher! Return code: %d", iFileDescriptor);
		logIt(ERROR, szLogMessage);
		return -1;
	}

	return iFileDescriptor;
}

/**
 * @brief adds a watcher to a specific directory
 * @param iFileDescriptor ... initialized watcher
 * @param cpDirectory ... absolute path to watch
 * @author chifac08
 * @ return -1 ... NOK
 *       != -1 ... write descriptor
 */
int addDirectory(int iFileDescriptor, char* cpDirectory)
{
	int iWriteDescriptor = -1;
	char szLogMessage[1024] = {0};

	iWriteDescriptor = inotify_add_watch(iFileDescriptor, cpDirectory, IN_CREATE | IN_DELETE );

	if(iWriteDescriptor < 0)
	{
		formatLog(szLogMessage, "Could not add write descriptor to directory %s. Return Code: %d", cpDirectory, iWriteDescriptor);
		logIt(ERROR, szLogMessage);
		return -1;
	}

	return iWriteDescriptor;
}

/**
 * @brief thread method to start the watcher
 * @author chifac08
 */
void* watch(void* arg)
{
	int iFileDescriptor = (int)arg;
	int iLength = 0;
	char szBuffer[EVENT_BUF_LEN] = {0};
	struct inotify_event *event = NULL;
	int i = 0;
	char szLogMessage[1024] = {0};

	while(1)
	{
		iLength = 0;
		memset(szBuffer, 0, sizeof(szBuffer));
		event = NULL;
		i = 0;
		iLength = read(iFileDescriptor, szBuffer, EVENT_BUF_LEN);

		while(i < iLength)
		{
			event = ( struct inotify_event * ) &szBuffer[ i ];

			if(event->len)
			{
				if ( event->mask & IN_CREATE ) {
					if ( event->mask & IN_ISDIR ) {
					  formatLog(szLogMessage, "New directory %s created.\n", event->name );
					  logIt(INFO, szLogMessage);
					}
					else {
					  formatLog(szLogMessage, "New file %s created.\n", event->name );
					  logIt(INFO, szLogMessage);
					}
				}
			}
			i += EVENT_SIZE + event->len;
		}
	}
}

/**
 * @brief wrapper function for removing a watcher from a directory
 * @param iFileDescriptor ... File Descriptor
 * @param iWriteDescript ... Write Descriptor
 * @author chifac08
 */
void removeWatcher(int iFileDescriptor, int iWriteDescriptor)
{
	inotify_rm_watch(iFileDescriptor, iWriteDescriptor);
}
