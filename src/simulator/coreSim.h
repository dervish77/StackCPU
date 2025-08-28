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
#define REG_INDEX_PC	0		// full 16 bits - points to next instruction to read
#define REG_INDEX_SP	1		// full 16 bits - points to last value put on stack
#define REG_INDEX_DR	2		// full 16 bits - points to last value accessed in memory
#define REG_INDEX_AC	3		// only lower 8 bits
#define REG_INDEX_IR	4		// only lower 8 bits
#define REG_INDEX_OR	5		// only lower 8 bits
#define REG_INDEX_SR	6		// only lower 8 bits
#define REG_INDEX_PR	7		// only lower 8 bits


// state ids



// fetch operation
#define FETCH_OP_NONE	0
#define FETCH_OP_INCR	1
#define FETCH_OP_DECR	2


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

	void SetReg(int index, uint16_t data);
	uint16_t GetReg(int index);
	
	void SetState(int index, int state);
	int GetState(int index);
	
    // operators
	void ClockTick();
	
	// unit test methods
	void UnitTest(int testnum);

private:

    // private data
	memSim *pMemSim;
	regArray *pRegisters;
	int memsize;
	
	// private methods
	void _clearRegisters();
	void _fillRegisters(uint16_t data);
	void _resetRegisters();
	
	void _incrementRegister(int index);
	void _decrementRegister(int index);
	
	uint8_t _fetchMemory(int reg, int operation);
	uint8_t _readMemory(uint16_t address);
	void _writeMemory(uint16_t address, uint8_t data);
	
	void _pushStack(uint8_t data);
	uint8_t _popStack();
	
	void _copyRegister(int fromreg, int toreg);
	
	// private debug methods
	void _debugDumpMemory(const char *header, uint16_t start, int bytes);
	void _debugDumpRegisters(const char *header);
};

#endif // __coreSim_h

