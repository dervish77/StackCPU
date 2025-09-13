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
void uiSim::RunCLI(char *name, int mode, int unit)
{
	int exit = 1;
	
	if (mode != MODE_EXIT) exit = 0;
	
	// load memory file
	_loadMemFile(name);
	
	// start core sim
	_startCore(MEM_PROG_START, mode, unit);
	
	// start CLI
	while( !exit )
	{
		printf("\nStarting CLI ...\n");
		
		if (unit)
			exit = _startCLI(1);
		else
			exit = _startCLI(0);
	}
	
	if (mode == 99 && unit == 1)
	{
		_unitTest(1);
		_unitTest(2);
		_unitTest(3);
		_unitTest(4);
	}
	
	printf("\nExiting sim ...\n\n");
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
	printf("\nClock set to %d\n", clock_rate);
}
	
// private operators
int uiSim::_startCLI(int skip)
{
	int done = 0;
	
	char cmdstr[CMD_BUFFER_LEN];
	char *cmdargs[MAX_CMD_ARGS];
	int argcount;
	
	char cmdltr;
	uint16_t address1;
	uint16_t address2;
	uint8_t data;
	int value;
	char *endptr;
	
	while (!done)
	{
		// get CLI command
		printf("\n%s", CMD_PROMPT);
		if (skip) // we're running unit tests so let's bail out
		{
			printf("e\n");
			cmdltr = 'e';
		}
		else
		{	
			if ( fgets( cmdstr, CMD_BUFFER_LEN, stdin ) == NULL)
			{
				fprintf(stderr,"CLI: fgets error");
				exit(1);
			}

			// parse CLI command string
			argcount = ParseString( cmdstr, cmdargs );
		
			// handle CLI command
			cmdltr = cmdargs[0][0];  // first letter of first arg
		}
		
		switch(cmdltr)
		{
			case 'l':
				if (argcount == 2)
					_loadMemFile( cmdargs[1] );
				else	
					printf("CLI: syntax error\n");
				break;

			case 's':
				_saveMemFile( cmdargs[1] );
				break;

			case 'd':
				_dumpMemFile( cmdargs[1] );
				break;
				
			case 'r':
				address1 = strtol(cmdargs[1], &endptr, 16);
				data = _getMem( address1 );
				printf("\nRead %04X is %02X\n", address1, data);
				break;

			case 'w':
				address1 = strtol(cmdargs[1], &endptr, 16);
				data = strtol(cmdargs[2], &endptr, 16);
				_setMem( address1, data );
				printf("\nWrote %04X with %02X\n", address1, data);
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
				_resetCore();
				_startCore( address1, MODE_RUN, 0 );
				break;

			case 'k':
				address1 = strtol(cmdargs[1], &endptr, 16);
				_resetCore();
				_startCore( address1, MODE_SSTEP, 0 );
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

			case 'c':
				_resetCore();
				break;

			case 'i':
				data = strtol(cmdargs[1], &endptr, 16);
				_setInputReg( data );
				break;

			case 'o':
				_displayOutputReg();
				break;

			case 'm':
				_displayMode();
				break;

			case 'p':
				_displaySerialOutput();
				break;

			case 'q':
				_displaySerialInput();
				break;
				
			case 't':
				value = strtol(cmdargs[1], &endptr, 10);
				_setClock( value );
				break;

			case 'v':
				// display versions
				break;
				
			case '?':
				_showCliHelp();
				break;
				
			case 'e':
				done = 1;
				break;
			
			default:
				fprintf(stderr,"CLI: unknown command\n");
				break;
		}
	}
	
	return(done);
}

