#pragma once

#include "Error/ErrorHandling.h"

#define CORE_VECTORWRITER_TYPE(DataStructureTypeName) DataStructureTypeName##Writer
#define CORE_VECTORWRITER_TYPE_FROMELEMENT(ElementTypeName, DataStructureTypeMacroProvider) DataStructureTypeMacroProvider(ElementTypeName)##Writer

#define CORE_VECTORWRITER_DEFINE(DataStructureTypeName, ElementTypeName) \
	typedef struct CORE_VECTORWRITER_TYPE(DataStructureTypeName) \
	{ \
		struct DataStructureTypeName* Core_VectorWriter_UserObject; \
		Core_ReturnCodes (*PushBack)(struct DataStructureTypeName* p_dataStructure, ElementTypeName* p_value); \
		Core_ReturnCodes (*Swap)(struct DataStructureTypeName* p_dataStructure, size_t p_left, size_t p_right); \
		Core_ReturnCodes (*InsertArrayAt)(struct DataStructureTypeName* p_dataStructure, struct DataStructureTypeName* p_insertedDataStructure, size_t p_index); \
	} CORE_VECTORWRITER_TYPE(DataStructureTypeName); \
	\
	void CORE_VECTORWRITER_TYPE(##DataStructureTypeName)_pushBack(struct CORE_VECTORWRITER_TYPE(##DataStructureTypeName)* p_writer, ElementTypeName* p_value);

#define CORE_VECTORWRITER_DEFINE_IMPL(DataStructureTypeName, ElementTypeName) \
	void CORE_VECTORWRITER_TYPE(##DataStructureTypeName)_pushBack(CORE_VECTORWRITER_TYPE(##DataStructureTypeName)* p_writer, ##ElementTypeName* p_value) \
	{ \
		CORE_HANDLE_ERROR_BEGIN(err) \
			err = p_writer->PushBack(p_writer->Core_VectorWriter_UserObject, p_value); \
		CORE_HANDLE_ERROR_END(err); \
	}

#define CORE_VECTORWRITER_BUILD(DataContainerName, in_DataStructure, var_VectorWriter) \
	CORE_VECTORWRITER_TYPE(##DataContainerName)* ##var_VectorWriter =  (CORE_VECTORWRITER_TYPE(##DataContainerName)*) &((Core_Vector*)##in_DataStructure)->Writer;