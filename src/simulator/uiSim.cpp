//
// uiSim.cpp
//
// implementation of UI simulator class
//

#include <cstdio>
#include <cstdint>

#include "uiSim.h"
#include "coreSim.h"
#include "memSim.h"

#include "debug.h"


// default constructor
uiSim::uiSim()
{
    mem_file_p = NULL;
	mem_file_name = NULL;
    current_mode = MODE_HALT;
	
	pCore = NULL;
	pMem = new memSim();
	if (pMem != NULL)
	{
		pCore = new coreSim( pMem );
	}
}

// destructor
uiSim::~uiSim()
{
	delete pCore;
	delete pMem;
}


// accessor - tbd



// operator - load memory sim
void uiSim::Load(char *name, FILE *file_p)
{
	DebugPrint("uiSim::Load");
	
	mem_file_name = name;
    mem_file_p = file_p;
	
	
	// unit test of memSim
	{
		int size;
		size = pMem->GetSize();
		DebugPrintNumber("mem size", size);
		uint8_t data;
		pMem->ClearMemory();
		data = pMem->Read( MEM_DATA_START );
		DebugPrintHex("mem read", data);
		pMem->FillMemory( 0xFF );
		data = pMem->Read( MEM_DATA_START );
		DebugPrintHex("mem read", data);
	}

}

// operator - start core sim
void uiSim::Start(int mode)
{
	DebugPrint("uiSim::Start");

    current_mode = mode;


	// unit test of memSim
	{
		uint8_t data;
		data = pMem->Read( 0xAA );
		DebugPrintHex("mem read", data);
		pMem->Write( 0xAA, 0xAA );
		DebugPrintHexHex("mem write", 0xAA, 0xAA);
		data = pMem->Read( 0xAA );
		DebugPrintHex("mem read", data);
		pMem->Write( 0xAA, 0x55 );
		DebugPrintHexHex("mem write", 0xAA, 0x55);
		data = pMem->Read( 0xAA );
		DebugPrintHex("mem read", data);
	}
	
	// unit test of coreSim
	{
		uint16_t data;
		data = pCore->GetReg(REG_INDEX_PC);
		DebugPrintHex("reg pc", data);
		data = pCore->GetReg(REG_INDEX_SP);
		DebugPrintHex("reg sp", data);
		data = pCore->GetReg(REG_INDEX_DR);
		DebugPrintHex("reg dr", data);

		data = pCore->GetReg(REG_INDEX_AC);
		DebugPrintHex("reg ac", data);
		pCore->SetReg( REG_INDEX_AC, 0x55 );
		DebugPrintHex("write ac", data);
		data = pCore->GetReg(REG_INDEX_AC);
		DebugPrintHex("reg ac", data);
	}

}

// operator - run CLI interface
void uiSim::RunCLI()
{

}

//
// PRIVATE METHODS
//




// end of file
