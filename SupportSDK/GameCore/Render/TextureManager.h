#ifndef __GAMECORE_TEXTUREMANAGER_H__
#define __GAMECORE_TEXTUREMANAGER_H__

#include "../Render/RenderTypes.h"
#include "../GenericHandlesStaticArray.h"

namespace SDK
{
	namespace Render
	{
		struct TextureHandleTag {};
		typedef GenericHandle<int, 12, 20, TextureHandleTag> TextureHandle;

		class TextureManager
		{
		public:
			virtual ~TextureManager() {}

			virtual TextureHandle Load(const std::string& i_file_name) = 0;
			virtual void Bind(int i_target, TextureHandle) {}
		};

	} // Render
} // SDK

#endif