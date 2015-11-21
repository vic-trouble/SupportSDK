#ifndef	__GAMECORE_SHADERCOMPILER_H__
#define __GAMECORE_SHADERCOMPILER_H__

#include "Shader.h"

namespace SDK
{

	namespace Render
	{
		class Shader;

		class ShaderCompiler
		{
		public:
			virtual ~ShaderCompiler() {}

			virtual Shader Compile(const std::string& i_vertex, const std::string& i_fragment) = 0;
			virtual void Release(Shader& io_shader) = 0;

			virtual void SetUniform(const Shader& i_shader, const std::string& i_name, const Matrix4f& i_matrix) = 0;
		};
	}

} // SDK

#endif
