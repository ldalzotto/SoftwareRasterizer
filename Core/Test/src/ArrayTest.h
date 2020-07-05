#pragma once

extern "C" {
	#include "DataContainer/Structures/VectorMacros.h"
	#include "DataContainer/Structures/SortedVectorMacros.h"
	#include "DataContainer/Structures/ArrayMacros.h"
	#include "DataContainer/FunctionalObjets/VectorIteratorMacros.h"
	#include "DataContainer/FunctionalObjets/VectorWriterMacros.h"
	#include "DataContainer/FunctionalObjets/VectorAccessorMacros.h"
	#include "DataContainer/Algorithm/Sort/SortingAlgorithmMacro.h"
	#include "DataContainer/Algorithm/Compare/CompareAlgorithmMacros.h"
	#include "Error/ReturnCodes.h"
}

namespace _Core::_Test
{

	struct Vector3f_Test
	{
		float x, y, z;
	};

	CORE_VECTORACCESSOR_DEFINE(Core_Vector3f_Test_Vector, Vector3f_Test);
	CORE_VECTORWRITER_DEFINE(Core_Vector3f_Test_Vector, Vector3f_Test);
	CORE_VECTORITERATOR_DEFINE(Core_Vector3f_Test_Vector, Vector3f_Test);
	CORE_DEFINE_VECTOR(Vector3f_Test);

	CORE_SORT_ALGORITHM_DEFINE(Core_Vector3f_Test_Vector, Vector3f_Test);

	CORE_COMPARE_ALGORITHM_FIND_DEFINE(Core_Vector3f_Test_Vector, Vector3f_Test, float, void);

	CORE_VECTORACCESSOR_DEFINE(Core_Vector3f_Test_Array, Vector3f_Test);
	CORE_VECTORWRITER_DEFINE(Core_Vector3f_Test_Array, Vector3f_Test);
	CORE_VECTORITERATOR_DEFINE(Core_Vector3f_Test_Array, Vector3f_Test);
	CORE_DEFINE_ARRAY(Vector3f_Test);

	CORE_SORT_ALGORITHM_DEFINE(Core_Vector3f_Test_Array, Vector3f_Test);

	CORE_VECTORACCESSOR_DEFINE(Core_Vector3f_Test_SortedVector, Vector3f_Test);
	CORE_VECTORWRITER_DEFINE(Core_Vector3f_Test_SortedVector, Vector3f_Test);
	CORE_VECTORITERATOR_DEFINE(Core_Vector3f_Test_SortedVector, Vector3f_Test);
	CORE_DEFINE_SORTEDVECTOR(Vector3f_Test);

	CORE_SORT_ALGORITHM_DEFINE(Core_Vector3f_Test_SortedVector, Vector3f_Test);

	void ExecuteTest();
}