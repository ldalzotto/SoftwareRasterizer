#pragma once

#include <stdint.h>

typedef struct WindowSize_TYP
{
	uint32_t Width;
	uint32_t Height;
	uint32_t HalfWidth;
	uint32_t HalfHeight;
} WindowSize, * WindowSize_PTR;