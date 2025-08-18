//
// uiSim.cpp
//
// implementation of UI simulator class
//

#include <cstdio>

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


}

// operator - start core sim
void uiSim::Start(int mode)
{
	DebugPrint("uiSim::Start");

    current_mode = mode;


}

