#include "ElementSorter.h"

#include "Constants.h"

#define INT_SORT_COMPARE(Left, Right) \
	if  (*Left > *Right) { return 1; } \
	else if (*Left < *Right ) { return -1; } \
	else { return 0; }

#define NUMERIC_SORT_COMPARE(Left, Right) \
	if  ((*Left - *Right) >= FLOAT_TOLERANCE) { return 1; } \
	else if ((*Left -*Right) <= FLOAT_TOLERANCE ) { return -1; } \
	else { return 0; }

short int Core_sortCompare_sizet_sizet(size_t * p_left, size_t * p_right)
{
	INT_SORT_COMPARE(p_left, p_right);
};

short int Core_sortCompare_float_float(float* p_left, float* p_right)
{
	NUMERIC_SORT_COMPARE(p_left, p_right);
};

short int Core_sortCompare_uint16_uint16(unsigned __int16* p_left, unsigned __int16* p_right)
{
	NUMERIC_SORT_COMPARE(p_left, p_right);
};