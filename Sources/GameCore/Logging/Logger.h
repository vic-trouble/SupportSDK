#ifndef	__GAMECORE_LOGGER_H__
#define __GAMECORE_LOGGER_H__

#include "LoggingDefinitions.h"

namespace SDK
{
	namespace Log
	{
		class Logger
		{
		public:
			virtual ~Logger() {}
			virtual void Log(size_t channel, LogType type, const SourceInformation& i_source_info, const std::string& i_message) = 0;
		};

		template <typename FilterPolicy, typename FormatPolicy, typename WritePolicy>
		class LoggerImpl : public Logger
		{
		private:
			FilterPolicy m_filter;
			FormatPolicy m_formatter;
			WritePolicy m_writer;

		public:
			virtual void Log(size_t channel, LogType type, const SourceInformation& i_source_info, const std::string& i_message) override
			{
				if (m_filter.Filter(channel, type))
				{
					std::string buffer = m_formatter.Format(channel, type, i_source_info, i_message);
					m_writer.Write(buffer);
				}
			}
		};

	} // Log
} // SDK

#endif