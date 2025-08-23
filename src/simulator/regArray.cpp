//
// regArray.cpp
//
// implementation of Memory Array class
//

#include <iostream>
#include <cstdio>
#include <cstdint>

using namespace std;

#include "regArray.h"


// default constructor
regArray::regArray()
{
	array = new uint16_t[DEFAULT_REG_SIZE];
}

// constructor
regArray::regArray(int size)
{
	if (size < MAX_REG_SIZE)
	{
		array_size = size;
	
		array = new uint16_t[size];
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
regArray::~regArray()
{
	delete array;
}


// accessor - set data value at index
int regArray::Set(int index, uint16_t data)
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
uint16_t regArray::Get(int index)
{
	uint16_t data = 0;
	
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
int regArray::SetSize(int size)
{
	int code = 1;
	
	if (size < MAX_REG_SIZE)
	{
		array_size = size;
	
		delete array;
		array = new uint16_t[size];
		if (array != NULL)
		{
			code = 0;
		}
	}
	
	return code;
}


// operator - get size of memory array
int regArray::GetSize()
{
	return array_size;
}


//
// PRIVATE METHODS
//


// end of file
