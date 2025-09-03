//
// opCodes.cpp
//
// definition of stack cpu instruction op code table
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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


//
// access functions
//
int IsValidOpCode( uint8_t opcode )
{
	int valid = 1;
	
	if (SearchOpCode( opcode ) == -1)
	{
		valid = 0;
	}
	
	return valid;
}

int SearchOpCode( uint8_t opcode )
{
	int i = 0;
	int index = -1;
	
	while( InstTable[i].index != -1 )
	{
		if (InstTable[i].opcode == opcode)
		{
			index = i;
			break;
		}
		else
		{
			i++;
		}
	}
	
	return index;
}

int SearchMnemonic( char *opstr )
{
	int i = 0;
	int index = -1;
	
	// convert to upper case
	char upstr[OP_NAME_LEN];
	for (int j=0; opstr[j] != '\0'; j++)
	{
		upstr[j] = toupper( opstr[j] );
	}
	
	// search for op code name
	while( InstTable[i].index != -1 )
	{
		if (strcmp(InstTable[i].mnemonic, upstr) == 0)
		{
			index = i;
			break;
		}
		else
		{
			i++;
		}
	}
	
	return index;
}


int GetOpCodeMnemonic( int index, char *str )
{
	int error = -1;
	
	if (index != -1)
	{
		if (InstTable[index].index == index)
		{
			strcpy( str, InstTable[index].mnemonic );
			error = 0;
		}
	}
	
	return error;
}

uint8_t GetOpCodeCode( int index )
{
	uint8_t opcode = 0xFF;
	
	if (index != -1)
	{
		if (InstTable[index].index == index)
		{
			opcode = InstTable[index].opcode;
		}
	}
	
	return opcode;
}

int GetOpCodeOperands( int index )
{
	int operands = -1;
	
	if (index != -1)
	{
		if (InstTable[index].index == index)
		{
			operands = InstTable[index].numops;
		}
	}
	
	return operands;
}

int GetOpCodeClocks( int index )
{
	int clocks = -1;
	
	if (index != -1)
	{
		if (InstTable[index].index == index)
		{
			clocks = InstTable[index].numclocks;
		}
	}
	
	return clocks;
}


//
// debug functions
//
void DumpOpCodeRecord( int index )
{
	if (index != -1)
	{
		printf("index %02d str %s op 0x%02X num %d clk %d\n", 
			InstTable[index].index,
			InstTable[index].mnemonic,
			InstTable[index].opcode,
			InstTable[index].numops,
			InstTable[index].numclocks);			
	}
}

void DumpOpCodeTable()
{
	int i = 0;
	
	printf("\n");
	
	while( InstTable[i].index != -1 )
	{
		DumpOpCodeRecord( i );
		i++;
	}
}


// end of file
