#include "stdafx.h"

#include "Shader.h"

#include <Utilities/HashFunctions.h>

namespace SDK
{
	namespace Render
	{

		void Shader::AddUniform(const std::string& i_name, int i_location, int i_type)
		{
			const name_hash hash = Utilities::hash_function(i_name);
			m_uniforms.emplace_back(hash, i_location, i_type);
		}

		int Shader::GetUniformLocation(const std::string& i_name) const
		{
			const name_hash hash = Utilities::hash_function(i_name);
			auto it = std::find_if(m_uniforms.begin(), m_uniforms.end(), [hash](const uniform& uni)
			{
				return uni.name_hash == hash;
			});
			if (it != m_uniforms.end())
				return it->location;
			return 0;
		}

		int Shader::GetUniformType(const std::string& i_name) const
		{
			const name_hash hash = Utilities::hash_function(i_name);
			auto it = std::find_if(m_uniforms.begin(), m_uniforms.end(), [hash](const uniform& uni)
			{
				return uni.name_hash == hash;
			});
			if (it != m_uniforms.end())
				return it->type;
			return 0;
		}

	}
} // SDK