//
// debug.h
//
// definition of debug functions
//

#ifndef __debug_h
#define __debug_h


#define DEBUG


//
// prototypes
//

void DebugPrint(char *msg);

void DebugPrintNumber(char *msg, int value);

void DebugPrintFloat(char *msg, float value);

void DebugPrintString(char *msg, char *str);


#endif /* __debug_h */
