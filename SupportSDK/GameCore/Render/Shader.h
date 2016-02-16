#ifndef	__GAMECORE_SHADER_H__
#define __GAMECORE_SHADER_H__

#include "../GameCoreAPI.h"

#include "Render/RenderTypes.h"

namespace SDK
{
	namespace Render
	{

		class Shader
		{
		public:
			enum ShaderType
			{
				Vertex = 0,
				TesselationConrol,
				TesselationEvaluation,
				Geometry,
				Fragment,
				Compute,

				TypesNumber
			};
			struct attribute
			{
				size_t name_hash;
				int location;
				int type;
				VertexSemantic element_semantic;

				attribute()
					: attribute(0, -1, -1, VertexSemantic::Dynamic)
				{}

				attribute(size_t i_hash, int i_location, int i_type, VertexSemantic i_semantic)
					: name_hash(i_hash)
					, location(i_location)
					, type(i_type)
					, element_semantic(i_semantic)
				{}
			};

			struct uniform
			{
				size_t name_hash;
				int location;
				int type;
				UniformType uniform_type;

				uniform()
					: uniform(0, -1, -1, UniformType::Dynamic)
				{}

				uniform(size_t i_hash, int i_location, int i_type, UniformType i_uni_type)
					: name_hash(i_hash)
					, location(i_location)
					, type(i_type)
					, uniform_type(i_uni_type)
				{}
			};

		private:
			uint m_program_id;
			
			using name_hash = size_t;
			using location = uint;
			
			std::vector<uniform> m_uniforms;
			std::vector<attribute> m_attributes;

		public:
			Shader(uint i_program_id)
				: m_program_id(i_program_id)
			{}

			Shader()
				: m_program_id(0)
			{}

			void Reset()
			{
				m_program_id = 0;
			}

			bool IsValid() const
			{
				return m_program_id != 0;
			}

			uint GetId() const
			{
				return m_program_id;
			}

			void ResetUniformsNumber(size_t i_new_number) { m_uniforms.reserve(i_new_number); }
			GAMECORE_EXPORT void AddUniform(const std::string& i_name, int i_location, int i_type, UniformType i_semantic);
			GAMECORE_EXPORT uniform GetUniform(const std::string& i_name) const;

			GAMECORE_EXPORT void AddAttribute(const std::string& i_name, int i_location, int i_type, VertexSemantic i_semantic);
			GAMECORE_EXPORT attribute GetAttribute(const std::string& i_name) const;

			const std::vector<attribute>& GetAttributes() const { return m_attributes; }
			const std::vector<uniform>& GetUniforms() const { return m_uniforms; }
		};

	} // Render
} // SDK

#endif
