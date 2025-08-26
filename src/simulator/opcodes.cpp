//
// opcodes.cpp
//
// definition of stack cpu instruction op code table
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opcodes.h"


//
// instruction op code table
//
// Format:
//   index, mnemonic string, op code, num operands, num clocks
//

Instruction_s_t InstTable[] = {
0,	"PSH", OPC_PSH, 1, 2,
1,	"PSA", OPC_PSA, 0, 1,
2,	"POP", OPC_POP, 0, 1,
3,	"LDM", OPC_LDM, 2, 4,
4,	"LDI", OPC_LDI, 0, 2,
5,	"STM", OPC_STM, 2, 4,
6,	"STI", OPC_STI, 0, 2,

7,	"ADD", OPC_ADD, 0, 4,
8,	"SUB", OPC_SUB, 0, 4,
9,	"NEG", OPC_NEG, 0, 3,
10,	"LSR", OPC_LSR, 0, 3,
11,	"LSL", OPC_LSL, 0, 3,

12,	"AND", OPC_AND, 1, 4,
13,	"ORR", OPC_ORR, 1, 4,
14,	"XOR", OPC_XOR, 1, 4,
15,	"INV", OPC_INV, 0, 3,

16,	"CPE", OPC_CPE, 1, 5,
17,	"CNE", OPC_CNE, 1, 5,
18,	"BRZ", OPC_BRZ, 2, 4,
19,	"BRN", OPC_BRN, 2, 4,
20,	"BRU", OPC_BRU, 2, 3,

21,	"INP", OPC_INP, 0, 2,
22,	"OUT", OPC_OUT, 0, 2,
23,	"SER", OPC_SER, 0, 2,
24,	"PRT", OPC_PRT, 0, 2,

25,	"NOP", OPC_NOP, 0, 1,
26,	"CLS", OPC_CLS, 0, 1,
27,	"END", OPC_END, 0, 1,
28,	"RST", OPC_RST, 0, 3,
-1,	" ", 0, 0, 0,
};



// end of file
