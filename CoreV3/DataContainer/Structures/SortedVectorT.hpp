#pragma message(__FILE__)

namespace _CoreV3
{
	template <class T>
	struct SortedVectorT
	{
		T* Memory;
		size_t Size;
		size_t Capacity;
		size_t ElementSize;

		SortFunctionT<T, T, void> Function;
	};

	template <class T>
	__forceinline void Alloc(SortedVectorT<T>* p_container, size_t p_initialCapacity, SortFunctionT<T, T, void> p_sortFunction)
	{
		SortedVector_alloc((SortedVector*)p_container, sizeof(T), p_initialCapacity, (SortFunction)p_sortFunction);
	};

	template <class T>
	__forceinline void Free(SortedVectorT<T>* p_container)
	{
		GenericArray_free(Convert(p_container));
	};

	template <class T>
	__forceinline T* At(SortedVectorT<T>* p_container, size_t p_index)
	{
		return (T*) GenericArray_at(Convert(p_container), p_index);
	};

	template <class T>
	__forceinline T* Clear(SortedVectorT<T>* p_container)
	{
		GenericArray_clear(Convert(p_container));
	};

	template <class T>
	__forceinline void PushBack(SortedVectorT<T>* p_container, T* p_value)
	{
		SortedVector_pushBack(Convert(p_container), p_value);
	};

	template <class T>
	__forceinline void PushBack(SortedVectorT<T>* p_container, T&& p_value)
	{
		PushBack(p_container, &p_value);
	};

}