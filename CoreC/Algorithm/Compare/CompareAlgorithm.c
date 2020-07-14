#include "CompareAlgorithm.h"

#include <stdlib.h>

#include "Functional/Vector/VectorIterator.h"
#include "Functional/Comparator/Comparator.h"

bool Core_find(struct Core_VectorIterator* p_iterator, struct Core_Comparator* p_comparator)
{
	while (Core_VectorIterator_moveNext(p_iterator))
	{
		if (p_comparator->Function(p_iterator->Current, p_comparator->ComparedObject, p_comparator->UserObject))
		{
			return true;
		}
	}

	return false;
};

bool Core_contains(struct Core_VectorIterator* p_iterator, struct Core_Comparator* p_comparator)
{
	return Core_find(p_iterator, p_comparator);
};