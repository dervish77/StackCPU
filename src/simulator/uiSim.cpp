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
	
}

// destructor
uiSim::~uiSim()
{

}


// accessor - set



// operator - load memory sim
void uiSim::Load(char *name, FILE *file_p)
{
	DebugPrint("uiSim::Load");
	
	mem_file_name = name;
    mem_file_p = file_p;
	
	
	// unit test of memSim
	{
		int size;
		size = Mem.GetSize();
		DebugPrintNumber("mem size", size);
		uint8_t data;
		Mem.ClearMemory();
		data = Mem.Read( 0xAA );
		DebugPrintHex("mem read", data);
		Mem.FillMemory( 0xF0 );
		data = Mem.Read( 0xAA );
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
		Mem.Write( 0xAA, 0xAA );
		DebugPrintHexHex("mem write", 0xAA, 0xAA);
		data = Mem.Read( 0xAA );
		DebugPrintHex("mem read", data);
		Mem.Write( 0xAA, 0x55 );
		DebugPrintHexHex("mem write", 0xAA, 0x55);
		data = Mem.Read( 0xAA );
		DebugPrintHex("mem read", data);
	}
}

