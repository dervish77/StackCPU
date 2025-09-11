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


#define CPU_NAME		"stackcpu"
#define CPU_VERSION		"010"


// register indexes
#define REG_INDEX_PC	0		// full 16 bits - points to next instruction to read
#define REG_INDEX_SP	1		// full 16 bits - points to last value put on stack
#define REG_INDEX_DR	2		// full 16 bits - points to last value accessed in memory
#define REG_INDEX_AC	3		// only lower 8 bits
#define REG_INDEX_TR	4		// only lower 8 bits
#define REG_INDEX_IR	5		// only lower 8 bits
#define REG_INDEX_OR	6		// only lower 8 bits
#define REG_INDEX_SR	7		// only lower 8 bits
#define REG_INDEX_PR	8		// only lower 8 bits

// size of register array
#define DEFAULT_REGISTER_SIZE	9

// register name length
#define REG_NAME_LEN	3

// register struct for reg table
struct Register_s {
	int index;				// reference index (-1 means end of table)
	const char regstr[REG_NAME_LEN];	// "PC"
	int regindex;			// see defines above
	int size;				// size in bytes
};

// typedef for reg table
typedef struct Register_s Register_s_t;


// core state indexes
#define CORE_MODE		1
#define CORE_STATUS		2


// core state mode ids
#define STATE_IDLE		0
#define STATE_HALT		1
#define STATE_RUN		2
#define STATE_SSTEP		3


// display state ids
#define DISPLAY_OFF		0
#define DISPLAY_REGS	1
#define DISPLAY_MEM		2
#define DISPLAY_FULL	3


// fetch operation
#define FETCH_OP_NONE	0
#define FETCH_OP_INCR	1
#define FETCH_OP_DECR	2


// memory DR operation
#define MEM_DR_NONE		0
#define MEM_DR_INCR		1
#define MEM_DR_DECR		2


// compare operation
#define COMP_EQUAL		1
#define COMP_NOTEQUAL	2


// branch operation
#define BRANCH_UNCOND	0
#define BRANCH_ZERO		1
#define BRANCH_NOTZERO	2


// IO direction
#define IO_DIR_IN		1
#define IO_DIR_OUT		2


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
	
	int SetState(int index, int state);
	int GetState(int index);
	
	int SearchRegName(char *str);    // returns index
	int SearchRegIndex(int regid);   // returns index
	int GetRegSize(int index);       // returns size in bytes
	
    // operators
	int CoreRun();
	int CoreStep();
	int CoreHalt();
	
	void CoreReset();
	
	void ClockTick();
	
	// unit test methods
	void UnitTest(int testnum);

private:

    // private data
	memSim *pMemSim;
	regArray *pRegisters;
	int memsize;
	int coreState;
	int displayState;
	
	// private methods
	void _displayStatus();
	
	void _clearRegisters();
	void _fillRegisters(uint16_t data);
	void _resetRegisters();
	void _clearStack();
	
	void _incrementRegister(int index);
	void _decrementRegister(int index);
	
	void _copyRegister(int fromreg, int toreg);

	uint8_t _fetchMemory(int reg, int operation);
	uint8_t _readMemory(uint16_t address);
	void _writeMemory(uint16_t address, uint8_t data);
	
	void _pushStack(uint8_t data);
	uint8_t _popStack();
	
	uint8_t _fetchInstruction();
	uint8_t _fetchOperand();
	
	int _executeInstruction(uint8_t inst, uint8_t oper1, uint8_t oper2);
	int _decodeInstruction(uint8_t inst, uint8_t *oper1, uint8_t *oper2);
	
	int _doInstructionCycle();
	
	void _stackPushDirect(uint8_t oper);
	void _stackPushAcc();
	void _stackPop();
	
	void _memDrLoad(uint8_t oper1, uint8_t oper2, int operation);
	void _memDrStore(uint8_t oper1, uint8_t oper2, int operation);
	
	void _aluAddition();
	void _aluSubtract();
	void _aluNegate();
	void _aluShiftRight();
	void _aluShiftLeft();
	
	void _aluLogicalAnd(uint8_t oper);
	void _aluLogicalOr(uint8_t oper);
	void _aluLogicalXor(uint8_t oper);
	void _aluLogicalInv();
	
	void _compare(uint8_t oper, int operation);
	
	void _branch(uint8_t oper1, uint8_t oper2, int operation);
	
	void _inputOutput(int reg, int direction);
	
	// private debug methods
	void _debugDumpMemory(const char *header, uint16_t start, int bytes);
	void _debugDumpRegisters(const char *header);
	
	void _debugDisplayRegisters(int show_all_regs);
	void _debugDisplayMemory();
};

#endif // __coreSim_h