void uiSim::_showCliHelp()
{
	printf("Commands:\n");
	printf("l filename             - load binary 'file' into memory simulator\n");
	printf("s filename             - save memory simulator to binary 'file'\n");
	printf("d filename             - dump memory to 'hex file'\n");
	printf("\n");
	printf("r hhhh                 - read memory at addess hhhh\n");
	printf("w hhhh dd              - write dd to memory at address hhhh\n");
	printf("b ssss eeee            - dump memory block from ssss to eeee\n");
	printf("f ssss eeee dd         - fill memory block from ssss to eeee with dd\n");
	printf("\n");
	printf("g                      - go - i.e. enter 'run' mode\n");
	printf("h                      - halt - i.e. enter 'halt' mode\n");
	printf("j hhhh                 - reset core, jump to address hhhh and begin 'run' mode\n");
	printf("k hhhh                 - reset core, jump to address hhhh and begin 'single step' mode\n");
	printf("n                      - single step to next instruction\n");
	printf("\n");
	printf("x cc                   - read register 'cc'\n");
	printf("y cc dddd              - write dddd to register 'cc'\n");
	printf("z                      - dump contents of all registers\n");
	printf("\n");
	printf("c                      - reset core\n");
	printf("m                      - get current mode\n");
	printf("i dd                   - set IR register to dd\n");
	printf("o                      - display OR register\n");
	printf("p                      - display contents of PR serial file\n");
	printf("q                      - display contents of SR serial file\n");
	printf("t rate                 - set clock tick to rate\n");
	printf("v                      - display versions\n");
	printf("\n");
	printf("?                      - display CLI help\n");
	printf("\n");
	printf("e                      - exit the simulator\n");
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
		
		printf("... loaded %d bytes\n", start-1);
	}
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
		
		// use 0 as start address for hex file / use 16 bytes per record 
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
	
	printf("\nFilling memory from %04X to %04X with %02X\n", start, end, data);
	
	for (i = start; i <= end; i++)
	{
		pMem->Write( i, data );
	}
}
	
void uiSim::_startCore(uint16_t pcaddr, int mode, int unit)
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
			if (!unit) exit(0);
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
	printf("\nSim is HALTED\n");
	pCore->CoreHalt();
}
void uiSim::_stepCore()
{
	current_mode = MODE_SSTEP;

	pCore->CoreStep();
}

void uiSim::_resetCore()
{
	printf("\nCore reset\n");
	pCore->CoreReset();
}	

void uiSim::_displayMode()
{
	int mode = _getMode(); // TODO consider getting mode from coreSim
	
	switch(mode)
	{
		case MODE_EXIT:
			printf("\nMode is EXIT\n");
			break;
		case MODE_HALT:
			printf("\nMode is HALTED\n");
			break;
		case MODE_RUN:
			printf("\nMode is RUN\n");
			break;
		case MODE_SSTEP:
			printf("\nMode is SSTEP\n");
			break;
		default:
			break;
	}
}

void uiSim::_readReg(char *reg)
{
	uint16_t data;
	
	int index = pCore->SearchRegName( reg );
	
	if (index != -1)
	{
		data = pCore->GetReg( index );
		
		printf("\nRead reg %s is %04X\n", reg, data);
	}
}

void uiSim::_writeReg(char *reg, uint16_t data )
{
	int index = pCore->SearchRegName( reg );
	
	if (index != -1)
	{
		pCore->SetReg( index, data );
		
		printf("\nWrite reg %s with %04X\n", reg, data);
	}
}

void uiSim::_dumpRegs()
{
	printf("\n");
	_debugDumpRegisters( "Registers" );
}

void uiSim::_setInputReg( uint8_t data )
{
	printf("\nInput IR set to %04X\n", data);
	pCore->SetReg(REG_INDEX_IR, data);
}

void uiSim::_displayOutputReg()
{
	printf("\nOutput OR is %04X\n", pCore->GetReg(REG_INDEX_OR));
}

void uiSim::_displaySerialOutput()
{
	printf("\nSerial output:\n");
}

void uiSim::_displaySerialInput()
{
	printf("\nSerial input:\n");
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
			fprintf(stderr, "Couldn't open %s for %s\n", name, dir);
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
	else
	{
		fprintf(stderr, "Specified file ptr is null\n");		
	}
}

// private debug
void uiSim::_unitTest(int testnum)
{
	int size;
	uint8_t data;

	printf("uiSim::_unitTest test %d\n", testnum);

	switch (testnum)
	{
		case 1:
			pCore->UnitTest(1);
			pCore->UnitTest(2);
			pCore->UnitTest(3);
			pCore->UnitTest(4);
			pCore->UnitTest(5);
			pCore->UnitTest(6);
			break;
		case 2:
			size = pMem->GetSize();
			DebugPrintNumber("mem size", size);
			pMem->ClearMemory();
			data = pMem->Read( MEM_DATA_START );
			DebugPrintHex("mem read", data);
			pMem->FillMemory( 0xFF );
			data = pMem->Read( MEM_DATA_START );
			DebugPrintHex("mem read", data);
			break;
		case 3:
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
		case 4:
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
	
	for (index = 0; index < DEFAULT_REGISTER_SIZE; index++)
	{
		regdata = pCore->GetReg(index);
		printf("%s %04X\n", labels[index], regdata);
	}
}

// end of file
