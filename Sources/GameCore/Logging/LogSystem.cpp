#include "stdafx.h"

#include "LogSystem.h"
#include "Logger.h"

namespace SDK
{
	namespace Log
	{
		namespace details
		{
			LogSystem g_log_system;

			LogSystem::LogSystem()
			{}

			void LogSystem::Register(LoggerPtr ip_logger)
			{
				const size_t type_hash = typeid(*ip_logger).hash_code();
				auto it = std::find_if(m_loggers.begin(), m_loggers.end(), [type_hash](const LoggerPtr& p_logger)
				{
					return typeid(*p_logger).hash_code() == type_hash;
				});
				if (it == m_loggers.end())
					m_loggers.push_back(std::move(ip_logger));
			}

			void LogSystem::Remove(size_t i_type)
			{
				m_loggers.erase(
					std::find_if(m_loggers.begin(), m_loggers.end(), [i_type](const LoggerPtr& p_logger)
				{return typeid(*p_logger).hash_code() == i_type; }),
					m_loggers.end());
			}

			void LogSystem::LogImpl(const char* i_channel, LogType i_type, SourceInformation i_source, std::string i_message) const
			{
				for (const auto& p_logger : m_loggers)
					p_logger->Log(i_channel, i_type, i_source, i_message);
			}
		} // details
	} // Log
} // SDK