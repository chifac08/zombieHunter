/*
 * zombiequeue.h
 * Custom queue for zombie process files
 *
 *  Created on: Nov 3, 2019
 *      Author: chifac08
 */

#ifndef INCL_ZOMBIEQUEUE_H_
#define INCL_ZOMBIEQUEUE_H_

#include "typvars.h"

Queue* createQueue(int iMaxElements);
void enqueue(Queue* queue, char* cpElement);
char* front(Queue* queue);
char* dequeue(Queue* queue);
void clearQueue(Queue* queue);

#endif /* INCL_ZOMBIEQUEUE_H_ */
