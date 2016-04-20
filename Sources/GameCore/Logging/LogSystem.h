#ifndef	__GAMECORE_LOGSYSTEM_H__
#define __GAMECORE_LOGSYSTEM_H__

#include "../GameCoreAPI.h"

#include "LoggingDefinitions.h"

#include <Utilities/StringUtilities.h>

namespace SDK
{
	namespace Log
	{
		namespace details
		{
			class LogSystem
			{
			private:
				using LoggerPtr = std::unique_ptr<Logger>;
				using Loggers = std::vector<LoggerPtr>;
				Loggers m_loggers;

			public:
				LogSystem();

				GAMECORE_EXPORT void Register(LoggerPtr ip_logger);
				GAMECORE_EXPORT void Remove(size_t i_type);
				GAMECORE_EXPORT void LogImpl(const char* i_channel, LogType i_type, SourceInformation i_source, std::string i_message) const;

				template <typename... Args>
				void Log(const char* i_channel, LogType i_type, SourceInformation i_source, const std::string& i_format, Args... i_args) const
				{
					LogImpl(i_channel, i_type, i_source, Strings::FormatString(i_format, i_args...));
				}

				void Log(const char* i_channel, LogType i_type, SourceInformation i_source, const std::string& i_format) const
				{
					LogImpl(i_channel, i_type, i_source, i_format);
				}
			};

			GAMECORE_EXPORT extern LogSystem g_log_system;

		} // details

		template <typename LoggerType>
		void RegisterLogger()
		{
			details::g_log_system.Register(std::make_unique<LoggerType>());
		}
		template <typename LoggerType>
		void RemoveLogger()
		{
			details::g_log_system.RegisterLogger(typeid(LoggerType).hash_code());
		}

		template <typename... Args>
		void Debug(const char* i_channel, SourceInformation i_source, const std::string& i_format, Args... i_args)
		{
			details::g_log_system.Log(i_channel, LogType::Debug, i_source, i_format, i_args...);
		}
		template <typename... Args>
		void Info(const char* i_channel, SourceInformation i_source, const std::string& i_format, Args... i_args)
		{
			details::g_log_system.Log(i_channel, LogType::Info, i_source, i_format, i_args...);
		}
		template <typename... Args>
		void Warn(const char* i_channel, SourceInformation i_source, const std::string& i_format, Args... i_args)
		{
			details::g_log_system.Log(i_channel, LogType::Warning, i_source, i_format, i_args...);
		}
		template <typename... Args>
		void Error(const char* i_channel, SourceInformation i_source, const std::string& i_format, Args... i_args)
		{
			details::g_log_system.Log(i_channel, LogType::Error, i_source, i_format, i_args...);
		}
		template <typename... Args>
		void Fatal(const char* i_channel, SourceInformation i_source, const std::string& i_format, Args... i_args)
		{
			details::g_log_system.Log(i_channel, LogType::Fatal, i_source, i_format, i_args...);
		}

	} // Log
} // SDK

#define Source { __FILE__, __func__, __LINE__ }

#endif