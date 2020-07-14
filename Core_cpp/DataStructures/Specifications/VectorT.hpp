#pragma once

#include "DataStructures/GenericArray.hpp"
#include "Functional/Iterator/IteratorT.hpp"
#include "Functional/Comparator/ComparatorT.hpp"

namespace _Core
{
	template <typename ELEMENT_TYPE>
	struct VectorT
	{
		ELEMENT_TYPE* Memory;
		size_t Size;
		size_t Capacity;
		size_t ElementSize;
	};

	template <typename ELEMENT_TYPE>
	inline void VectorT_alloc(VectorT<ELEMENT_TYPE>* p_array, size_t p_initialCapacity)
	{
		GenericArray_alloc((GenericArray*)p_array, sizeof(ELEMENT_TYPE), p_initialCapacity);
	}

	template <typename ELEMENT_TYPE>
	inline void VectorT_free(VectorT<ELEMENT_TYPE>* p_array)
	{
		GenericArray_free((GenericArray*)p_array);
	}

	template <typename ELEMENT_TYPE>
	inline ELEMENT_TYPE* VectorT_at(VectorT<ELEMENT_TYPE>* p_array, size_t p_index)
	{
		return (ELEMENT_TYPE*)GenericArray_at((GenericArray*)p_array, p_index);
	}

	template <typename ELEMENT_TYPE>
	inline void VectorT_pushBack(VectorT<ELEMENT_TYPE>* p_array, ELEMENT_TYPE* p_element)
	{
		GenericArray_pushBack_realloc((GenericArray*)p_array, p_element);
	}

	template <typename ELEMENT_TYPE>
	inline void VectorT_pushBack(VectorT<ELEMENT_TYPE>* p_array, ELEMENT_TYPE& p_element)
	{
		GenericArray_pushBack_realloc((GenericArray*)p_array, &p_element);
	}

	template <typename ELEMENT_TYPE>
	inline void VectorT_buildIterator(VectorT<ELEMENT_TYPE>* p_array, VectorIteratorT<ELEMENT_TYPE>* out_iterator)
	{
		GenericArray_buildIterator((GenericArray*)p_array, (VectorIterator*)out_iterator);
	}

	template <typename ELEMENT_TYPE>
	inline VectorIteratorT<ELEMENT_TYPE> VectorT_buildIterator(VectorT<ELEMENT_TYPE>* p_array)
	{
		VectorIteratorT<ELEMENT_TYPE> l_it;
		GenericArray_buildIterator((GenericArray*)p_array, (VectorIterator*)&l_it);
		return l_it;
	}

	template <typename ELEMENT_TYPE>
	inline void VectorT_clear(VectorT<ELEMENT_TYPE>* p_array)
	{
		GenericArray_clear((GenericArray*)p_array);
	}

	template <typename ELEMENT_TYPE, typename COMPARED_TYPE, typename USER_TYPE>
	inline void VectorT_eraseCompare(VectorT<ELEMENT_TYPE>* p_array, ComparatorT<ELEMENT_TYPE, COMPARED_TYPE, USER_TYPE>* p_comparator)
	{
		GenericArray_eraseCompare((GenericArray*)p_array, (Comparator*)p_comparator);
	};
}