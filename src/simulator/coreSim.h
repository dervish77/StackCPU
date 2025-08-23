//
// coreSim.h
//
// definition of Core simulator class
//

#ifndef __coreSim_h
#define __coreSim_h

#include <cstdint>

#include "memSim.h"

#include "regArray.h"


// size of register array
#define DEFAULT_REGISTER_SIZE	8


// register indexes
#define REG_INDEX_PC	0
#define REG_INDEX_SP	1
#define REG_INDEX_DR	2
#define REG_INDEX_AC	3
#define REG_INDEX_IR	4
#define REG_INDEX_OR	5
#define REG_INDEX_SR	6
#define REG_INDEX_PR	7


// state ids




// error codes
#define CORE_NOERROR	0
#define CORE_ERROR		1


class coreSim
{
public:

    coreSim();                  // ctor
    coreSim(memSim *mem);       // ctor
    ~coreSim();                 // dtor

    // accessors
	void SetMemRef(memSim *mem);
	memSim* GetMemRef();


    // operators


private:

    // private data
	memSim *pMemSim;

	regArray *pRegisters;

	int memsize;
};

#endif // __coreSim_h

