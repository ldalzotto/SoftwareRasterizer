#pragma once

extern "C"
{
#include "DataStructures/String_def.h"
}
namespace _Core
{
	void ToString_float(String_PTR p_string, float* p_element);
	void ToString_int(String_PTR p_string, int* p_element);
	void ToString_intv(char p_string[50], int* p_element);
}