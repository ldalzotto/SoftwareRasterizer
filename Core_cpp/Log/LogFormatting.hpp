#pragma once

#include "DataStructures/Specifications/String.hpp"

namespace _Core
{
	String Log_formatError(char* p_file, int p_line, char* p_message);
	String Log_formatError_string(const char* p_file, int p_line, String* p_message);
}


#define MYLOG_BUILD_ERRORMESSAGE(MESSAGE) ::_Core::Log_formatError(__FILE__, __LINE__, MESSAGE).Memory
#define MYLOG_BUILD_ERRORMESSAGE_STRING(MESSAGE) ::_Core::Log_formatError_string(__FILE__, __LINE__, MESSAGE).Memory