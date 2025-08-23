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
	_clearRegisters();
	SetReg(REG_INDEX_PC, MEM_PROG_START);
	SetReg(REG_INDEX_SP, MEM_STACK_END);
	SetReg(REG_INDEX_DR, MEM_DATA_START);
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

void coreSim::_incrememtRegister(int index)
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

	switch(operation)
	{
		case FETCH_OP_INCR:
			_incrememtRegister(reg);
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
	regdata = pRegisters->Get(REG_INDEX_SP);
	
	pMemSim->Write( regdata, data );
	
	_decrementRegister(REG_INDEX_SP);
}

uint8_t coreSim::_popStack()
{
	uint8_t memdata;
	uint16_t regdata;
	regdata = pRegisters->Get(REG_INDEX_SP);
	
	memdata = pMemSim->Read( regdata );
	
	_incrememtRegister(REG_INDEX_SP);
	
	return memdata;
}

void coreSim::_copyRegister(int fromreg, int toreg)
{
	uint16_t regdata;
	regdata = pRegisters->Get(fromreg);
	pRegisters->Set(toreg, regdata);
}

// end of file
