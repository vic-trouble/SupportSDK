#include "stdafx.h"
#include "ResourceManager.h"

#include "Core.h"
#include "Render/HardwareBufferManagerBase.h"
#include "Render/IRenderer.h"

#include "FileSystem/FileStream.h"

#include <fstream>

namespace SDK
{
	using namespace Render;

	namespace Resources
	{
		FS::StreamPtr ResourceManager::OpenStream(const std::string& i_file_name)
		{			
			FS::StreamPtr p_stream(new FS::FileStream(i_file_name));
			if (!p_stream->IsValid())
				return nullptr;
			return p_stream;
		}

		InternalHandle ResourceManager::GetHandleToResource(const std::string& i_res_name) const
		{
			const size_t hash = Utilities::hash_function(i_res_name);
			// check if handle already exist
			auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash));
			if (it != m_loaded_resources.end())
				return{ it->m_handle.index, it->m_handle.generation };
			return InternalHandle::InvalidHandle();
		}

		void ResourceManager::Use(const std::string& i_res_name)
		{
			const size_t hash = Utilities::hash_function(i_res_name);
			// check if handle already exist
			auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash));
			if (it != m_loaded_resources.end())
				++it->m_use_count;
			else
			{
				assert(false && "No resource with such handle");
			}
		}

	} // Resources

} // SDK