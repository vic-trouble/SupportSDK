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

	} // Render
} // SDK

#endif
