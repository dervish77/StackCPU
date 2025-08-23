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
	
	void RunCLI();
	

private:

    // private data
    FILE *mem_file_p;
	char *mem_file_name;
    int current_mode;
	
	coreSim *pCore;
	memSim *pMem;
	
	// private methods
	

};

#endif // __uiSim_h

