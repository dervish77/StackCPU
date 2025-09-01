//
// opCodes.cpp
//
// definition of stack cpu instruction op code table
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opCodes.h"


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
5,	"LDD", OPC_LDD, 0, 2,
6,	"STM", OPC_STM, 2, 4,
7,	"STI", OPC_STI, 0, 2,
8,	"STD", OPC_STD, 0, 2,

9,	"ADD", OPC_ADD, 0, 4,
10,	"SUB", OPC_SUB, 0, 4,
11,	"NEG", OPC_NEG, 0, 3,
12,	"LSR", OPC_LSR, 0, 3,
13,	"LSL", OPC_LSL, 0, 3,

14,	"AND", OPC_AND, 1, 4,
15,	"ORR", OPC_ORR, 1, 4,
16,	"XOR", OPC_XOR, 1, 4,
17,	"INV", OPC_INV, 0, 3,

18,	"CPE", OPC_CPE, 1, 5,
19,	"CNE", OPC_CNE, 1, 5,
20,	"BRZ", OPC_BRZ, 2, 4,
21,	"BRN", OPC_BRN, 2, 4,
22,	"BRU", OPC_BRU, 2, 3,

23,	"INP", OPC_INP, 0, 2,
24,	"OUT", OPC_OUT, 0, 2,
25,	"SER", OPC_SER, 0, 2,
26,	"PRT", OPC_PRT, 0, 2,

27,	"NOP", OPC_NOP, 0, 1,
28,	"CLS", OPC_CLS, 0, 1,
29,	"END", OPC_END, 0, 1,
30,	"RST", OPC_RST, 0, 3,
-1,	" ", 0, 0, 0,
};



// end of file
