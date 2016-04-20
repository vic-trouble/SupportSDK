#ifndef	__GAMECORE_TEXTUREDEFINITIONS_H__
#define __GAMECORE_TEXTUREDEFINITIONS_H__

namespace SDK
{
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
	} // Render
} // SDK

#endif