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
			struct input_entry
			{
				size_t name_hash;
				int location;
				int type;
				VertexSemantic element_semantic;

				input_entry()
					: input_entry(0, -1, -1, VertexSemantic::Dynamic)
				{}

				input_entry(size_t i_hash, int i_location, int i_type, VertexSemantic i_semantic)
					: name_hash(i_hash)
					, location(i_location)
					, type(i_type)
					, element_semantic(i_semantic)
				{}
			};
		private:
			uint m_program_id;
			
			using name_hash = size_t;
			using location = uint;
			
			std::vector<input_entry> m_uniforms;
			std::vector<input_entry> m_attributes;

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
			GAMECORE_EXPORT void AddUniform(const std::string& i_name, int i_location, int i_type, VertexSemantic i_semantic);
			GAMECORE_EXPORT input_entry GetUniform(const std::string& i_name) const;

			GAMECORE_EXPORT void AddAttribute(const std::string& i_name, int i_location, int i_type, VertexSemantic i_semantic);
			GAMECORE_EXPORT input_entry GetAttribute(const std::string& i_name) const;

			const std::vector<input_entry>& GetAttributes() const { return m_attributes; }
			const std::vector<input_entry>& GetUniforms() const { return m_uniforms; }
		};

	} // Render
} // SDK

#endif
