/*
 * zombiequeue.c
 * Custom queue for zombie process files
 *
 *  Created on: Nov 3, 2019
 *      Author: chifac08
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "zombiequeue.h"


//TODO functions headers

/**
 *
 */
Queue* createQueue(int iMaxElements)
{
	Queue* queue = NULL;
	queue = (Queue*)malloc(sizeof(Queue));
	memset(queue, 0, sizeof(Queue));
	queue->capacity = iMaxElements;
	queue->size = 0;
	queue->front = 0;
	queue->rear = -1;
	queue->elements = (char**)malloc(sizeof(char*)*iMaxElements);
	memset(queue->elements, 0, sizeof(sizeof(char*)*iMaxElements));

	return queue;
}

void enqueue(Queue* queue, char* cpElement)
{
	if(queue->size == queue->capacity)
	{
		//TODO error handling
		return;
	}

	queue->size++;
	queue->rear = queue->rear+1;

	if(queue->rear == queue->capacity)
	{
		queue->rear = 0;
	}

	queue->elements[queue->rear] = (char*)malloc(strlen(cpElement)+1);
	memset(queue->elements[queue->rear], 0, strlen(cpElement)+1);
	memcpy(queue->elements[queue->rear], cpElement, strlen(cpElement));
}

char* front(Queue* queue)
{
	if(queue->size < 1)
		return NULL;

	return queue->elements[queue->front];
}

char* dequeue(Queue* queue)
{
	char* cpReturn = NULL;

	if(queue->size < 1)
		return NULL;

	cpReturn = queue->elements[queue->front];
	queue->front = (queue->front + 1)%queue->capacity;
	queue->size -= 1;

	return cpReturn;
}

void clearQueue(Queue* queue)
{
	if(queue)
	{
		for(int i = 0; i < queue->size;i++)
		{
			if(queue->elements[i])
			{
				free(queue->elements[i]);
				queue->elements[i]=NULL;
			}
		}
		queue->size = 0;
		queue->front = 0;
		queue->rear = -1;
	}
}

void freeQueue(Queue* queue)
{
	clearQueue(queue);
	free(queue);
}

void copyQueue(Queue* sourceQueue, Queue** destQueue)
{
	if(sourceQueue)
	{
		*destQueue = createQueue(sourceQueue->capacity);

		for(int i = 0; i < sourceQueue->size;i++)
		{
			(*destQueue)->elements[i] = (char*)malloc(strlen(sourceQueue->elements[i])+1);
			memset((*destQueue)->elements[i], 0, strlen(sourceQueue->elements[i])+1);
			memcpy((*destQueue)->elements[i], sourceQueue->elements[i], strlen(sourceQueue->elements[i]));
		}
		(*destQueue)->capacity = sourceQueue->capacity;
		(*destQueue)->front = sourceQueue->front;
		(*destQueue)->rear = sourceQueue->rear;
		(*destQueue)->size = sourceQueue->size;
	}
}
