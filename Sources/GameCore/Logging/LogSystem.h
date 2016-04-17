#ifndef	__GAMECORE_LOGSYSTEM_H__
#define __GAMECORE_LOGSYSTEM_H__

#include "../GameCoreAPI.h"

#include "LoggingDefinitions.h"

#include <Utilities/StringUtilities.h>

namespace SDK
{
	namespace Log
	{		
		class LogSystem
		{
		private:
			using LoggerPtr = std::unique_ptr<Logger>;
			using Loggers = std::vector<LoggerPtr>;
			Loggers m_loggers;

		private:
			GAMECORE_EXPORT void Register(LoggerPtr ip_logger);
			GAMECORE_EXPORT void Remove(size_t i_type);
			GAMECORE_EXPORT void LogImpl(size_t i_channel, LogType i_type, SourceInformation i_source, std::string i_message) const;

		public:
			LogSystem();

			template <typename LoggerType>
			void RegisterLogger()
			{
				Register(std::make_unique<LoggerType>());
			}

			template <typename LoggerType>
			void RemoveLogger()
			{
				Remove(typeid(LoggerType).hash_code());
			}

			template <typename... Args>
			void Log(size_t i_channel, LogType i_type, SourceInformation i_source, const std::string& i_format, Args... i_args) const
			{
				LogImpl(i_channel, i_type, i_source, Strings::FormatString(i_format, i_args...));
			}

			void Log(size_t i_channel, LogType i_type, SourceInformation i_source, const std::string& i_format) const
			{
				LogImpl(i_channel, i_type, i_source, i_format);
			}
		};

		GAMECORE_EXPORT extern LogSystem g_log;

		template <typename... Args>
		void Debug(size_t i_channel, SourceInformation i_source, const std::string& i_format, Args... i_args)
		{
			g_log.Log(i_channel, LogType::Debug, i_source, i_format, i_args...);
		}

		template <typename... Args>
		void Info(size_t i_channel, SourceInformation i_source, const std::string& i_format, Args... i_args)
		{
			g_log.Log(i_channel, LogType::Info, i_source, i_format, i_args...);
		}
		template <typename... Args>
		void Warn(size_t i_channel, SourceInformation i_source, const std::string& i_format, Args... i_args)
		{
			g_log.Log(i_channel, LogType::Warning, i_source, i_format, i_args...);
		}
		template <typename... Args>
		void Error(size_t i_channel, SourceInformation i_source, const std::string& i_format, Args... i_args)
		{
			g_log.Log(i_channel, LogType::Error, i_source, i_format, i_args...);
		}
		template <typename... Args>
		void Fatal(size_t i_channel, SourceInformation i_source, const std::string& i_format, Args... i_args)
		{
			g_log.Log(i_channel, LogType::Fatal, i_source, i_format, i_args...);
		}

	} // Log
} // SDK

#endif