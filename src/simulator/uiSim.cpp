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

#include "binUtils.h"
#include "parse.h"

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

	int count;

	int exit = 1;
	
	if (mode != MODE_EXIT) exit = 0;
	
	DebugPrint("uiSim::RunCLI");
	
	// load memory file
	_loadMemFile(name);
	
	// start core sim
	_startCore(MEM_PROG_START, mode);
	
	// start CLI
	while( !exit )
	{
		printf("\nStarting CLI ...\n");
		
		exit = _startCLI(0);
	}
	
	printf("\nExiting sim ...\n\n");
	
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
	
	// dump core state
	#if 0
	_debugDumpMemory("program:", MEM_PROG_START, 18);
	pCore->UnitTest(4);
	#endif
	
	#if 0
	count = DumpMemory(pMem, 0, 127, 0); 
	DebugPrintNumber("dumped", count);
	#endif
	
	#if 0
	pCore->UnitTest(5);
	pCore->UnitTest(6);
	#endif
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
int uiSim::_startCLI(int skip)
{
	int done = 0;
	
	char cmdstr[BUFFER_LEN];
	char *cmdargs[MAX_CMD_ARGS];
	int argcount;
	
	char cmdltr;
	uint16_t address1;
	uint16_t address2;
	uint8_t data;
	int value;
	char *endptr;
	
	if (skip) done = 1;
	
	while (!done)
	{
		// get CLI command
		printf("\n%s", PROMPT);
		if ( fgets( cmdstr, BUFFER_LEN, stdin ) == NULL)
		{
			fprintf(stderr,"fgets error");
			exit(1);
		}

		// parse CLI command string
		argcount = ParseString( cmdstr, cmdargs );
		
		// handle CLI command
		cmdltr = cmdargs[0][0];  // first letter of first arg
		switch(cmdltr)
		{
			case 'l':
				_loadMemFile( cmdargs[1] );
				break;

			case 's':
				_saveMemFile( cmdargs[1] );
				break;

			case 'd':
				_dumpMemFile( cmdargs[1] );
				break;
				
			case 'r':
				address1 = strtol(cmdargs[1], &endptr, 16);
				data = pMem->Read( address1 );
				printf("read %04X is %02X\n", address1, data);
				break;

			case 'w':
				address1 = strtol(cmdargs[1], &endptr, 16);
				data = strtol(cmdargs[2], &endptr, 16);
				pMem->Write( address1, data );
				printf("wrote %04X with %02X\n", address1, data);
				break;

			case 'b':
				address1 = strtol(cmdargs[1], &endptr, 16);
				address2 = strtol(cmdargs[2], &endptr, 16);
				_dumpMemBlock( address1, address2 );
				break;

			case 'f':
				address1 = strtol(cmdargs[1], &endptr, 16);
				address2 = strtol(cmdargs[2], &endptr, 16);
				data = strtol(cmdargs[3], &endptr, 16);
				_fillMemBlock( address1, address2, data );
				break;
				
			case 'g':
				_goCore();
				break;

			case 'h':
				_haltCore();
				break;

			case 'n':
				_stepCore();
				break;

			case 'j':
				address1 = strtol(cmdargs[1], &endptr, 16);
				_startCore( address1, MODE_RUN );
				break;

			case 'k':
				address1 = strtol(cmdargs[1], &endptr, 16);
				_startCore( address1, MODE_SSTEP );
				break;

			case 'x':
				_readReg( cmdargs[1] );
				break;

			case 'y':
				data = strtol(cmdargs[2], &endptr, 16);
				_writeReg( cmdargs[1], data );
				break;

			case 'z':
				_dumpRegs();
				break;

			case 't':
				value = strtol(cmdargs[1], &endptr, 10);
				_setClock( value );
				printf("clock set to %d\n", value);
				break;

			case '?':
				_showCliHelp();
				break;
				
			case 'q':
				printf("goodbye\n");
				done = 1;
				break;
			
			default:
				break;
		}
	}
	
	return(done);
}

void uiSim::_showCliHelp()
{

}

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
	
	printf("... loaded %d bytes\n", start-1);

	//_debugDumpMemory("Memory:", MEM_PROG_START, 18);
}

void uiSim::_saveMemFile(char *name)
{
	uint16_t i;
	uint8_t c;
	int count = 0;
	
	save_file_name = name;
	save_file_p = _openFile(save_file_name, "w");
	
	// save memory into binary file
	
	if (save_file_p != NULL)
	{
		printf("\nSaving memory to %s\n", save_file_name);	
		
		for (i = MEM_PROG_START; i <= MEM_DATA_END; i++)
		{
			c = pMem->Read( i );
			
			fputc( (int) c, save_file_p );
			count++;
		}
		
		_closeFile(save_file_p);
	}

	printf("... saved %d bytes\n", count);
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
		
		count = ConvertDataToHex( pMem, MEM_PROG_START, MEM_DATA_END, dump_file_p, 0, 16);
		
		_closeFile(dump_file_p);		
	}
	
	printf("... dumped %d bytes\n", count);
}

void uiSim::_dumpMemBlock(uint16_t start, uint16_t end)
{
	DumpMemory( pMem, start, end, 0 );
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
	
	pCore->CoreRun();
}
void uiSim::_haltCore()
{
	current_mode = MODE_HALT;
	
	pCore->CoreHalt();
}
void uiSim::_stepCore()
{
	current_mode = MODE_SSTEP;

	pCore->CoreStep();
}

void uiSim::_readReg(char *reg)
{
	uint16_t data;
	
	int index = pCore->SearchRegName( reg );
	
	if (index != -1)
	{
		data = pCore->GetReg( index );
		
		printf("read reg %s is %04X\n", reg, data);
	}
}

void uiSim::_writeReg(char *reg, uint16_t data )
{
	int index = pCore->SearchRegName( reg );
	
	if (index != -1)
	{
		pCore->SetReg( index, data );
		
		printf("write reg %s with %04X\n", reg, data);
	}
}

void uiSim::_dumpRegs()
{
	_debugDumpRegisters( "registers" );
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
	const char *labels[9] = { "pc", "sp", "dr", "ac", "tr", "ir", "or", "sr", "pr" };
	
	uint16_t regdata;
	
	printf("%s\n", header);
	//DebugPrint(header);
	
	for (index = 0; index < DEFAULT_REGISTER_SIZE; index++)
	{
		regdata = pCore->GetReg(index);
		printf("%s %04X\n", labels[index], regdata);
		//DebugPrintHex16( labels[index], regdata );
	}
}

// end of file
