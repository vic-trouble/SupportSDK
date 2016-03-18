#ifndef __GAMECORE_TEXTUREMANAGER_H__
#define __GAMECORE_TEXTUREMANAGER_H__

#include "../Render/RenderTypes.h"
#include "../Render/Texture.h"

#include "../PropertyElement.h"

namespace SDK
{

	namespace Resources {
		namespace Serialization {

			template <typename ResInfo>
			struct LoaderImpl;

		} // Serialization
	} // Resources

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

			virtual void Initialize() = 0;
			virtual void Release() = 0;
			virtual void Load(const PropertyElement& i_resource_element) = 0;
		};

	} // Render
} // SDK

#endif