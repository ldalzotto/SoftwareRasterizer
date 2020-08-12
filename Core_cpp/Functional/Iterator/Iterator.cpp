#include "Iterator.hpp"

#include "DataStructures/GenericArray.hpp"

namespace _Core
{
	bool VectorIterator_moveNext(VectorIterator* p_vectorIterator)
	{
		p_vectorIterator->CurrentIndex += 1;
		if (p_vectorIterator->CurrentIndex < p_vectorIterator->Array->Size)
		{
			p_vectorIterator->Current = GenericArray_at_unchecked(p_vectorIterator->Array, p_vectorIterator->CurrentIndex);
			return true;
		}
		else
		{
			return false;
		}
	};

	bool VectorIterator_moveToIndex(VectorIterator* p_vectorIterator, size_t p_index)
	{
		p_vectorIterator->CurrentIndex = p_index;
		p_vectorIterator->Current = GenericArray_at(p_vectorIterator->Array, p_vectorIterator->CurrentIndex);
		return p_vectorIterator->Current != nullptr;
	};

	bool VectorReverseIterator_moveNext(VectorIterator* p_vectorIterator)
	{
		p_vectorIterator->CurrentIndex -= 1;
		if (p_vectorIterator->CurrentIndex + 1 > p_vectorIterator->Array->Size)
		{
			p_vectorIterator->Current = nullptr;
		}
		else
		{
			p_vectorIterator->Current = GenericArray_at(p_vectorIterator->Array, p_vectorIterator->CurrentIndex);
		}

		return p_vectorIterator->Current != nullptr;
	};

	bool VectorRangedIterator_moveNext(VectorRangedIterator* p_vectorIterator)
	{
		p_vectorIterator->CurrentIndex += 1;
		if (p_vectorIterator->CurrentIndex >= p_vectorIterator->BeginInluded && p_vectorIterator->CurrentIndex < p_vectorIterator->EndExcluded)
		{
			p_vectorIterator->Current = GenericArray_at_unchecked(p_vectorIterator->Array, p_vectorIterator->CurrentIndex);
			return true;
		}
		else
		{
			p_vectorIterator->Current = nullptr;
			return false;
		}
	};
}