/*
 * tcpcomm.h
 *
 *  Created on: Sep 20, 2019
 *      Author: chifac08
 */
#ifndef INCL_TCPCOMM_H_
#define INCL_TCPCOMM_H_

//defines

//vars

//functions
short createSocket();
int connectSocket();
int sendSocket(char* cpRequest, short lenRequest);
int receiveSocket(char* cpRc, short lenRc);
void destroySocket();

#endif /* INCL_TCPCOMM_H_ */
