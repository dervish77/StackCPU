//
// opCodes.h
//
// definition of stack cpu op codes
//

#ifndef __opCodes_h
#define __opCodes_h

#include <cstdint>

//
// define op codes
//

#define OPC_PSH		0x11
#define OPC_PSA		0x12
#define OPC_POP		0x14
#define OPC_LDM		0x21
#define OPC_LDI		0x22
#define OPC_LDD		0x24
#define OPC_STM		0x31
#define OPC_STI		0x32
#define OPC_STD		0x34

#define OPC_ADD		0x41
#define OPC_SUB		0x42
#define OPC_NEG		0x44
#define OPC_LSR		0x48
#define OPC_LSL		0x4A

#define OPC_AND		0x51
#define OPC_ORR		0x52
#define OPC_XOR		0x54
#define OPC_INV		0x58

#define OPC_CPE		0x61
#define OPC_CNE		0x62
#define OPC_BRZ		0x71
#define OPC_BRN		0x72
#define OPC_BRU		0x74

#define OPC_INP		0x81
#define OPC_OUT		0x82
#define OPC_SER		0x84
#define OPC_PRT		0x88

#define OPC_NOP		0x90
#define OPC_CLS		0x92
#define OPC_END		0x98
#define OPC_RST		0x9F


// structure for instruction op code table
struct Instruction_s {
	int index;				// reference index (-1 means end of table)
	const char mnemonic[4];	// "ABC"
	uint8_t opcode;			// see defines above
	int numops;				// number of operands
	int numclocks;			// number of clock cycles
};

// typedef for instruction op code table
typedef struct Instruction_s Instruction_s_t;

extern Instruction_s_t InstTable[];


#endif /* __opCodes_h */