//
// coreSim.h
//
// implementation of Core simulator class
//

#include <iostream>
#include <cstdio>
#include <cstdint>
#include <cstring>

#include "coreSim.h"
#include "memSim.h"

#include "regArray.h"

#include "opCodes.h"

#include "debug.h"


//
// instruction op code table
//
// Format:
//   index, mnemonic string, op code, num operands, num clocks
//

Register_s_t RegTable[] = {
0,	"PC", REG_INDEX_PC, 2,
1,	"SP", REG_INDEX_SP, 2,
2,	"DR", REG_INDEX_DR, 2,
3,	"AC", REG_INDEX_AC, 1,
4,	"TR", REG_INDEX_TR, 1,
5,	"IR", REG_INDEX_IR, 1,
6,	"OR", REG_INDEX_OR, 1,
7,	"SR", REG_INDEX_SR, 1,
8,	"PR", REG_INDEX_PR, 1,
-1,	" ", 0, 0,
};


// default constructor
coreSim::coreSim()
{
	pMemSim = NULL;

	memsize = 0;
	
	pRegisters = new regArray(DEFAULT_REGISTER_SIZE);
	_resetRegisters();
	
	coreState = STATE_IDLE;
}

// constructor
coreSim::coreSim(memSim *mem)
{
	pMemSim = NULL;
	memsize = 0;
	pRegisters = NULL;
	
	// setup memory
	if (mem != NULL)
	{
		pMemSim = mem;
		memsize = pMemSim->GetSize();
	}
	
	// setup registers
	pRegisters = new regArray(DEFAULT_REGISTER_SIZE);
	_resetRegisters();
	
	coreState = STATE_IDLE;
}

// destructor
coreSim::~coreSim()
{
	delete pRegisters;
}

// -------------------------------------------------------------

// accessor - set memSim reference
void coreSim::SetMemRef(memSim *mem)
{
	if (mem != NULL)
	{
		pMemSim = mem;
		memsize = pMemSim->GetSize();
	}
}

// accessor - get memSim reference
memSim* coreSim::GetMemRef()
{
	return pMemSim;
}

// accessor - set memSim reference
void coreSim::SetReg(int index, uint16_t data)
{
	if (index < DEFAULT_REGISTER_SIZE)
	{
		pRegisters->Set(index, data);
	}
}

// accessor - get memSim reference
uint16_t coreSim::GetReg(int index)
{
	return pRegisters->Get(index);
}

// accessor - set memSim reference
int coreSim::SetState(int index, int state)
{
	UNUSED(index);
	UNUSED(state);
	
	return 0;
}

// accessor - get memSim reference
int coreSim::GetState(int index)
{
	UNUSED(index);
	
	return 0;
}

// accessor - search register name - returns index
int coreSim::SearchRegName(char *str)
{
	int i = 0;
	int index = -1;
	
	// convert to upper case
	char upstr[REG_NAME_LEN];
	for (int j=0; str[j] != '\0'; j++)
	{
		upstr[j] = toupper( str[j] );
	}
	
	// search for register name
	while( RegTable[i].index != -1 )
	{
		if (strcmp(RegTable[i].regstr, upstr) == 0)
		{
			index = RegTable[i].regindex;
			break;
		}
		else
		{
			i++;
		}
	}
	
	return index;
}

