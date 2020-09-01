#pragma once

#include <stdlib.h>

typedef struct Array_TYP
{
	char* Memory;
	size_t Size;
	size_t Capacity;
} Array, *Array_PTR;

#define ARRAY_TYPE_DEFINITION(ArrayElement) \
union \
{ \
	struct { Array array; }; \
	struct \
	{ \
		ArrayElement* Memory; \
		size_t Size; \
		size_t Capacity; \
	}; \
};

typedef struct ArrayIterator_TYP
{
	Array_PTR Array;
	char* Current;
	size_t CurrentIndex;
	size_t ElementOffset;
} ArrayIterator, * ArrayIterator_PTR;

#define ARRAY_ITERATOR_DEFINITION(ArrayType, ArrayElement) \
ArrayType* Array; \
ArrayElement* Current; \
size_t CurrentIndex; \
size_t ElementOffset;
