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

#include "debug.h"


// default constructor
coreSim::coreSim()
{
	pMemSim = NULL;
}

// constructor
coreSim::coreSim(memSim *mem)
{
	if (mem != NULL)
	{
		pMemSim = mem;
	}
}

// destructor
coreSim::~coreSim()
{

}


// accessor - set



// operator - tbd


