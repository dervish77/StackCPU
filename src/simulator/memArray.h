//
// memArray.h
//
// definition of Memory Array class
//

#ifndef __memArray_h
#define __memArray_h

#include <cstdint>

// array sizes
#define DEFAULT_SIZE	256
#define MAX_SIZE 		65536


class memArray
{
public:

    memArray();                  // ctor
	memArray(int size);          // ctor
    ~memArray();                 // dtor

    // accessors
	int Set(int index, uint8_t data);
	uint8_t Get(int index);

    // operators
	int SetSize(int size);  // warning, reinitializes the memory array!
	int GetSize();

private:

    // private data
	int array_size;

	uint8_t *array;

};

#endif // __memArray_h
