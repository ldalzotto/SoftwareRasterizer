#pragma once

namespace _GameEngine::_Core
{
	typedef struct Vector
	{
		void* Memory;
		size_t Size;
		size_t ElementSize;
		size_t Capacity;
	} Vector;

	typedef bool(*VectorElementComparator)(void* p_element, void* p_userObject, size_t p_vectorElementSize);

	void Vector_alloc(Vector* p_vector, size_t p_initialCapacity, size_t p_elementSize);
	void Vector_free(Vector* p_vector);
	void Vector_resize(Vector* p_vector, size_t p_newCapacity);
	void Vector_pushBack(Vector* p_vector, void* p_value);
	void Vector_erase(Vector* p_vector, size_t p_index);
	void* Vector_at(Vector* p_vector, size_t p_index);
	void* Vector_get(Vector* p_vector, VectorElementComparator p_comparator, void* p_userObject = nullptr);
	size_t Vector_getIndex(Vector* p_vector, VectorElementComparator p_comparator, void* p_userObject = nullptr);

	bool Vector_equalsComparator(void* p_element, void* p_userObject, size_t p_vectorElementSize);
}
