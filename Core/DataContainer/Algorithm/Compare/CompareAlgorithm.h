#pragma once

#include "CompareComparators.h"

void* Core_find(struct Core_VectorIterator* p_iterator, Core_CompareElementComparatorWithUserObject p_comparator, void* p_other, void* p_userObject);