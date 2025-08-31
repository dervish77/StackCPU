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

#include "hex.h"

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



// operator - run CLI interface
void uiSim::RunCLI(char *name, int mode)
{
	char savefilename[SAVE_NAME_LEN];
	char dumpfilename[DUMP_NAME_LEN];

	int exit = 0;
	
	DebugPrint("uiSim::RunCLI");
	
	// load memory file
	_loadMemFile(name);
	
	// start core sim
	_startCore(MEM_PROG_START, mode);
	
	// start CLI
	#if 0
	while( !exit )
	{
		if (mode == MODE_EXIT || mode == MODE_HALT) 
			exit = 1;
	}
	#endif
	
	// save memory file
	#if 0
	strcpy(savefilename, "save.bin");
	_saveMemFile(savefilename);
	#endif
	
	// dump memory file
	#if 0
	strcpy(dumpfilename, "dump.hex");
	_dumpMemFile(dumpfilename);
	#endif
	
	pCore->UnitTest(4);
}

//
// PRIVATE METHODS
//

// private accessors
void uiSim::_setReg(int reg, uint16_t data)
{
	pCore->SetReg(reg, data);
}

uint16_t uiSim::_getReg(int reg)
{
	uint16_t data;
	data = pCore->GetReg(reg);
	return data;
}
	
void uiSim::_setMem(uint16_t addr, uint8_t data)
{
	pMem->Write(addr, data);
}

uint8_t uiSim::_getMem(uint16_t addr)
{
	uint8_t data;
	data = pMem->Read(addr);
	return data;
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
	char c;
	uint16_t start = MEM_PROG_START;
	
	load_file_name = name;
	load_file_p = _openFile(load_file_name, "r");
	
	// load binary file into memory
	
	if (load_file_p != NULL)
	{
		printf("\nLoading %s into memory\n", load_file_name);
		
		while ( !feof(load_file_p) )
		{
			c = getc(load_file_p);
			
			pMem->Write( start, (uint8_t) c );
			start++;
		}
		
		_closeFile(load_file_p);
	}
	
	DebugPrintNumber("loaded bytes", start-1);
	_debugDumpMemory("Memory:", MEM_PROG_START, 18);
}

void uiSim::_saveMemFile(char *name)
{
	uint16_t i;
	uint8_t c;
	
	save_file_name = name;
	save_file_p = _openFile(save_file_name, "w");
	
	// save memory into binary file
	
	if (save_file_p != NULL)
	{
		printf("\nSaving memory to %s\n", save_file_name);	
		
		for (i = MEM_PROG_START; i < MEM_DATA_END; i++)
		{
			c = pMem->Read( i );
			
			fputc( (int) c, save_file_p );
		}
		
		_closeFile(save_file_p);
	}
	
}

void uiSim::_dumpMemFile(char *name)
{
	int count = 0;
	dump_file_name = name;
	dump_file_p = _openFile(dump_file_name, "w");
	
	// save memory into intel hex file
	
	if (dump_file_p != NULL)
	{
		printf("\nDumping memory to %s\n", dump_file_name);	
		
		count = ConvertData( pMem, dump_file_p, 0, 16);
	}
	
	printf("... dumped %d bytes\n", count);
	
	fclose(dump_file_p);
}

void uiSim::_dumpMemBlock(uint16_t start, uint16_t end)
{
	
}
	
void uiSim::_fillMemBlock(uint16_t start, uint16_t end, uint8_t data)
{
	uint16_t i;
	
	DebugPrintHexHex("Filling memory", start, end);
	DebugPrintHex("....with", data);
	
	for (i = start; i < end; i++)
	{
		pMem->Write( i, data );
	}
}
	
void uiSim::_startCore(uint16_t pcaddr, int mode)
{
	_setMode(mode);
	
	pCore->SetReg(REG_INDEX_PC, pcaddr);
	
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
	current_mode = MODE_RUN;
}
void uiSim::_haltCore()
{
	current_mode = MODE_HALT;
}
void uiSim::_stepCore()
{
	current_mode = MODE_SSTEP;
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

void uiSim::_closeFile(FILE *file_p)
{
	if (file_p != NULL)
	{
		fclose(file_p);
	}
}

// private debug
void uiSim::_unitTest(int testnum)
{
	int size;
	uint8_t data;

	switch (testnum)
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
			fprintf(stderr, "Unknown testnum %d specified\n", testnum);
			break;
	}
}

void uiSim::_debugDumpMemory(const char *header, uint16_t start, int bytes)
{
	uint16_t address, offset;
	uint8_t data;
		
	DebugPrint(header);

	for (offset = 0; offset < bytes; offset++)
	{
		address = start + offset;
		data = pMem->Read(address);
		DebugPrintHexHex("memory", address, data);
	}
}

void uiSim::_debugDumpRegisters(const char *header)
{
	int index;
	const char *labels[8] = { "pc", "sp", "dr", "ac", "ir", "or", "sr", "pr" };
	
	uint16_t regdata;
	
	DebugPrint(header);
	
	for (index = 0; index < DEFAULT_REGISTER_SIZE; index++)
	{
		regdata = pCore->GetReg(index);
		DebugPrintHex16( labels[index], regdata);
	}
}

// end of file
