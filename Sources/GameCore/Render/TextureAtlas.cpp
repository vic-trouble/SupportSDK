#include "stdafx.h"

#include "TextureAtlas.h"

namespace SDK
{
	namespace Render
	{
		TextureAtlas::TextureAtlas(size_t i_width, size_t i_height, size_t i_deepth)
			: m_texture(i_width, i_height, i_deepth)
			, m_in_use(0)
		{
			// We want a one pixel border around the whole atlas to avoid any artefact when
			// sampling texture
			m_nodes.emplace_back(Vector3i{ 1, 1, (int)i_width - 2 });
		}

		int TextureAtlas::AtlasFit(const size_t i_index, const size_t i_width, const size_t i_height)
		{
			auto& node = m_nodes[i_index];
			int x = node[0];
			int y = node[1];
			int width_left = i_width;
			
			size_t i = i_index;

			if ((x + i_width) > (m_texture.m_width - 1))
				return -1;

			y = node[1];
			while (width_left > 0)
			{
				auto& node = m_nodes[i];
				if (node[1] > y)
				{
					y = node[1];
				}
				if ((y + i_height) > (m_texture.m_height - 1))
					return -1;
				width_left -= node[2];
				++i;
			}
			return y;
		}

		void TextureAtlas::Merge()
		{
			for (size_t i = 0; i < m_nodes.size() - 1; ++i)
			{
				auto& node = m_nodes[i];
				auto& next = m_nodes[i + 1];
				if (node[1] == next[1])
				{
					node[2] += next[2];
					m_nodes.erase(m_nodes.begin() + i + 1);
					--i;
				}
			}
		}

		IRect TextureAtlas::GetNewRegion(const size_t i_width, const size_t i_height)
		{
			int best_index = -1;
			size_t best_height = std::numeric_limits<size_t>::max(); 
			size_t best_width = std::numeric_limits<size_t>::max();
			IRect region({ 0,0 }, i_width, i_height);
			int y = 0;
			for (size_t i = 0; i < m_nodes.size(); ++i)
			{
				y = AtlasFit(i, i_width, i_height);
				if (y >= 0)
				{
					const Vector3i& node = m_nodes[i];
					if (((y + i_height) < best_height) ||
						(((y + i_height) == best_height) && (node[2] > 0 && node[2] < best_width)))
					{
						best_height = y + i_height;
						best_index = i;
						best_width = node[2];
						region.SetBottomLeft({ node[0],  y });
					}
				}
			}
			
			if (best_index == -1)
				return IRect{ {-1,-1}, 0, 0 };

			Vector2i reg = region.GetBottomLeft();
			Vector3i new_node{ reg[0], reg[1] + (int)i_height, (int)i_width };
			m_nodes.insert(m_nodes.begin() + best_index, new_node);

			for (size_t i = best_index + 1; i < m_nodes.size(); ++i)
			{
				auto& node = m_nodes[i];
				const auto& prev = m_nodes[i - 1];

				if (node[0] < (prev[0] + prev[2]))
				{
					int shrink = prev[0] + prev[2] - node[0];
					node[0] += shrink;
					node[2] -= shrink;
					if (node[2] <= 0)
					{
						m_nodes.erase(m_nodes.begin() + i);
						--i;
					}
					else
						break;
				}
				else
					break;
			}
			
			Merge();
			m_in_use += i_width*i_height;
			return region;
		}

		void TextureAtlas::SetRegion(const IRect& i_region, const ubyte* ip_data, const size_t i_stride)
		{
			auto bot_left = i_region.GetBottomLeft();
			assert(bot_left[0] > 0);
			assert(bot_left[1] > 0);
			assert(bot_left[0] < (m_texture.m_width - 1));
			assert((bot_left[0] + i_region.Width()) <= (m_texture.m_width - 1));
			assert(bot_left[1] < (m_texture.m_height - 1));
			assert(bot_left[1] + i_region.Height() <= m_texture.m_height - 1);

			int x = bot_left[0];
			int y = bot_left[1];
			int width = i_region.Width();
			int height = i_region.Height();

			size_t depth = m_texture.m_deepth;
			size_t charsize = sizeof(ubyte);
			for (size_t i = 0; i < height; ++i)
			{
				memcpy(m_texture.mp_data.get() + ((y + i)*m_texture.m_width + x) * charsize * depth,
					ip_data + (i*i_stride) * charsize, width * charsize * depth);
			}
		}

		void TextureAtlas::Clear()
		{
			m_in_use = 0;
			m_nodes.clear();
			m_nodes.emplace_back(Vector3i{ 1, 1, (int)m_texture.m_width - 2 });
			m_texture.Clear();
		}
	} // Render
} // SDK