// accessor - search register index - returns index
int coreSim::SearchRegIndex(int regid)
{
	int i = 0;
	int index = -1;
	
	while( RegTable[i].index != -1 )
	{
		if (RegTable[i].regindex == regid)
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

// accessor - get register size - returns size in bytes
int coreSim::GetRegSize(int index)
{
	int size = -1;
	
	if (index != -1)
	{
		if (RegTable[index].index == index)
		{
			size = RegTable[index].size;
		}
	}
	
	return size;
}
	
// -------------------------------------------------------------

// operator - run the core
int coreSim::CoreRun()
{
	int done = 0;
	
	coreState = STATE_RUN;
	DebugPrint("CoreRun start");
	while (!done)
	{
		done = _doInstructionCycle();
	}
	DebugPrint("CoreRun end");
	return 1;
}

// operator - single step the core
int coreSim::CoreStep()
{
	int done = 0;
	
	coreState = STATE_SSTEP;
	DebugPrint("CoreStep start");
	done = _doInstructionCycle();
	DebugPrint("CoreStep end");
	return 1;
}

// operator - halt the core
int coreSim::CoreHalt()
{
	coreState = STATE_HALT;
	DebugPrint("CoreHalt start");

	return 1;
}

// operator - tick the clock
void coreSim::ClockTick()
{
	
}

// -------------------------------------------------------------

//
// unit test code
//
void coreSim::UnitTest(int testnum)
{
	int index;
	uint8_t memdata;
	uint8_t memdata2;
	uint16_t address;
	uint16_t regdata;
	
	switch (testnum)
	{
		case 1:
			regdata = GetReg(REG_INDEX_PC);
			DebugPrintHex16("reg pc", regdata);
			regdata = GetReg(REG_INDEX_SP);
			DebugPrintHex16("reg sp", regdata);
			regdata = GetReg(REG_INDEX_DR);
			DebugPrintHex16("reg dr", regdata);

			regdata = GetReg(REG_INDEX_AC);
			DebugPrintHex16("reg ac", regdata);
			SetReg( REG_INDEX_AC, 0x55 );
			DebugPrintHex16("write ac", regdata);
			regdata = GetReg(REG_INDEX_AC);
			DebugPrintHex16("reg ac", regdata);
			
			_copyRegister(REG_INDEX_AC, REG_INDEX_OR);
			DebugPrintHex16("reg or", regdata);
			break;
			
		case 2:
			// put program into memory
			pMemSim->Write(0x0000, 0x92); // CLS
			pMemSim->Write(0x0001, 0x11); // PSH 0x22
			pMemSim->Write(0x0002, 0x22);
			pMemSim->Write(0x0003, 0x11); // PSH 0x55
			pMemSim->Write(0x0004, 0x55);
			pMemSim->Write(0x0005, 0x31); // STM 0x0c00
			pMemSim->Write(0x0006, 0x0C);
			pMemSim->Write(0x0007, 0x00);
			pMemSim->Write(0x0008, 0x32); // STI
			pMemSim->Write(0x0009, 0x21); // LDM 0x0c00
			pMemSim->Write(0x000A, 0x0C); 
			pMemSim->Write(0x000B, 0x00);
			pMemSim->Write(0x000C, 0x22); // LDI
			pMemSim->Write(0x000D, 0x41); // ADD
			pMemSim->Write(0x000E, 0x82); // OUT	
			pMemSim->Write(0x000F, 0x98); // END
			_debugDumpMemory("program:", 0x0000, 16);
			break;
	
		case 3:
			// mock execution of above program
			DebugPrint("executing:");
			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR); // CLS
			pRegisters->Set(REG_INDEX_SP, MEM_STACK_END);
			
			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR); // PSH 
			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR);
			_pushStack(memdata);
			//_debugDumpMemory("stack:", 0x0FFC, 4);
			
			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR); // PSH
			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR);
			_pushStack(memdata);
			//_debugDumpMemory("stack:", 0x0FFC, 4);

			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR); // STM
			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR);
			memdata2 = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR);
			address = memdata << 8;
			address = address | memdata2;
			SetReg(REG_INDEX_DR, address);
			memdata = _popStack();
			pMemSim->Write(GetReg(REG_INDEX_DR), memdata);
			
			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR); // STI
			_incrementRegister(REG_INDEX_DR);
			memdata = _popStack();
			pMemSim->Write(GetReg(REG_INDEX_DR), memdata);
			
			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR); // LDM
			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR);
			memdata2 = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR);
			address = memdata << 8;
			address = address | memdata2;
			SetReg(REG_INDEX_DR, address);
			memdata = pMemSim->Read(GetReg(REG_INDEX_DR));
			_pushStack(memdata);
			
			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR); // LDI
			_incrementRegister(REG_INDEX_DR);
			memdata = pMemSim->Read(GetReg(REG_INDEX_DR));
			_pushStack(memdata);
			//_debugDumpMemory("stack:", 0x0FFC, 4);

			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR); // ADD
			memdata2 = _popStack(); // pop temp
			memdata = _popStack();  // pop ac
			pRegisters->Set(REG_INDEX_AC, memdata);
			memdata = pRegisters->Get(REG_INDEX_AC) + memdata2; // add ac + temp
			pRegisters->Set(REG_INDEX_AC, memdata);
			_pushStack(pRegisters->Get(REG_INDEX_AC)); // push ac

			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_INCR); // OUT
			memdata = _popStack();
			pRegisters->Set(REG_INDEX_OR, memdata);

			memdata = _fetchMemory(REG_INDEX_PC, FETCH_OP_NONE); // END
			break;

		case 4:	
			_debugDumpMemory("data:", 0x0c00, 4);
			_debugDumpMemory("stack:", 0x0FFC, 4);
			_debugDumpRegisters("registers:");
			break;
			
		case 5:	
			index = SearchOpCode( 0x31 );
			DebugPrint("Found 0x31");
			DumpOpCodeRecord(index);
			index = SearchMnemonic( "CPE" );
			DebugPrint("Found CPE");
			DumpOpCodeRecord(index);
			break;
			
		case 6:	
			DumpOpCodeTable();
			break;
			
		default:
			break;
	}
}

