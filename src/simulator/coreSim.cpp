//
// coreSim.h
//
// implementation of Core simulator class
//

#include <iostream>
#include <cstdio>
#include <cstdint>

#include "coreSim.h"
#include "memSim.h"

#include "regArray.h"

#include "debug.h"



// default constructor
coreSim::coreSim()
{
	pMemSim = NULL;
	
	pRegisters = new regArray(DEFAULT_REGISTER_SIZE);
	
	memsize = 0;
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
}

// destructor
coreSim::~coreSim()
{
	delete pRegisters;
}


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
void coreSim::SetState(int index, int state)
{
	UNUSED(index);
	UNUSED(state);
}
	
// accessor - get memSim reference
int coreSim::GetState(int index)
{
	UNUSED(index);
	return 0;
}
	

// operator - tick the clock
void coreSim::ClockTick()
{
	
}


//
// unit test code
//
void coreSim::UnitTest(int testnum)
{
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
			
		default:
			break;
	}
}


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

void coreSim::_copyRegister(int fromreg, int toreg)
{
	uint16_t regdata;
	regdata = pRegisters->Get(fromreg);
	pRegisters->Set(toreg, regdata);
}


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
	const char *labels[8] = { "pc", "sp", "dr", "ac", "ir", "or", "sr", "pr" };
	
	uint16_t regdata;
	
	DebugPrint(header);
	
	for (index = 0; index < DEFAULT_REGISTER_SIZE; index++)
	{
		regdata = pRegisters->Get(index);
		DebugPrintHex16( labels[index], regdata);
	}
}


// end of file
