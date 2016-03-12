#ifndef	__GAMECORE_SHADER_H__
#define __GAMECORE_SHADER_H__

#include "../GameCoreAPI.h"

#include "../Render/RenderTypes.h"

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
			uniform GetUniform(size_t i_name_hash) const;
		};
#if defined(_DEBUG)
		template <typename T>
		void CheckType(ShaderVariableType i_type, const T& i_value)
		{
			using SVT = ShaderVariableType;
			switch (i_type)
			{
				case SVT::Float:
					assert(typeid(T).hash_code() == typeid(float).hash_code());
					break;
				case SVT::FloatVec4:
					assert(typeid(float[4]).hash_code() == typeid(T).hash_code());
					assert(sizeof(i_value) == sizeof(float)*4);
					break;
				case SVT::FloatMat4:
					// TODO: check type
					assert(sizeof(i_value) == sizeof(float) * 16);
					break;
			}
		}
#endif
		struct ShaderUniformValue
		{
			int location;
			ShaderVariableType type;
			bool transposed;
			char* p_data;
			size_t size;			
			constexpr static int MAX_BUFFER_SIZE = 8;
			char buffer[MAX_BUFFER_SIZE];

			template <typename T>
			void SetValue(int i_location, ShaderVariableType i_type, const T& i_value)
			{
				SetValue(i_location, i_type, i_value, false);
			}

			template <typename T>
			void SetValue(int i_location, ShaderVariableType i_type, const T& i_value, bool i_transposed)
			{
				// TODO: own macro for such checks
#if defined(_DEBUG)
				CheckType(i_type, i_value);
#endif
				SetValue(i_location, i_type, &i_value, sizeof(T), i_transposed);				
			}

			void SetValue(int i_location, ShaderVariableType i_type, const void* ip_value, size_t i_size, bool i_transposed)
			{
				location = i_location;
				type = i_type;
				transposed = i_transposed;
				std::fill(std::begin(buffer), std::end(buffer), 0);
				p_data = nullptr;

				const char* p_val = reinterpret_cast<const char*>(ip_value);
				size = i_size;
				if (size <= MAX_BUFFER_SIZE)
				{
					memcpy(buffer, p_val, size);
				}
				else
				{
					p_data = new char[size];
					std::fill(p_data, p_data + size, 0);
					memcpy(p_data, p_val, size);
				}
			}

			const void* GetDataPtr() const
			{
				if (p_data)
					return p_data;
				return buffer;
			}

			static ShaderUniformValue Construct(int i_location, ShaderVariableType i_type, const void* ip_raw_data, size_t i_size, bool i_transposed)
			{
				ShaderUniformValue val;
				val.SetValue(i_location, i_type, ip_raw_data, i_size, i_transposed);
				return val;
			}
			void Reset()
			{
				size = 0;
				p_data = nullptr;
				location = -1;
			}
		};
		static_assert(std::is_pod<ShaderUniformValue>::value, "");

	} // Render
} // SDK

#endif
