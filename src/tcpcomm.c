/*
 * tcpcomm.c
 *
 * Creates an tcp socket and connects to a given host and port
 * All information must be provided via CONFIG struct
 *
 *  Created on: Sep 20, 2019
 *      Author: chifac08
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include "tcpcomm.h"
#include "SCLogger.h"
#include "basement.h"

//vars
CONFIG* tcpConfig;
short hSocket;
char szLogMessage[512];

/**
 * @brief creates a tcp socket
 * @author chifac08
 * @return -1    ... NOK
 *         != -1 ... OK
 */
short createSocket(CONFIG config)
{
	memset(szLogMessage, 0, sizeof(szLogMessage));
	tcpConfig = NULL;

	if(!tcpConfig)
	{
		tcpConfig = malloc(sizeof *tcpConfig);
		*tcpConfig = config;
	}

	logIt(DEBUG, "Create tcp socket");
	hSocket = socket(AF_INET, SOCK_STREAM, 0);

	formatLog(szLogMessage, sizeof(szLogMessage), "TCP Socket returned %d", hSocket);
	logIt(DEBUG, szLogMessage);

	return hSocket;
}

/**
 * @brief connects to a tcp socket
 * @author chifac08
 * @return 0 ... OK
 *      != 0 ... NOK
 */
int connectSocket()
{
	int iRet = -1;
	struct sockaddr_in remote;

	memset(szLogMessage, 0, sizeof(szLogMessage));
	memset(&remote, 0, sizeof(struct sockaddr_in));

	logIt(DEBUG, "Trying to connect to tcp socket");
	formatLog(szLogMessage, sizeof(szLogMessage), "IP: %s Port: %d", tcpConfig->host, tcpConfig->port);
	logIt(DEBUG, szLogMessage);

	remote.sin_addr.s_addr = inet_addr(tcpConfig->host);
	remote.sin_family = AF_INET;
	remote.sin_port = htons(tcpConfig->port);

	iRet = connect(hSocket, (struct sockaddr*)&remote, sizeof(struct sockaddr_in));

	memset(szLogMessage, 0, sizeof(szLogMessage));
	formatLog(szLogMessage, sizeof(szLogMessage), "Connection returned: %d", iRet);
	logIt(DEBUG, szLogMessage);

	return iRet;
}

/**
 * @brief send data to a socket
 * @param hSocket    ... current created socket
 * @param cpRequest  ... data to send
 * @param lenRequest ... length of data to be sent
 * @author chifac08
 * @return 0 ... OK
 *      != 0 ... NOK
 */
int sendSocket(char* cpRequest, short lenRequest)
{
	int shortRetVal = -1;
	struct timeval tv;

	memset(szLogMessage, 0, sizeof(szLogMessage));
	memset(&tv, 0, sizeof(struct timeval));
	tv.tv_sec = 20;
	tv.tv_usec = 0;

	if(setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(struct timeval)) < 0)
	{
		logIt(ERROR, "Connection timed out!");
		return -1;
	}

	shortRetVal = send(hSocket, cpRequest, lenRequest, 0);

	return shortRetVal;
}

/**
 * @brief receive data from socket
 * @param cpRc  ... received data
 * @param lenRc ... length of received data
 * @author chifac08
 * @return 0 ... OK
 *      != 0 ... NOK
 */
int receiveSocket(char* cpRc, short lenRc)
{
	int shortRetval = -1;
	struct timeval tv;

	memset(szLogMessage, 0, sizeof(szLogMessage));
	memset(&tv, 0, sizeof(struct timeval));

	logIt(DEBUG, "Receiving data from socket");

	tv.tv_sec = 20;  /* 20 Secs Timeout */
	tv.tv_usec = 0;

	if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(tv)) < 0)
	{
	  logIt(ERROR, "Connected timed out!");
	  return -1;
	}

	shortRetval = recv(hSocket, cpRc , lenRc , 0);

	snprintf(szLogMessage, sizeof(szLogMessage)-1, "Response: %s", cpRc);
	logIt(DEBUG, szLogMessage);

	return shortRetval;
}

/**
 * @brief shutdown/close socket and free memory
 * @author chifac08
 */
void destroySocket()
{
	close(hSocket);
	shutdown(hSocket, 0);

	if(tcpConfig)
		free(tcpConfig);

	memset(szLogMessage, 0, sizeof(szLogMessage));
}
