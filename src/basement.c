/*
 * basement.c
 *
 * Holds base functions.
 *
 *  Created on: Apr 7, 2019
 *      Author: chifac08
 */
#include <stdlib.h>
#include <string.h>
#include "basement.h"

/**
 * @brief Check if list contains element. list must have a fixed size!
 * @author chifac08
 * @return true or false
 */
bool contains(char** list, char* strTarget)
{
	int iListLength = strlen(list);
	bool bFound = false;

	for(int i = 0; i < iListLength;i++)
	{
		if(strcmp(strTarget, *(list+i)) == 0)
		{
			bFound = true;
			break;
		}
	}

	return bFound;
}
