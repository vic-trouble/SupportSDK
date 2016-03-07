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
		enum class TextureType
		{
			OneD,
			TwoD,
			ThreeD,
			Rectangle,
			Buffer,
			CubeMap,
			OneDArray,
			TwoDArray,
			CubeMapArray,
			TwoDMultisample,
			TwoDMultisampleArray
		};

		enum class ImageFormat
		{
			RGBA
		};		

		enum class Swizzle
		{
			Red, // The value for this component comes from the red channel of the image. All color formats have at least a red channel
			Green, // The value for this component comes from the green channel of the image, or 0 if it has no green channel
			Blue, // The value for this component comes from the green channel of the image, or 0 if it has no blue channel
			Alpha, // The value for this component comes from the alpha channel of the image, or 1 if it has no alpha channel
			Zero, // The value for this component is always 0
			One // The value for this component is always 1
		};

		struct Texture
		{
			int hardware_id;
			TextureType type;
			ImageFormat image_format;
			// one of Swizzle mask
			//	set with
			//	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask); 
			//		- map the red channel in the image to the alpha channel when the shader accesses it
			int swizzle_mask[4];
		};
		class GLTextureManager : public TextureManager
		{
		private:
			friend struct Resources::Serialization::LoaderImpl<Texture>;

		private:
			using TexturesArray = GenericHandlesStaticArray<TextureHandle, Texture, 4096>;
			TexturesArray m_textures;

		public:
			virtual ~GLTextureManager();

			virtual TextureHandle Load(const std::string& i_file_name) override;
		};

	} // Render
} // SDK

#endif