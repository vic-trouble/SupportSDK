#ifndef	__GAMECORE_MATERIAL_H__
#define __GAMECORE_MATERIAL_H__

#include "Render/RenderTypes.h"

namespace SDK
{
	namespace Render
	{

		struct Material
		{
		public:
			ShaderHandler m_shader;
			std::string m_name;
			size_t m_name_hash;

			Material()
				: m_name_hash(0)
				, m_shader{ -1,-1 }
			{}

			Material(const std::string& i_name, size_t i_hash)
				: m_name(i_name)
				, m_name_hash(i_hash)
			{}
		};

	} // Render
} // SDK

#endif
