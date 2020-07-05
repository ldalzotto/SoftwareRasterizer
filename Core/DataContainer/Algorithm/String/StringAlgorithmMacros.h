#pragma once

#define CORE_ALGORITHM_METHOD_NAME(DataContainerName, MethodName) DataContainerName##_##MethodName

// Type safety

#define CORE_STRING_ALGORITHM_DEFINE(DataContainerName) \
	__forceinline void CORE_ALGORITHM_METHOD_NAME(DataContainerName, append)(struct DataContainerName* p_string, struct CORE_VECTORWRITER_TYPE(##DataContainerName)* p_writer, char* p_appended); \
	__forceinline bool CORE_ALGORITHM_METHOD_NAME(DataContainerName, find)(struct DataContainerName* p_string, struct CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, char* p_comparedStr, size_t p_startIndex, size_t* p_outfoundIndex);

#define CORE_STRING_ALGORITHM_DEFINE_IMPL(DataContainerName) \
	__forceinline void CORE_ALGORITHM_METHOD_NAME(DataContainerName, append)(DataContainerName* p_string, CORE_VECTORWRITER_TYPE(##DataContainerName)* p_writer, char* p_appended) \
	{ \
		Core_string_append((Core_GenericArray*)p_string, (Core_VectorWriter*)p_writer, p_appended); \
	}; \
	\
	__forceinline bool CORE_ALGORITHM_METHOD_NAME(DataContainerName, find)(DataContainerName* p_string, CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, char* p_comparedStr, size_t p_startIndex, size_t* p_outfoundIndex) \
	{ \
		return Core_string_find((Core_GenericArray*) p_string, (Core_VectorAccessor*)p_accessor, p_comparedStr, p_startIndex, p_outfoundIndex); \
	};


// Syntax sugar

#define CORE_STRING_APPEND(DataContainerName, in_string, in_appendedCharStr) CORE_ALGORITHM_METHOD_NAME(##DataContainerName, append)(##in_string, &(##in_string)->Writer, ##in_appendedCharStr);


#define CORE_STRING_FIND_BEGIN(DataContainerName, in_string, in_compareCharStr, in_startIndex, var_foundIndex) \
	size_t var_foundIndex; \
	{ \
		if(CORE_ALGORITHM_METHOD_NAME(##DataContainerName, find)(##in_string, &((##in_string)->Accessor), ##in_compareCharStr, ##in_startIndex, &(##var_foundIndex))) \
		{

#define CORE_STRING_FIND_ELSE() \
		} else { \

#define CORE_STRING_FIND_END() \
		} \
	}
