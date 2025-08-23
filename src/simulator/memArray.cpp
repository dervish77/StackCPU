//
// memArray.cpp
//
// implementation of Memory Array class
//

#include <iostream>
#include <cstdio>
#include <cstdint>

using namespace std;

#include "memArray.h"


// default constructor
memArray::memArray()
{
	array = new uint8_t[DEFAULT_SIZE];
}

// constructor
memArray::memArray(int size)
{
	if (size < MAX_SIZE)
	{
		array_size = size;
	
		array = new uint8_t[size];
		if (array == NULL)
		{
			cerr << "Error: array allocate failed!" << endl;
		}
	}
	else
	{
		cerr << "Error: size too large!" << endl;
	}
}

// destructor
memArray::~memArray()
{
	delete array;
}


// accessor - set data value at index
int memArray::Set(int index, uint8_t data)
{
	int code = 1;
	
	if (index < array_size)
	{
		array[index] = data;
		code = 0;
	}

	return code;
}

// accessor - get data value from index
uint8_t memArray::Get(int index)
{
	uint8_t data = 0;
	
	if (index < array_size)
	{
		data = array[index];
	}
	else
	{
		cerr << "Error: index out of bounds!" << endl;
	}
	
	return data;
}

// operator - set size of memory array
int memArray::SetSize(int size)
{
	int code = 1;
	
	if (size < MAX_SIZE)
	{
		array_size = size;
	
		delete array;
		array = new uint8_t[size];
		if (array != NULL)
		{
			code = 0;
		}
	}
	
	return code;
}


// operator - get size of memory array
int memArray::GetSize()
{
	return array_size;
}


//
// PRIVATE METHODS
//


// end of file
