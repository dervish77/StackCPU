//
// memSim.h
//
// definition of Memory simulator class
//

#ifndef __memSim_h
#define __memSim_h

#include <cstdint>

#include "memArray.h"

#define DEFAULT_MEM_SIZE	4096

#define ERROR		1
#define NOERROR		0


class memSim
{
public:

    memSim();                  // ctor
    ~memSim();                 // dtor

    // accessors
	int Write(uint16_t address, uint8_t data);
	uint8_t Read(uint16_t address);

    // operators
	void ClearMemory();
	void FillMemory(uint8_t data);

private:

    // private data
	memArray *pMemory;

};

#endif // __memSim_h

