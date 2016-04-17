#include "stdafx.h"

#include "BasicPolicies.h"

#include "FileSystem/FileSystem.h"

#include <Utilities/lexical_cast.h>
#include <fstream>

namespace SDK
{
	namespace Log
	{
		/////////////////////////////////////////////////////////////////////////////
		// Filter Policies

		bool NoFilterPolicy::Filter(size_t i_channel, LogType i_type)
		{
			return true;
		}

		/////////////////////////////////////////////////////////////////////////////
		// Format Policies

		std::string ExtendedFormatPolicy::Format(size_t channel, LogType type, const SourceInformation& i_source_info, const std::string& i_message)
		{
			return i_source_info.file_name
				+ std::string("(") + Utilities::lexical_cast(i_source_info.line) + std::string("): ")
				+ i_message + std::string("\n");
		}
		
		/////////////////////////////////////////////////////////////////////////////
		// Writer Policies

		void IDEWriter::Write(const std::string& i_message)
		{
			OutputDebugStringA(i_message.c_str());
		}

		void FileWriter::Write(const std::string& i_message)
		{
			const auto app_path = FS::GetApplicationPath();
			auto path = app_path;
			path.append("\\Log.log");

			std::fstream stream;
			stream.open(path, std::fstream::binary | std::fstream::app);
			if (stream.good())
				stream << i_message << std::endl;
			stream.close();
		}

	} // Log
} // SDK