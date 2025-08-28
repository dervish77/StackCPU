//
// uiSim.h
//
// definition of UI simulator class
//

#ifndef __uiSim_h
#define __uiSim_h

#include "coreSim.h"
#include "memSim.h"


// simulator modes
#define MODE_EXIT       0
#define MODE_HALT       1
#define MODE_RUN        2
#define MODE_SSTEP      3


#define SAVE_NAME_LEN	20


class uiSim
{
public:

    uiSim();                  // ctor
    ~uiSim();                 // dtor

    // accessors


    // operators
	void RunCLI(char *name, int mode);
	

private:

    // private data
    FILE *load_file_p;
	char *load_file_name;

    FILE *save_file_p;
	char *save_file_name;

    FILE *dump_file_p;
	char *dump_file_name;
	
    int current_mode;
	
	int clock_rate;
	
	coreSim *pCore;
	memSim *pMem;
	
	// private accessors
	void _setReg(int reg, uint16_t data);
	uint16_t _getReg(int reg);
	
	void _setMem(uint16_t addr, uint8_t data);
	uint8_t _getMem(uint16_t addr);

	void _setMode(int mode);
	int _getMode();

	void _setClock(int rate);
	
	// private operators
	void _loadMemFile(char *name);
	void _saveMemFile(char *name);
	void _dumpMemFile(char *name);
	
	void _dumpMemBlock(uint16_t start, uint16_t end);
	void _fillMemBlock(uint16_t start, uint16_t end, uint8_t data);
	
	void _startCore(uint16_t pcaddr, int mode);
	
	void _goCore();
	void _haltCore();
	void _stepCore();
	
	// private helpers
	FILE* _openFile(char *name, const char *dir);
	void _closeFile(FILE *file_p);

	// private debug methods
	void _unitTest(int testnum);

	void _debugDumpMemory(const char *header, uint16_t start, int bytes);
	void _debugDumpRegisters(const char *header);
};

#endif // __uiSim_h

