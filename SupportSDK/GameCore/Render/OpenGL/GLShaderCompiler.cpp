#include "stdafx.h"

#include "GLShaderCompiler.h"

#include "GlUitlities.h"

#include "Render/Shader.h"

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

					// Provide the infolog in whatever manor you deem best.
					// TODO: Log
					// printf("%s\n", &VertexShaderErrorMessage[0]);                

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
					o_shader.AddUniform(name_data, values[2], values[3]);
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
			}
			return shader;
		}

		void GLShaderCompiler::Release(Shader& io_shader)
		{
			glDeleteProgram(io_shader.GetId());
			CHECK_GL_ERRORS;
			io_shader.Reset();
		}

		void SetUniformImpl(uint i_location, const float* i_values, uint i_count)
		{
			glUniformMatrix4fv(i_location, i_count, GL_FALSE, i_values);
		}
		struct Uniform
		{
			uint location;
		};
		void GLShaderCompiler::SetUniform(const Shader& i_shader, const std::string& i_name, const Matrix4f& i_matrix)
		{
			Uniform uniform;
			{
				auto _program = i_shader.GetId();
				const int CONST_UNIFORM_NAME_MAX_LENGTH = 256;
				char uniformName[CONST_UNIFORM_NAME_MAX_LENGTH];

				int activeUniformsCount = 0;
				glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &activeUniformsCount);
				CHECK_GL_ERRORS;

				for (int i = 0; i < activeUniformsCount; i++) {
					GLsizei nameLen;
					GLint uniformSize;
					GLenum uniformType;

					glGetActiveUniform(_program, (GLuint)i, CONST_UNIFORM_NAME_MAX_LENGTH, &nameLen, &uniformSize, &uniformType, uniformName);
					CHECK_GL_ERRORS;

					GLint uniformLocation = glGetUniformLocation(_program, uniformName);

					// Имя юниформы в случае структур или массивов может содержать лишние символы,
					// поэтому их надо обрезать
					std::string realName(uniformName);
					realName = realName.substr(0, realName.find_first_of("[]", 0, 1));
					if (realName == i_name)
					{
						uniform.location = uniformLocation;
						break;
					}
				}
			}


			
			uint uniform_size = 16;
			uint size = 16;
			GLsizei uniform_count = size / uniform_size;
			SetUniformImpl(uniform.location, i_matrix[0], uniform_count);
		}

	} // Render

} // SDK