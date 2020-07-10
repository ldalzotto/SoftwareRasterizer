#include "MyLog.h"

#include "Clock/Clock.h"

namespace _GameEngine::_Log
{
	void MyLog_build(MyLog* p_myLog, _Clock::Clock* p_clock)
	{
		p_myLog->Clock = p_clock;
		Core_Vector_alloc(&p_myLog->LogMessages, sizeof(LogMessage), 0);
	};

	void MyLog_free(MyLog* p_myLog)
	{
		MyLog_processLogs(p_myLog);
		Core_GenericArray_free(&p_myLog->LogMessages);
	};

	void logMessage_free(LogMessage* p_logMessage)
	{
		//	free(p_logMessage->Message);
	};

	void MyLog_pushLog(MyLog* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, char* p_message)
	{
		LogMessage l_logMessage{};
		l_logMessage.LogLevel = p_logLevel;
		l_logMessage.FilePath = p_filePath;
		l_logMessage.FileLine = p_line;

		{
			strcpy((char*)l_logMessage.Message, p_message);
		}

		{
			l_logMessage.FrameNb = p_myLog->Clock->FrameCount;
		}

		p_myLog->LogMessages.Functions->Writer->PushBack(&p_myLog->LogMessages, &l_logMessage);
	};

	void MyLog_pushLog_string(MyLog* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, Core_GenericArray* p_message)
	{
		MyLog_pushLog(p_myLog, p_logLevel, p_filePath, p_line, (char*)p_message->Memory);
		Core_GenericArray_free(p_message);
	};

	void MyLog_processLogs(MyLog* p_myLog)
	{
		for (size_t i = 0; i < p_myLog->LogMessages.Size; i++)
		{
			LogMessage* l_message = (LogMessage*)p_myLog->LogMessages.Functions->Accessor->At(&p_myLog->LogMessages, i);
			char* l_logLevemMessage = nullptr;

			switch (l_message->LogLevel)
			{
			case LogLevel::INFO:
				l_logLevemMessage = " [INFO] - ";
				printf("\033[0;32m");
				break;

			case LogLevel::DEBUG:
				l_logLevemMessage = " [DEBUG] - ";
				printf("\033[1;32m");
				break;

			case LogLevel::WARN:
				l_logLevemMessage = " [WARN] - ";
				printf("\033[0;33m");
				break;

			case LogLevel::ERROR:
				l_logLevemMessage = " [ERROR] - ";
				printf("\033[0;31m");
				break;
			}

			char  l_frameCountStr[256] = "";
			ultoa(l_message->FrameNb, l_frameCountStr, 10);

			char l_lineCountStr[256] = "";
			itoa(l_message->FileLine, l_lineCountStr, 10);

			strcpy_s(p_myLog->TmpFinalMessage, l_frameCountStr);
			strcat_s(p_myLog->TmpFinalMessage, l_logLevemMessage);
			strcat_s(p_myLog->TmpFinalMessage, l_message->FilePath);
			strcat_s(p_myLog->TmpFinalMessage, ".");
			strcat_s(p_myLog->TmpFinalMessage, l_lineCountStr);
			strcat_s(p_myLog->TmpFinalMessage, " ");
			strcat_s(p_myLog->TmpFinalMessage, l_message->Message);

			printf(p_myLog->TmpFinalMessage);
			printf("\n");
			printf("\033[0m");

			logMessage_free(l_message);
		}

		p_myLog->LogMessages.Functions->Writer->Clear(&p_myLog->LogMessages);

	};

	Core_String MyLog_formatError(char* p_file, int p_line, char* p_message)
	{
		Core_String l_str;
		Core_string_alloc(&l_str, 100);

		char l_intContainer[50];
		Core_toString_intv(l_intContainer, &p_line);

		Core_string_append(&l_str, p_file);
		Core_string_append(&l_str, " ");
		Core_string_append(&l_str, l_intContainer);
		Core_string_append(&l_str, p_message);

		return l_str;
	};

	Core_GenericArray MyLog_formatError_string(const char* p_file, int p_line, Core_GenericArray* p_message)
	{
		Core_GenericArray l_errorMessage;
		Core_string_alloc(&l_errorMessage, 0);

		Core_GenericArray l_lineString;
		Core_string_alloc(&l_lineString, 0);

		Core_toString_int(&l_lineString, &p_line);
		
		Core_string_append(&l_errorMessage, (char*)l_lineString.Memory);
		Core_string_append(&l_errorMessage, (char*)p_message->Memory);

		Core_GenericArray_free(&l_lineString);
		Core_GenericArray_free(p_message);
		return l_errorMessage;
	};
}