#ifndef	__GAMECORE_FONT_H__
#define __GAMECORE_FONT_H__

#include "GenericHandle.h"

#include "Render/RenderTypes.h"

namespace SDK
{
	namespace Render
	{
		struct FontSettings
		{
			std::string name;
			std::wstring symbols_to_load;
			size_t pixel_width = 0;
			size_t pixel_height = 48;
		};

		struct Font
		{
			struct Character {
				int			char_id;	// id of character
				Vector2i	Size;       // Size of glyph
				Vector2i	Bearing;    // Offset from baseline to left/top of glyph
				Vector2i	Advance;	// Offset to advance to next glyph
				Vector2		UV_bot_left;
				Vector2		UV_top_right;

				Character()
					: char_id(-1)
					, Size{ 0,0 }
					, Bearing{ 0,0 }
					, Advance{0, 0}
				{}
			};
			std::string m_name;
			std::vector<Character> m_characters;
			TextureHandle m_texture_handle;

			Character Find(int i_char_id) const
			{
				const auto it = std::find_if(m_characters.begin(), m_characters.end(), [i_char_id](const Character& ch)
				{
					return ch.char_id == i_char_id;
				});
				if (it != m_characters.end())
					return *it;
				return Character();
			}
		};

		struct FontHandleTag {};
		using FontHandle = GenericHandle<int, 12, 20, FontHandleTag>;
	}
} // SDK

#endif