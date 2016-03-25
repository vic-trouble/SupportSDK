#ifndef	__GAMECORE_MATERIAL_H__
#define __GAMECORE_MATERIAL_H__

#include "Render/RenderTypes.h"

#include "Render/Shader.h"

#include <Utilities/any.h>

namespace SDK
{
	namespace Render
	{

		namespace details
		{
#if defined(_DEBUG)
			template <typename T>
			void CheckShaderValType(ShaderVariableType i_type, const T& i_value)
			{
				using SVT = ShaderVariableType;
				switch (i_type)
				{
					case SVT::Float:
						assert(typeid(T).hash_code() == typeid(float).hash_code());
						break;
					case SVT::FloatVec4:
						assert(typeid(float[4]).hash_code() == typeid(T).hash_code());
						assert(sizeof(i_value) == sizeof(float) * 4);
						break;
					case SVT::FloatMat4:
						// TODO: check type
						assert(sizeof(i_value) == sizeof(float) * 16);
						break;
				}
			}
#endif
			struct ValueContainer
			{
				size_t name_hash;
				bool transposed;
				std::unique_ptr<char[]> p_data;
				size_t size;
				constexpr static int MAX_BUFFER_SIZE = 8;
				char buffer[MAX_BUFFER_SIZE];

				ValueContainer() = default;

				ValueContainer(const ValueContainer& other)
				{
					*this = other;
				}

				ValueContainer& operator = (const ValueContainer& other)
				{
					name_hash = other.name_hash;
					transposed = other.transposed;
					size = other.size;
					std::copy(std::begin(other.buffer), std::end(other.buffer), buffer);
					if (other.p_data)
					{
						p_data.reset(new char[size]);
						memcpy(&p_data[0], other.p_data.get(), size);
					}
					return *this;
				}
				template <typename T>
				ValueContainer(size_t i_hash, const T& i_value, bool i_transposed)
				{
					SetValue(i_hash, i_value, i_transposed);
				}

				ValueContainer(ValueContainer&& other)
					: name_hash(other.name_hash)
					, transposed(other.transposed)
					, p_data(std::move(other.p_data))
					, size(other.size)
				{
					std::copy(std::begin(other.buffer), std::end(other.buffer), buffer);
				}

				template <typename T>
				void SetValue(size_t i_hash, const T& i_value)
				{
					SetValue(i_hash, i_value, false);
				}

				template <typename T>
				void SetValue(size_t i_hash, const T& i_value, bool i_transposed)
				{
					name_hash = i_hash;
					transposed = i_transposed;
					std::fill(std::begin(buffer), std::end(buffer), 0);
					p_data = nullptr;

					const char* p_val = reinterpret_cast<const char*>(&i_value);
					size = sizeof(T);
					if (size <= MAX_BUFFER_SIZE)
					{
						memcpy(buffer, p_val, size);
					}
					else
					{
						p_data.reset(new char[size]);
						std::fill(&p_data[0], &p_data[size], 0);
						memcpy(&p_data[0], p_val, size);
					}
				}

				void SetValue(size_t i_hash, const void* ip_value, size_t i_raw_size)
				{
					SetValue(i_hash, ip_value, i_raw_size, false);
				}

				void SetValue(size_t i_hash, const void* ip_value, size_t i_raw_size, bool i_transposed)
				{
					name_hash = i_hash;
					transposed = i_transposed;
					std::fill(std::begin(buffer), std::end(buffer), 0);
					p_data = nullptr;

					const char* p_val = reinterpret_cast<const char*>(ip_value);
					size = i_raw_size;
					if (size <= MAX_BUFFER_SIZE)
					{
						memcpy(buffer, p_val, size);
					}
					else
					{
						p_data.reset(new char[size]);
						std::fill(&p_data[0], &p_data[size], 0);
						memcpy(&p_data[0], p_val, size);
					}
				}

				const void* GetDataPtr() const
				{
					if (p_data)
						return p_data.get();
					return buffer;
				}

			};
			

			struct MaterialEntry
			{
				int shader_var_location = -1;
				ValueContainer container;
				ShaderVariableType type = ShaderVariableType::Undefined;
				
				std::string show_name;

				MaterialEntry(int i_location, ValueContainer i_value, ShaderVariableType i_type, const std::string& i_name)
					: shader_var_location(i_location)
					, container(std::move(i_value))
					, show_name(i_name)
					, type(i_type)
				{}				
			};
		} // details

		struct Material
		{
		public:
			ShaderHandle m_shader;
			
			std::vector<details::MaterialEntry> m_entries;

			std::string m_name;
			size_t m_name_hash;
			float m_color[4];

			Material()
				: m_name_hash(0)
				, m_shader{ -1,-1 }
				, m_color{ 0,0,0,0 }
			{}

			Material(const std::string& i_name, size_t i_hash)
				: m_name(i_name)
				, m_name_hash(i_hash)
				, m_color{0,0,0,0}
				, m_shader{ -1, -1 }
			{}

			/*Material& operator = (Material&&)
			{
				return *this;
			}*/
		};

	} // Render
} // SDK

#endif
