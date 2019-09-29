/*
 * watcher.h
 *
 *  Created on: Sep 26, 2019
 *      Author: chifac08
 */

#ifndef INCL_WATCHER_H_
#define INCL_WATCHER_H_

//defines

//vars

//fn
int initWatcher();
int addDirectory(int iFileDescriptor, char* cpDirectory);
void* watch(void* arg);
void removeWatcher(int iFileDescriptor, int iWriteDescriptor);

#endif /* INCL_WATCHER_H_ */
