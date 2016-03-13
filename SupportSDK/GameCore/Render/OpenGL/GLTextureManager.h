#ifndef	__GAMECORE_GLTEXTUREMANAGER_H__
#define __GAMECORE_GLTEXTUREMANAGER_H__

#include "Render/TextureManager.h"

#include "GenericHandlesStaticArray.h"

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
		
		class GLTextureManager : public TextureManager
		{
		private:
			friend struct Resources::Serialization::LoaderImpl<Texture>;

		private:
			using TexturesArray = GenericHandlesStaticArray<TextureHandle, Texture, 4096>;
			TexturesArray m_textures;

		public:
			virtual ~GLTextureManager();

			virtual TextureHandle Load(const std::string& i_resource_name, const std::string& i_file_name) override;
			virtual void Unload(TextureHandle i_texture) override;
			virtual Texture* Access(TextureHandle i_texture) override;
			virtual void Bind(int i_target, TextureHandle i_texture) override;
			virtual void Release(int i_target) override;
		};

	} // Render
} // SDK

#endif