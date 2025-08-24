//
// memSim.h
//
// definition of Memory simulator class
//

#ifndef __memSim_h
#define __memSim_h

#include <cstdint>

#include "memArray.h"


// memory size
#define DEFAULT_MEM_SIZE	4096


// memory address ranges
#define MEM_BOTTOM			0x0000
#define MEM_TOP				0x0FFF

#define MEM_PROG_START		MEM_BOTTOM
#define MEM_PROG_END		0x0BFF

#define MEM_DATA_START		0x0C00
#define MEM_DATA_END		0x0EFF

#define MEM_STACK_START		0x0F00
#define MEM_STACK_END		MEM_TOP


// error codes
#define MEM_NOERROR		0
#define MEM_ERROR		1


class memSim
{
public:

    memSim();                  // ctor
    ~memSim();                 // dtor

    // accessors
	int Write(uint16_t address, uint8_t data);
	uint8_t Read(uint16_t address);
	
	int GetSize();

    // operators
	void ClearMemory();
	void FillMemory(uint8_t data);

private:

    // private data
	memArray *pMemory;

};

#endif // __memSim_h