// -------------------------------------------------------------

//
// PRIVATE METHODS
//

void coreSim::_clearRegisters()
{
	int i;
	
	for (i = 0; i < DEFAULT_REGISTER_SIZE; i++)
	{
		pRegisters->Set(i, 0);
	}
}
	
void coreSim::_fillRegisters(uint16_t data)
{
	int i;
	
	for (i = 0; i < DEFAULT_REGISTER_SIZE; i++)
	{
		pRegisters->Set(i, data);
	}
}

void coreSim::_resetRegisters()
{
	_clearRegisters();
	
	pRegisters->Set(REG_INDEX_PC, MEM_PROG_START);
	pRegisters->Set(REG_INDEX_DR, MEM_DATA_START);
	pRegisters->Set(REG_INDEX_SP, MEM_STACK_END);	
}

void coreSim::_clearStack()
{
	pRegisters->Set(REG_INDEX_SP, MEM_STACK_END);
}

// -------------------------------------------------------------

void coreSim::_incrementRegister(int index)
{
	uint16_t regdata;
	regdata = pRegisters->Get(index);
	regdata++;
	pRegisters->Set(index, regdata);
}

void coreSim::_decrementRegister(int index)
{
	uint16_t regdata;
	regdata = pRegisters->Get(index);
	regdata--;
	pRegisters->Set(index, regdata);
}

void coreSim::_copyRegister(int fromreg, int toreg)
{
	uint16_t regdata;
	regdata = pRegisters->Get(fromreg);
	pRegisters->Set(toreg, regdata);
}

// -------------------------------------------------------------

uint8_t coreSim::_fetchMemory(int reg, int operation)
{
	uint8_t memdata;
	uint16_t regdata;
	
	regdata = pRegisters->Get(reg);
	memdata = pMemSim->Read( regdata );

	DebugPrintHexHex("fetched", regdata, memdata);

	switch(operation)
	{
		case FETCH_OP_INCR:
			_incrementRegister(reg);
			break;
		case FETCH_OP_DECR:
			_decrementRegister(reg);
			break;
		default:
			break;
	}

	return memdata;
}

uint8_t coreSim::_readMemory(uint16_t address)
{
	return pMemSim->Read( address );
}

void coreSim::_writeMemory(uint16_t address, uint8_t data)
{
	pMemSim->Write( address, data );
}	

void coreSim::_pushStack(uint8_t data)
{
	uint16_t regdata;
	
	// decrement SP
	_decrementRegister(REG_INDEX_SP);
	
	// write value to stack
	regdata = pRegisters->Get(REG_INDEX_SP);
	pMemSim->Write( regdata, data );

	//DebugPrintHexHex("pushstack", regdata, data);
}

uint8_t coreSim::_popStack()
{
	uint8_t memdata;
	uint16_t regdata;

	// read value from stack
	regdata = pRegisters->Get(REG_INDEX_SP);
	memdata = pMemSim->Read( regdata );
	
	// increment SP
	if (pRegisters->Get(REG_INDEX_SP) < MEM_STACK_END)
	{
		_incrementRegister(REG_INDEX_SP);
	}

	//DebugPrintHexHex("popstack", regdata, memdata);
	
	return memdata;
}

// -------------------------------------------------------------

uint8_t coreSim::_fetchInstruction()
{
	uint8_t inst;
	
	inst = _fetchMemory( REG_INDEX_PC, FETCH_OP_INCR );
	
	return inst;
}

