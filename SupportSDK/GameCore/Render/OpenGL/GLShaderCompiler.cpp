#include "stdafx.h"

#include "GLShaderCompiler.h"

#include "GlUitlities.h"

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

namespace SDK
{

	namespace Render
	{

		Shader GLShaderCompiler::Compile(const std::string& i_vertex, const std::string& i_fragment)
		{
			// Create the shaders
			GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			GLint Result = GL_FALSE;
			int InfoLogLength = -1;
			// compile Vertex Shader
			const char* VertexSourcePointer = i_vertex.c_str();
			glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
			glCompileShader(VertexShaderID);
			CHECK_GL_ERRORS;

			// Check Vertex Shader
			glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			CHECK_GL_ERRORS;
			if (InfoLogLength > 0) {
				std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
				glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
				// TODO: Log
				// printf("%s\n", &VertexShaderErrorMessage[0]);				
			}

			// Compile Fragment Shader
			const char* FragmentSourcePointer = i_fragment.c_str();
			glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
			glCompileShader(FragmentShaderID);
			CHECK_GL_ERRORS;
			
			// Check Fragment Shader
			glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0) {
				std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
				glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
				// TODO: Log
				// printf("%s\n", &FragmentShaderErrorMessage[0]);				
			}
			CHECK_GL_ERRORS;


			// Link program
			uint program_id = glCreateProgram();
			glAttachShader(program_id, VertexShaderID);
			glAttachShader(program_id, FragmentShaderID);
			glLinkProgram(program_id);

			// Check the program
			glGetProgramiv(program_id, GL_LINK_STATUS, &Result);
			glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0) {
				std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
				glGetProgramInfoLog(program_id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
				// TODO: Log
				// printf("%s\n", &ProgramErrorMessage[0]);				
			}
			CHECK_GL_ERRORS;

			glDetachShader(program_id, VertexShaderID);
			glDetachShader(program_id, FragmentShaderID);
			
			glDeleteShader(VertexShaderID);			
			glDeleteShader(FragmentShaderID);

			glUseProgram(0);

			return Shader(program_id);
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