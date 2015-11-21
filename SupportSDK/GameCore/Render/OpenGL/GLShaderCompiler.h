#ifndef	__GAMECORE_GLSHADERCOMPILER_H__
#define __GAMECORE_GLSHADERCOMPILER_H__

#include "Render/ShaderCompiler.h"

namespace SDK
{

	namespace Render
	{

		class GLShaderCompiler : public ShaderCompiler
		{
		public:
			virtual ~GLShaderCompiler() {}

			virtual Shader Compile(const std::string& i_vertex, const std::string& i_fragment) override;
			virtual void Release(Shader& io_shader) override;

			virtual void SetUniform(const Shader& i_shader, const std::string& i_name, const Matrix4f& i_matrix) override;
		};
	}

} // SDK

#endif
