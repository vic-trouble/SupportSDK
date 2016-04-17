#ifndef	__GAMECORE_LOGGINGDEFINITIONS_H__
#define __GAMECORE_LOGGINGDEFINITIONS_H__

namespace SDK
{
	namespace Log
	{
		enum class LogType
		{
			Debug,
			Info,
			Warning,
			Error,
			Fatal
		};

		struct SourceInformation
		{
			const char* file_name;
			const char* function_name;
			const int line;
		};

		class Logger;

	} // Log
} // SDK

#endif