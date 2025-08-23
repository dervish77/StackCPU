//
// coreSim.h
//
// definition of Core simulator class
//

#ifndef __coreSim_h
#define __coreSim_h

#include <cstdint>

#include "memSim.h"



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

};

#endif // __coreSim_h

