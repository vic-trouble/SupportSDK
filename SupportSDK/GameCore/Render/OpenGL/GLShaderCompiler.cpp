#include "stdafx.h"

#include "GLShaderCompiler.h"

#include "GlUitlities.h"

#include "Render/Shader.h"
#include "Render/irenderer.h"
#include "Render/HardwareBufferManagerBase.h"

#include <Utilities/HashFunctions.h>

#include "Core.h"

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

namespace SDK
{

	namespace Render
	{
		namespace ShaderCompilerDetails
		{
			GLuint CompileShader(GLenum i_shader_type, const char* ip_source)
			{
				GLuint shader = glCreateShader(i_shader_type);
				glShaderSource(shader, 1, &ip_source, NULL);
				glCompileShader(shader);
				CHECK_GL_ERRORS;
				GLint isCompiled = 0;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
				if (isCompiled == GL_FALSE)
				{
					GLint maxLength = 0;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

					// The maxLength includes the NULL character
					std::vector<GLchar> errorLog(maxLength);
					glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
					OutputDebugStringA(&errorLog[0]);
					// Provide the infolog in whatever manor you deem best.
					// TODO: Log
					
					// Exit with failure.
					glDeleteShader(shader); // Don't leak the shader.
					return 0;
				}

				return shader;
			}

			void DetachAndDeleteShaders(uint o_program, const GLuint* ip_sources, size_t i_size)
			{
				for (size_t i = 0; i < i_size; ++i)
				{
					if (ip_sources[i] != 0)
					{
						if (o_program != 0)
							glDetachShader(o_program, ip_sources[i]);
						glDeleteShader(ip_sources[i]);
					}
				}
			}

			uint LinkProgram(const GLuint* ip_sources, size_t i_size)
			{
				uint program_id = glCreateProgram();

				for (size_t i = 0; i < i_size; ++i)
				{
					if (ip_sources[i] != 0)
						glAttachShader(program_id, ip_sources[i]);
				}
				glLinkProgram(program_id);
				CHECK_GL_ERRORS;

				//Note the different functions here: glGetProgram* instead of glGetShader*.
				GLint isLinked = 0;
				glGetProgramiv(program_id, GL_LINK_STATUS, (int *)&isLinked);
				if (isLinked == GL_FALSE)
				{
					GLint maxLength = 0;
					glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &maxLength);

					//The maxLength includes the NULL character
					std::vector<GLchar> infoLog(maxLength);
					glGetProgramInfoLog(program_id, maxLength, &maxLength, &infoLog[0]);

					OutputDebugStringA(&infoLog[0]);

					//We don't need the program anymore.
					glDeleteProgram(program_id);

					//Don't leak shaders either.
					DetachAndDeleteShaders(0, ip_sources, i_size);

					//Use the infoLog as you see fit.
					// TODO: Log
					// printf("%s\n", &VertexShaderErrorMessage[0]);                

					glUseProgram(0);
					//In this simple program, we'll just leave
					return 0;
				}

				DetachAndDeleteShaders(program_id, ip_sources, i_size);

				glUseProgram(0);

				return program_id;
			}

			VertexSemantic DetectSemantic(const std::string& i_attr_name)
			{
				if (i_attr_name == "pos" || i_attr_name == "position")
					return VertexSemantic::Position;
				if (i_attr_name == "uv")
					return VertexSemantic::TextureCoordinates;
				if (i_attr_name == "normal")
					return VertexSemantic::Normal;
				if (i_attr_name == "color")
					return VertexSemantic::Color;
				if (i_attr_name == "diffuse")
					return VertexSemantic::Diffuse;
				if (i_attr_name == "specular")
					return VertexSemantic::Specular;
				if (i_attr_name == "ambient")
					return VertexSemantic::Ambient;

				return VertexSemantic::Dynamic;
			}

			void FetchAttributes(Shader& o_shader, GLuint i_program)
			{
				constexpr static GLenum properties[] = { GL_NAME_LENGTH, GL_LOCATION, GL_TYPE };

				GLint num_uniforms = 0;
				glGetProgramInterfaceiv(i_program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &num_uniforms);
				GLint max_length = 0;
				glGetProgramInterfaceiv(i_program, GL_PROGRAM_INPUT, GL_MAX_NAME_LENGTH, &max_length);
				CHECK_GL_ERRORS;

				o_shader.ResetUniformsNumber(num_uniforms);

				std::string name_data;
				name_data.resize(max_length);
				for (int unif = 0; unif < num_uniforms; ++unif)
				{
					GLint values[3];
					glGetProgramResourceiv(i_program, GL_PROGRAM_INPUT, unif, sizeof(properties) / sizeof(GLenum), properties, sizeof(values) / sizeof(GLint), NULL, values);
					CHECK_GL_ERRORS;
					name_data.resize(values[0]);
					glGetProgramResourceName(i_program, GL_PROGRAM_INPUT, unif, name_data.size(), NULL, &name_data[0]);
					// in case of structures uniforms can contain not needed symbols
					std::string real_name(name_data);
					real_name = real_name.substr(0, real_name.size() - 1);
					o_shader.AddAttribute(real_name, values[1], values[2], DetectSemantic(real_name));
				}
			}
			
			struct KnownUniform
			{
				UniformType uni_type;
				std::array<const char*, 2> names;
				std::array<size_t, 2> name_hashes;
			};			

