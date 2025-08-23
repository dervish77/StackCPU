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
	#else
	UNUSED(msg);
	#endif
}

void DebugPrintNumber(const char *msg, int value)
{
	#ifdef DEBUG
	printf("%s %d\n", msg, value);
	#else
	UNUSED(msg);
	UNUSED(value);
	#endif
}

void DebugPrintHex(const char *msg, int value)
{
	#ifdef DEBUG
	printf("%s 0x%X\n", msg, value);
	#else
	UNUSED(msg);
	UNUSED(value);
	#endif
}

void DebugPrintHexHex(const char *msg, int value1, int value2)
{
	#ifdef DEBUG
	printf("%s 0x%X 0x%X\n", msg, value1, value2);
	#else
	UNUSED(msg);
	UNUSED(value1);
	UNUSED(value2);
	#endif
}

void DebugPrintFloat(const char *msg, float value)
{
	#ifdef DEBUG
	printf("%s %f\n", msg, value);
	#else
	UNUSED(msg);
	UNUSED(value);
	#endif
}

void DebugPrintString(const char *msg, char *str)
{
	#ifdef DEBUG
	printf("%s %s\n", msg, str);
	#else
	UNUSED(msg);
	UNUSED(str);
	#endif
}
