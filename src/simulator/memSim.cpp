//
// memSim.cpp
//
// implementation of Memory simulator class
//

#include <iostream>
#include <cstdio>
#include <cstdint>

#include "memSim.h"
#include "memArray.h"

#include "debug.h"


// default constructor
memSim::memSim()
{
	pMemory = new memArray(DEFAULT_MEM_SIZE);
}

// destructor
memSim::~memSim()
{
	delete pMemory;
}


// accessor - write data to address
int memSim::Write(uint16_t address, uint8_t data)
{
	if (pMemory->Set( (int) address, data ))
	{
		return MEM_ERROR;
	}
	else
	{
		//DebugPrintHexHex("memSim::Write", address, data);
		return MEM_NOERROR;
	}
}
	
// accessor - read data from address
uint8_t memSim::Read(uint16_t address)
{
	uint8_t data;
	
	data = pMemory->Get( (int) address );
			
	//DebugPrintHexHex("memSim::Read", address, data);

	return data;
}

// accessor - get size of memory
int memSim::GetSize()
{
	return pMemory->GetSize();
}


// operator - clear the memory
void memSim::ClearMemory()
{
	int size;
	uint16_t address;
	
	size = pMemory->GetSize();
	
	address = 0;
	
	//DebugPrint("memSim::ClearMemory");
	
	while (address < size)
	{
		pMemory->Set( address, 0 );
		address++;
	}
}

// operator - fill the memory
void memSim::FillMemory(uint8_t data)
{
	int size;
	uint16_t address;
	
	size = pMemory->GetSize();
	
	address = 0;
		
	//DebugPrintHex("memSim::FillMemory with", data);

	while (address < size)
	{
		pMemory->Set( address, data );
		address++;
	}
}
	

//
// PRIVATE METHODS
//


// end of file
