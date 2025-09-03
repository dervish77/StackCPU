//
// debug.h
//
// definition of debug functions
//

#ifndef __debug_h
#define __debug_h


//#define DEBUG


#define UNUSED(x) (void)(x)


//
// prototypes
//

void DebugPrint(const char *msg);

void DebugPrintNumber(const char *msg, int value);

void DebugPrintHex(const char *msg, int value);

void DebugPrintHex16(const char *msg, int value);

void DebugPrintHexHex(const char *msg, int value1, int value2);

void DebugPrintFloat(const char *msg, float value);

void DebugPrintString(const char *msg, char *str);


#endif /* __debug_h */
