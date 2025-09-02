//
// binUtils.h
//
// definition of binary utility functions
//

#ifndef __binUtils_h
#define __binUtils_h

#include "memSim.h"


#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

typedef char BOOLEAN_T;


//
// dump mem block
//
#define DEFAULT_BLOCK_SIZE		256		/* 256 bytes */
#define DEFAULT_BYTES_PER_ROW	16



//
// intel hex file 
//
#define DEFAULT_BYTES_PER_REC 16
#define MAX_BYTES_PER_REC 64

#define TYPE_DAT 0
#define TYPE_EOF 1
#define TYPE_ESA 2
#define TYPE_SSA 3
#define TYPE_ELA 4
#define TYPE_SLA 5


//
// helper functions
//
BOOLEAN_T _DumpBlock(memSim *mem, int offset, int end, int size, int bytes_per_row, FILE *out_file_p);


//
// prototypes
//

// DumpMemory - dump bytes from input source and send to terminal
int DumpMemory(memSim *mem, int start, int end);

// ConvertDataToHex	- convert binary data to intel hex format
int ConvertDataToHex(memSim *mem, int start, int end, FILE *out_file_p, int addr, int bytes_rec); 


#endif /* __binUtils_h */