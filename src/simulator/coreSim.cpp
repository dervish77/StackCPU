//
// coreSim.h
//
// implementation of Core simulator class
//

#include <iostream>
#include <cstdio>
#include <cstdint>

#include "coreSim.h"
#include "memSim.h"

#include "regArray.h"

#include "debug.h"



// default constructor
coreSim::coreSim()
{
	pMemSim = NULL;
	
	pRegisters = new regArray(DEFAULT_REGISTER_SIZE);
	
	memsize = 0;
}

// constructor
coreSim::coreSim(memSim *mem)
{
	pMemSim = NULL;
	memsize = 0;
	
	if (mem != NULL)
	{
		pMemSim = mem;
		memsize = pMemSim->GetSize();
	}
	
	pRegisters = new regArray(DEFAULT_REGISTER_SIZE);
}

// destructor
coreSim::~coreSim()
{
	delete pRegisters;
}


// accessor - set



// operator - tbd


