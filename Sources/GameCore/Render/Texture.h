#ifndef	__GAMECORE_TEXTURE_H__
#define __GAMECORE_TEXTURE_H__

#include "TextureDefinitions.h"

namespace SDK
{
	namespace Render
	{		
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

		struct RawTexture
		{
			std::unique_ptr<ubyte> mp_data;
			size_t m_width = 0;
			size_t m_height = 0;
			size_t m_deepth = 0;

			RawTexture(size_t i_width, size_t i_height, size_t i_deepth)
				: m_width(i_width)
				, m_height(i_height)
				, m_deepth(i_deepth)
			{
				mp_data.reset(new ubyte[m_width*m_height*m_deepth]);
				Clear();
			}

			RawTexture(RawTexture&& i_other)
				: mp_data(std::move(i_other.mp_data))
				, m_width(i_other.m_width)
				, m_height(i_other.m_height)
				, m_deepth(i_other.m_deepth)
			{}

			void Clear()
			{
				memset(mp_data.get(), 0, m_width*m_height*m_deepth);
			}
		};

	} // Render
} // SDK

#endif
