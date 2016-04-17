#ifndef	__GAMECORE_BASICPOLICIES_H__
#define __GAMECORE_BASICPOLICIES_H__

#include "../GameCoreAPI.h"

#include "Logger.h"

namespace SDK
{
	namespace Log
	{
		/////////////////////////////////////////////////////////////////////////////
		// Filter Policies

		struct NoFilterPolicy
		{
			GAMECORE_EXPORT bool Filter(size_t i_channel, LogType i_type);
		};

		template <size_t hash>
		struct ChannelFilterPolicy
		{
			bool Filter(size_t i_channel, LogType i_type)
			{
				return hash == i_channel;
			}
		};

		template <Log::LogType... types>
		struct LogTypeFilterPolicy
		{
			bool Filter(size_t i_channel, Log::LogType i_type)
			{
				bool result[] = { types == i_type... };
				for (bool interested_type : result)
				{
					if (interested_type)
						return true;
				}
				return false;
			}
		};

		template <typename First, typename Second>
		struct ComposedFilterPolicy
		{
		private:
			First m_first;
			Second m_second;
		public:
			bool Filter(size_t i_channel, LogType i_type)
			{
				return m_first.Filter(i_channel, i_type) && m_second.Filter(i_channel, i_type);
			}
		};

		/////////////////////////////////////////////////////////////////////////////
		// Format Policies

		struct SimpleFormatPolicy
		{
			std::string Format(size_t channel, LogType type, const SourceInformation& i_source_info, const std::string& i_message)
			{
				return i_message;
			}
		};

		struct ExtendedFormatPolicy
		{
			GAMECORE_EXPORT std::string Format(size_t channel, LogType type, const SourceInformation& i_source_info, const std::string& i_message);
		};

		/////////////////////////////////////////////////////////////////////////////
		// Writer Policies

		struct IDEWriter
		{
			GAMECORE_EXPORT void Write(const std::string& i_message);
		};

		struct FileWriter
		{
			GAMECORE_EXPORT void Write(const std::string& i_message);
		};

		/////////////////////////////////////////////////////////////////////////////
		// Loggers definitions

		using IDELogger = LoggerImpl<NoFilterPolicy, ExtendedFormatPolicy, IDEWriter>;
		using FileLogger = LoggerImpl<NoFilterPolicy, ExtendedFormatPolicy, FileWriter>;
	} // Log
} // SDK

#endif