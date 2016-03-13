#ifndef __GAMECORE_TEXTUREMANAGER_H__
#define __GAMECORE_TEXTUREMANAGER_H__

#include "../Render/RenderTypes.h"
#include "../Render/Texture.h"

namespace SDK
{
	namespace Render
	{		
		class TextureManager
		{
		public:
			virtual ~TextureManager() {}

			virtual TextureHandle Load(const std::string& i_resource_name, const std::string& i_file_name) = 0;
			virtual void Unload(TextureHandle i_texture) = 0;
			virtual Texture* Access(TextureHandle i_texture) = 0;
			virtual void Bind(int i_target, TextureHandle i_texture) = 0;
			virtual void Release(int i_target) = 0;
		};

	} // Render
} // SDK

#endif