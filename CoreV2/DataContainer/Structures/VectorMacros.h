#pragma message (__FILE__)

#define CORE_VECTOR_TYPE(ElementTypeName) Core_##ElementTypeName##_Vector

#define CORE_DEFINE_VECTOR(ElementTypeName) \
typedef struct CORE_VECTOR_TYPE(ElementTypeName) \
{ \
	ElementTypeName* Memory; \
	size_t Size; \
	size_t Capacity; \
	size_t ElementSize; \
	\
	CORE_VECTORACCESSOR_TYPE_FROMELEMENT(ElementTypeName, CORE_VECTOR_TYPE) Accessor; \
	CORE_VECTORWRITER_TYPE_FROMELEMENT(ElementTypeName, CORE_VECTOR_TYPE) Writer; \
	void (*BuildIterator)(struct CORE_VECTOR_TYPE(ElementTypeName)* p_vector, struct CORE_VECTORITERATOR_TYPE_FROMELEMENT(ElementTypeName, CORE_VECTOR_TYPE)* out_iterator); \
} CORE_VECTOR_TYPE(ElementTypeName); \
\
__forceinline void CORE_VECTOR_TYPE(ElementTypeName)##_alloc(CORE_VECTOR_TYPE(ElementTypeName)* p_vector, size_t p_initialCapacity); \
__forceinline void CORE_VECTOR_TYPE(ElementTypeName)##_free(CORE_VECTOR_TYPE(ElementTypeName)* p_vector);


#define CORE_DEFINE_VECTOR_IMPL(ElementTypeName) \
__forceinline void CORE_VECTOR_TYPE(ElementTypeName)##_alloc(CORE_VECTOR_TYPE(ElementTypeName)* p_vector, size_t p_initialCapacity) \
{ \
	Core_Vector_alloc((Core_Vector*)p_vector, sizeof(ElementTypeName), p_initialCapacity); \
}; \
__forceinline void CORE_VECTOR_TYPE(ElementTypeName)##_free(CORE_VECTOR_TYPE(ElementTypeName)* p_vector) \
{ \
	Core_Vector_free((Core_Vector*)p_vector); \
};

#define CORE_VECTOR_ALLOC(ElementTypeName, in_initialCapacity, var_createdDataStructure) \
	CORE_VECTOR_TYPE(ElementTypeName) var_createdDataStructure; \
	CORE_VECTOR_TYPE(ElementTypeName)##_alloc(&var_createdDataStructure, in_initialCapacity);

#define CORE_VECTOR_FREE(ElementTypeName, in_dataStructure) CORE_VECTOR_TYPE(ElementTypeName)##_free(in_dataStructure);