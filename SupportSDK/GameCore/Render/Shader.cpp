#include "stdafx.h"

#include "Shader.h"

#include <Utilities/HashFunctions.h>

namespace SDK
{
	namespace Render
	{

		void Shader::AddUniform(const std::string& i_name, int i_location, int i_type, VertexSemantic i_semantic)
		{
			// hash without \0 in the end
			const name_hash hash = Utilities::hash_function(i_name.substr(0, i_name.size() - 1));
			m_uniforms.emplace_back(hash, i_location, i_type, i_semantic);
		}

		Shader::input_entry Shader::GetUniform(const std::string& i_name) const
		{
			const name_hash hash = Utilities::hash_function(i_name);
			auto it = std::find_if(m_uniforms.begin(), m_uniforms.end(), [hash](const input_entry& uni)
			{
				return uni.name_hash == hash;
			});
			if (it != m_uniforms.end())
				return *it;
			return input_entry();
		}

		void Shader::AddAttribute(const std::string& i_name, int i_location, int i_type, VertexSemantic i_semantic)
		{
			// hash without \0 in the end
			const name_hash hash = Utilities::hash_function(i_name.substr(0, i_name.size() - 1));
			m_attributes.emplace_back(hash, i_location, i_type, i_semantic);
		}

		Shader::input_entry Shader::GetAttribute(const std::string& i_name) const
		{
			const name_hash hash = Utilities::hash_function(i_name);
			auto it = std::find_if(m_attributes.begin(), m_attributes.end(), [hash](const input_entry& uni)
			{
				return uni.name_hash == hash;
			});
			if (it != m_attributes.end())
				return *it;
			return input_entry();
		}

	}
} // SDK