			static KnownUniform known_uniforms[] = {
				{ UniformType::ProjectionMatrix, {"projection_matrix", "proj_m"}, {Utilities::hash_function(std::string("projection_matrix")), Utilities::hash_function(std::string("proj_m")) } },
				{ UniformType::ModelviewMatrix, { "modelview_matrix", "mv_m" },{ Utilities::hash_function(std::string("modelview_matrix")), Utilities::hash_function(std::string("mv_m")) } }
			};

			UniformType DetectUniform(const std::string& i_uni_name)
			{
				const size_t target_hash = Utilities::hash_function(i_uni_name);
				for (KnownUniform uni : known_uniforms)
				{
					for (size_t hash : uni.name_hashes)
					{
						if (hash == target_hash)
							return uni.uni_type;
					}
				}				
				return UniformType::Dynamic;
			}

			void FetchUniforms(Shader& o_shader, GLuint i_program)
			{
				constexpr static GLenum properties[4] = { GL_BLOCK_INDEX, GL_NAME_LENGTH, GL_LOCATION, GL_TYPE };

				GLint num_uniforms = 0;
				glGetProgramInterfaceiv(i_program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &num_uniforms);
				GLint max_length = 0;
				glGetProgramInterfaceiv(i_program, GL_UNIFORM, GL_MAX_NAME_LENGTH, &max_length);
				CHECK_GL_ERRORS;
				
				o_shader.ResetUniformsNumber(num_uniforms);

				std::string name_data;
				name_data.resize(max_length);
				for (int unif = 0; unif < num_uniforms; ++unif)
				{
					GLint values[4];
					glGetProgramResourceiv(i_program, GL_UNIFORM, unif, sizeof(properties)/sizeof(GLenum), properties, sizeof(values)/sizeof(GLint), NULL, values);

					//Skip any uniforms that are in a block.
					if (values[0] != -1)
						continue;

					name_data.resize(values[1]);
					glGetProgramResourceName(i_program, GL_UNIFORM, unif, name_data.size(), NULL, &name_data[0]);
					// in case of structures uniforms can contain not needed symbols
					std::string real_name(name_data);
					real_name = real_name.substr(0, real_name.size() - 1);
					o_shader.AddUniform(real_name, values[2], values[3], DetectUniform(real_name));
				}
			}

		} // ShaderCompilerDetails

		Shader GLShaderCompiler::Compile(const char* ip_vertex,
			const char* ip_tess_control, const char* ip_tess_eval,
			const char* ip_geom,
			const char* ip_fragment,
			const char* ip_compute)
		{
			GLuint shaders[Shader::TypesNumber];
			for (auto& id : shaders)
				id = 0;

			if (ip_vertex != nullptr)
			{
				shaders[Shader::Vertex] = ShaderCompilerDetails::CompileShader(GL_VERTEX_SHADER, ip_vertex);
				if (shaders[Shader::Vertex] == 0)
					return Shader();
			}
			if (ip_tess_control != nullptr)
			{
				shaders[Shader::TesselationConrol] = ShaderCompilerDetails::CompileShader(GL_TESS_CONTROL_SHADER, ip_tess_control);
				if (shaders[Shader::TesselationConrol] == 0)
					return Shader();
			}
			if (ip_tess_eval != nullptr)
			{
				shaders[Shader::TesselationEvaluation] = ShaderCompilerDetails::CompileShader(GL_TESS_EVALUATION_SHADER, ip_tess_eval);
				if (shaders[Shader::TesselationEvaluation] == 0)
					return Shader();
			}
			if (ip_geom != nullptr)
			{
				shaders[Shader::Geometry] = ShaderCompilerDetails::CompileShader(GL_GEOMETRY_SHADER, ip_geom);
				if (shaders[Shader::Geometry] == 0)
					return Shader();
			}
			if (ip_fragment != nullptr)
			{
				shaders[Shader::Fragment] = ShaderCompilerDetails::CompileShader(GL_FRAGMENT_SHADER, ip_fragment);
				if (shaders[Shader::Fragment] == 0)
					return Shader();
			}
			if (ip_compute != nullptr)
			{
				shaders[Shader::Compute] = ShaderCompilerDetails::CompileShader(GL_COMPUTE_SHADER, ip_compute);
				if (shaders[Shader::Compute] == 0)
					return Shader();
			}

			uint program_id = ShaderCompilerDetails::LinkProgram(shaders, Shader::TypesNumber);
			Shader shader(program_id);
			// if shader valid fetch its parameters
			if (shader.IsValid())
			{
				ShaderCompilerDetails::FetchUniforms(shader, program_id);
				ShaderCompilerDetails::FetchAttributes(shader, program_id);
			}
			return shader;
		}

		void GLShaderCompiler::Release(Shader& io_shader)
		{
			glDeleteProgram(io_shader.GetId());
			CHECK_GL_ERRORS;
			io_shader.Reset();
		}

		void GLShaderCompiler::SetUniform(uint i_location, ShaderVariableType i_type, const void* const ip_value, bool i_transposed) const
		{
			switch (i_type)
			{
				case ShaderVariableType::Int:
					glUniform1i(i_location, reinterpret_cast<int>(ip_value));					
					break;
				case ShaderVariableType::Float:
					glUniform1f(i_location, *reinterpret_cast<const float*>(ip_value));
					break;
				case ShaderVariableType::FloatVec4:
					{
						const float* p_val = reinterpret_cast<const float*>(ip_value);
						glUniform4f(i_location, p_val[0], p_val[1], p_val[2], p_val[3]);
					}
					break;
				case ShaderVariableType::FloatMat4:
					glUniformMatrix4fv(i_location, 1, Convert(i_transposed), reinterpret_cast<const float*>(ip_value));
					break;
			}
			CHECK_GL_ERRORS;
		}

	} // Render

} // SDK