//
// debug.cpp
//
// implementation of debug functions
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"


void DebugPrint(const char *msg)
{
	#ifdef DEBUG
	printf("%s\n", msg);
	#endif
}

void DebugPrintNumber(const char *msg, int value)
{
	#ifdef DEBUG
	printf("%s %d\n", msg, value);
	#endif
}

void DebugPrintHex(const char *msg, int value)
{
	#ifdef DEBUG
	printf("%s 0x%x\n", msg, value);
	#endif
}

void DebugPrintHexHex(const char *msg, int value1, int value2)
{
	#ifdef DEBUG
	printf("%s 0x%x 0x%x\n", msg, value1, value2);
	#endif
}

void DebugPrintFloat(const char *msg, float value)
{
	#ifdef DEBUG
	printf("%s %f\n", msg, value);
	#endif
}

void DebugPrintString(const char *msg, char *str)
{
	#ifdef DEBUG
	printf("%s %s\n", msg, str);
	#endif
}