uint8_t coreSim::_fetchOperand()
{
	uint8_t oper;
	
	oper = _fetchMemory( REG_INDEX_PC, FETCH_OP_INCR );
	
	return oper;
}
	
int coreSim::_executeInstruction(uint8_t inst, uint8_t oper1, uint8_t oper2)
{
	//DebugPrintHex16("_executeInstruction", inst);
	
	switch(inst)
	{
		case OPC_PSH:
			break;
		
		case OPC_PSA:
			break;

		case OPC_POP:
			break;

		case OPC_LDM:
			break;

		case OPC_LDI:
			break;

		case OPC_LDD:
			break;

		case OPC_STM:
			break;

		case OPC_STI:
			break;

		case OPC_STD:
			break;

		case OPC_ADD:
			break;

		case OPC_SUB:
			break;

		case OPC_NEG:
			break;

		case OPC_LSR:
			break;

		case OPC_LSL:
			break;

		case OPC_AND:
			break;
			
		case OPC_ORR:
			break;
			
		case OPC_XOR:
			break;
			
		case OPC_INV:
			break;

		case OPC_CPE:
			break;
			
		case OPC_CNE:
			break;
			
		case OPC_BRZ:
			break;
			
		case OPC_BRN:
			break;
			
		case OPC_BRU:
			break;

		case OPC_INP:
			break;
			
		case OPC_OUT:
			break;
			
		case OPC_SER:
			break;
			
		case OPC_PRT:
			break;

		case OPC_NOP:
			// do nothing
			break;
		
		case OPC_CLS:
			_clearStack();
			break;
			
		case OPC_END:
			_decrementRegister( REG_INDEX_PC );
			break;

		case OPC_RST:
			_resetRegisters();
			break;
	
		default:
			printf("ERROR - attempt to execute invalid instruction!\n");
			break;
	}
	
	return 1;
}

int coreSim::_decodeInstruction(uint8_t inst, uint8_t *oper1, uint8_t *oper2)
{
	uint8_t data1 = 0;
	uint8_t data2 = 0; 
	int instIndex;
	int numops;
	
	//DebugPrintHex16("_decodeInstruction", inst);
	
	if ( IsValidOpCode( inst ) )
	{
		instIndex = SearchOpCode( inst );
		
		numops = GetOpCodeOperands( instIndex );
		
		if (numops == 2)
		{
			data1 = _fetchOperand();
			data2 = _fetchOperand();			
		}
		else if (numops == 1)
		{
			data1 = _fetchOperand();			
		}
	}
	else
	{
		printf("ERROR - invalid instruction decoded!\n");
	}
	
	*oper1 = data1;
	*oper2 = data2;
	
	return 1;
}
	
int coreSim::_doInstructionCycle()
{
	int halted = 0;
	uint8_t instruction;
	uint8_t operand1;
	uint8_t operand2;
	
	if (coreState == STATE_RUN || coreState == STATE_SSTEP)
	{
		instruction = _fetchInstruction();
	
		_decodeInstruction( instruction, &operand1, &operand2 );
		DebugPrintHexHexHex("decoded", instruction, operand1, operand2);
		
		_executeInstruction( instruction, operand1, operand2 );
		DebugPrintHexHexHex("executed", instruction, operand1, operand2);

		if (instruction == OPC_END)
		{
			coreState = STATE_HALT;
			halted = 1;
		}
	}
	
	return halted;
}

// -------------------------------------------------------------














// -------------------------------------------------------------

//
// DEBUG METHODS
//

void coreSim::_debugDumpMemory(const char *header, uint16_t start, int bytes)
{
	uint16_t address, offset;
	uint8_t data;
		
	DebugPrint(header);

	for (offset = 0; offset < bytes; offset++)
	{
		address = start + offset;
		data = pMemSim->Read(address);
		DebugPrintHexHex("memory", address, data);
	}
}

void coreSim::_debugDumpRegisters(const char *header)
{
	int index;
	const char *labels[DEFAULT_REGISTER_SIZE] = { "pc", "sp", "dr", "ac", "tr", "ir", "or", "sr", "pr" };
	
	uint16_t regdata;
	
	DebugPrint(header);
	
	for (index = 0; index < DEFAULT_REGISTER_SIZE; index++)
	{
		regdata = pRegisters->Get(index);
		DebugPrintHex16( labels[index], regdata);
	}
}


// end of file
