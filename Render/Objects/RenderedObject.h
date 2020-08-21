#pragma once

#include "RenderedObject_def.h"
#include "DataStructures/ARRAY.h"

inline void Arr_Alloc_RenderedObjectHandle(Array_RenderedObjectHandle_PTR p_array, size_t p_initialCapacity) { Arr_Alloc((Array_PTR)p_array, sizeof(RenderedObject_HANDLE), p_initialCapacity); };
inline void Arr_PushBackRealloc_RenderedObjectHandle(Array_RenderedObjectHandle_PTR p_array, RenderedObject_HANDLE_PTR p_item) { Arr_PushBackRealloc((Array_PTR)p_array, sizeof(RenderedObject_HANDLE), (char*)p_item); };
inline void Arr_Free_RenderedObjectHandle(Array_RenderedObjectHandle_PTR p_array) { Arr_Free((Array_PTR)p_array); };
inline void Arr_Erase_RenderedObjectHandle(Array_RenderedObjectHandle_PTR p_array, size_t p_index) { Arr_Erase((Array_PTR)p_array, sizeof(RenderedObject_HANDLE), p_index); };