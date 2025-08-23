//
// regArray.h
//
// definition of Register Array class
//

#ifndef __regArray_h
#define __regArray_h

#include <cstdint>

// array sizes
#define DEFAULT_REG_SIZE	16
#define MAX_REG_SIZE 		64



class regArray
{
public:

    regArray();                  // ctor
	regArray(int size);          // ctor
    ~regArray();                 // dtor

    // accessors
	int Set(int index, uint16_t data);
	uint16_t Get(int index);

    // operators
	int SetSize(int size);  // warning, reinitializes the memory array!
	int GetSize();

private:

    // private data
	int array_size;

	uint16_t *array;

};

#endif // __regArray_h
