#pragma once

#include "CompareComparatorsMacros.h"

#define CORE_COMPARE_ALGORITHM_FIND_METHOD_NAME(DataContainerName, ComparedObjectTypeName, UserObjectTypeName) \
	DataContainerName##_##ComparedObjectTypeName##_##UserObjectTypeName##_find

#define CORE_COMPARE_ALGORITHM_FIND_DEFINE(DataContainerName, ElementTypeName, ComparedObjectTypeName, UserObjectTypeName) \
	CORE_ALGORITHM_COMPARE_COMPARATOR(DataContainerName, ElementTypeName, ComparedObjectTypeName, UserObjectTypeName) \
	__forceinline ElementTypeName* CORE_COMPARE_ALGORITHM_FIND_METHOD_NAME(##DataContainerName, ##ComparedObjectTypeName, ##UserObjectTypeName)(struct CORE_VECTORITERATOR_TYPE(##DataContainerName)* p_iterator, struct CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_acccessor, CORE_ALGORITHM_COMPARE_COMPARATOR_TYPE(##DataContainerName, ##ComparedObjectTypeName, ##UserObjectTypeName) p_comparator, ComparedObjectTypeName* p_other, UserObjectTypeName* p_userObject);

#define CORE_COMPARE_ALGORITHM_FIND_DEFINE_IMPL(DataContainerName, ElementTypeName, ComparedObjectTypeName, UserObjectTypeName) \
	__forceinline ElementTypeName* CORE_COMPARE_ALGORITHM_FIND_METHOD_NAME(##DataContainerName, ##ComparedObjectTypeName, ##UserObjectTypeName)(CORE_VECTORITERATOR_TYPE(##DataContainerName)* p_iterator, CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_acccessor, CORE_ALGORITHM_COMPARE_COMPARATOR_TYPE(##DataContainerName, ##ComparedObjectTypeName, ##UserObjectTypeName) p_comparator, ComparedObjectTypeName* p_other, UserObjectTypeName* p_userObject) \
	{ \
		return (ElementTypeName*) Core_find((Core_VectorIterator*)p_iterator, (Core_VectorAccessor*)p_acccessor, (Core_CompareElementComparatorWithUserObject)p_comparator, p_other, p_userObject); \
	};
	
//DataContainerName, ComparedObjectTypeName, UserObjectTypeName