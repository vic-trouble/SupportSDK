#ifndef	__GAMECORE_FONT_H__
#define __GAMECORE_FONT_H__

#include "GenericHandle.h"

namespace SDK
{
	namespace Render
	{
		struct Font
		{
			struct Character {
				uint		TextureID;  // ID handle of the glyph texture
				int			char_id;	// id of character
				Vector2i	Size;       // Size of glyph
				Vector2i	Bearing;    // Offset from baseline to left/top of glyph
				int			Advance;    // Offset to advance to next glyph
				Vector2		UV_top_left;
				Vector2		UV_bot_right;

				Character()
					: TextureID(0)
					, char_id(-1)
					, Size{ 0,0 }
					, Bearing{ 0,0 }
					, Advance(0)
				{}

				Character(uint i_tex_id, int i_char_id, Vector2i i_size, Vector2i i_bearing, int i_advance)
					: TextureID(i_tex_id)
					, char_id(i_char_id)
					, Size(i_size)
					, Bearing(i_bearing)
					, Advance(i_advance)
					, UV_top_left{ 0.f, 1.f }
					, UV_bot_right{ 1.f, 0.f }
				{}
			};

			std::vector<Character> m_characters;

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