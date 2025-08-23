//
// debug.cpp
//
// implementation of debug functions
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"


void DebugPrint(char *msg)
{
	#ifdef DEBUG
	printf("%s\n", msg);
	#endif
}

void DebugPrintNumber(char *msg, int value)
{
	#ifdef DEBUG
	printf("%s %d\n", msg, value);
	#endif
}

void DebugPrintHex(char *msg, int value)
{
	#ifdef DEBUG
	printf("%s 0x%x\n", msg, value);
	#endif
}

void DebugPrintHexHex(char *msg, int value1, int value2)
{
	#ifdef DEBUG
	printf("%s 0x%x 0x%x\n", msg, value1, value2);
	#endif
}

void DebugPrintFloat(char *msg, float value)
{
	#ifdef DEBUG
	printf("%s %f\n", msg, value);
	#endif
}

void DebugPrintString(char *msg, char *str)
{
	#ifdef DEBUG
	printf("%s %s\n", msg, str);
	#endif
}
