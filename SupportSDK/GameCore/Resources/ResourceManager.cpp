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
		static ResourceManager manager;
		ResourceManager& g_load_manager = manager;

		FS::StreamPtr ResourceManager::OpenStream(const std::string& i_file_name)
		{			
			FS::StreamPtr p_stream(new FS::FileStream(i_file_name));
			if (!p_stream->IsValid())
				return nullptr;
			return p_stream;
		}

	} // Resources

} // SDK