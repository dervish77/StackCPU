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


class uiSim
{
public:

    uiSim();                  // ctor
    ~uiSim();                 // dtor

    // accessors


    // operators
    void Load(char *name, FILE *file_p);
    void Start(int mode);
	
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
	void _getReg(int reg);
	
	void _setMem(uint16_t addr, uint8_t data);
	void _getMem(uint16_t addr);

	void _setMode(int mode);
	int _getMode();

	void _setClock(int rate);
	
	// private operators
	void _loadMemFile(char *name);
	void _saveMemFile(char *name);
	void _dumpMemFile(char *name);
	
	void _startCore(int mode);
	void _goCore();
	void _haltCore();
	void _stepCore();
	
	// private helpers
	FILE* _openFile(char *name, const char *dir);

	// private debug methods
	void _unitTest(int test);

};

#endif // __uiSim_h

