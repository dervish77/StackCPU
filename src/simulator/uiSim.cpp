//
// uiSim.cpp
//
// implementation of UI simulator class
//

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>

#include "uiSim.h"
#include "coreSim.h"
#include "memSim.h"

#include "debug.h"


// default constructor
uiSim::uiSim()
{
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


}

// operator - start core sim
void uiSim::Start(int mode)
{
	DebugPrint("uiSim::Start");


}

// operator - run CLI interface
void uiSim::RunCLI(char *name, int mode)
{
	DebugPrint("uiSim::RunCLI");
	
	// load memory file
	_loadMemFile(name);
	
	// start core sim
	_startCore(mode);
	
	// start CLI

	
}

//
// PRIVATE METHODS
//

// private accessors
void uiSim::_setReg(int reg, uint16_t data)
{
}

void uiSim::_getReg(int reg)
{
}
	
void uiSim::_setMem(uint16_t addr, uint8_t data)
{
}

void uiSim::_getMem(uint16_t addr)
{
}
	
void uiSim::_setMode(int mode)
{
	current_mode = mode;
}

int uiSim::_getMode()
{
	return current_mode;
}

void uiSim::_setClock(int rate)
{
	clock_rate = rate;
}
	
// private operators
void uiSim::_loadMemFile(char *name)
{
	load_file_name = name;
	load_file_p = _openFile(name, "r");
	
	if (load_file_p != NULL)
	{
		printf("\nLoading %s into memory\n", load_file_name);
		
	}
}

void uiSim::_saveMemFile(char *name)
{
	save_file_name = name;
	save_file_p = _openFile(name, "w");
	
	if (save_file_p != NULL)
	{
		printf("\nSaving memory to %s\n", save_file_name);	
		
	}
}

void uiSim::_dumpMemFile(char *name)
{
	dump_file_name = name;
	dump_file_p = _openFile(name, "w");
	
	if (dump_file_p != NULL)
	{
		printf("\nDumping memory to %s\n", dump_file_name);	
		
	}
}
	
void uiSim::_startCore(int mode)
{
	_setMode(mode);
	
	switch (current_mode)
	{
		case MODE_EXIT:
			printf("\nSim is EXITED\n");
			break;
		case MODE_HALT:
			printf("\nSim is HALTED\n");
			break;
		case MODE_RUN:
			printf("\nSim is RUNNING\n");
			break;
		case MODE_SSTEP:
			printf("\nSim is SINGLE STEP\n");
			break;
		default:
			fprintf(stderr, "Unknown mode %d specified\n", mode);
			exit(0);
			break;
	}
}

void uiSim::_goCore()
{
}
void uiSim::_haltCore()
{
}
void uiSim::_stepCore()
{
}

// private helpers
FILE* uiSim::_openFile(char *name, const char *dir)
{
	FILE *file_p = NULL;
	
	if (strlen(name) != 0)
	{
		file_p = fopen(name, dir);
		if (file_p == (FILE *)NULL)
		{
			fprintf(stderr, "Couldn't open %s\n", name);
		}
	}
	else
	{
		fprintf(stderr, "Specified file name is null\n");
	}
	
	return(file_p);
}

// private debug
void uiSim::_unitTest(int test)
{
	int size;
	uint8_t data;

	switch (test)
	{
		case 1:
			size = pMem->GetSize();
			DebugPrintNumber("mem size", size);
			pMem->ClearMemory();
			data = pMem->Read( MEM_DATA_START );
			DebugPrintHex("mem read", data);
			pMem->FillMemory( 0xFF );
			data = pMem->Read( MEM_DATA_START );
			DebugPrintHex("mem read", data);
			break;
		case 2:
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
			break;
		case 3:
			pCore->UnitTest(1);
			pCore->UnitTest(2);
			pCore->UnitTest(3);
			pCore->UnitTest(4);
			break;
		default:
			break;
	}
}


// end of file
