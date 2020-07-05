#include "StringAlgorithm.h"

#include "DataContainer/Types/CoreString.h"
#include "DataContainer/Structures/GenericArray.h"
#include "DataContainer/FunctionalObjets/VectorWriter.h"
#include "DataContainer/FunctionalObjets/VectorIterator.h"
#include "DataContainer/FunctionalObjets/VectorAccessor.h"
#include <string.h>

void Core_string_append(struct Core_GenericArray* p_string, struct Core_VectorWriter* p_writer, char* p_appended)
{
	Core_char_Array l_appededArray;
	Core_char_Array_fromRawArray(&l_appededArray, p_appended, strlen(p_appended));

	CORE_HANDLE_ERROR_BEGIN(err)
		if (p_string->Size >= 2)
		{
			err = p_writer->InsertArrayAt(p_string, &l_appededArray, CORE_STRING_CHAR_NB(p_string));
		}
		else
		{
			err = p_writer->InsertArrayAt(p_string, &l_appededArray, 0);
		}
	CORE_HANDLE_ERROR_END(err);
};

bool Core_string_find(struct Core_GenericArray* p_string, struct Core_VectorAccessor* p_accessor, char* p_comparedStr, size_t p_startIndex, size_t* p_outfoundIndex)
{
	Core_char_Array l_comparedStr;
	Core_char_Array_fromRawArray(&l_comparedStr, p_comparedStr, strlen(p_comparedStr));

	if (p_string->Size > 0)
	{
		for (*p_outfoundIndex = p_startIndex; *p_outfoundIndex < p_string->Size; (*p_outfoundIndex)++)
		{
			size_t l_endIndex = (*p_outfoundIndex) + l_comparedStr.Size;
			if (l_endIndex > CORE_STRING_CHAR_NB(p_string))
			{
				break;
			}

			char* l_pstringCompareBegin = (char*)p_accessor->At(p_string, *p_outfoundIndex);
			char* l_compareStringBegin = p_comparedStr;
			if (*l_pstringCompareBegin == *l_compareStringBegin)
			{
				if (memcmp(l_pstringCompareBegin, l_compareStringBegin, sizeof(char) * l_comparedStr.Size) == 0)
				{
					return true;
				}
			}
		}
	}

	return false;
};