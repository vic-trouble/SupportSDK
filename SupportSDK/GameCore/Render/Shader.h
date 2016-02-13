#ifndef	__GAMECORE_SHADER_H__
#define __GAMECORE_SHADER_H__

#include "../GameCoreAPI.h"

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
		private:
			uint m_program_id;

			using name_hash = size_t;
			using location = uint;
			struct uniform
			{
				size_t name_hash;
				int location;
				int type;
				uniform(size_t i_hash, int i_location, int i_type)
					: name_hash(i_hash)
					, location(i_location)
					, type(i_type)
				{}
			};
			std::vector<uniform> m_uniforms;

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
			GAMECORE_EXPORT void AddUniform(const std::string& i_name, int i_location, int i_type);
			GAMECORE_EXPORT int GetUniformLocation(const std::string& i_name) const;
			GAMECORE_EXPORT int GetUniformType(const std::string& i_name) const;
		};

	} // Render
} // SDK

#endif
