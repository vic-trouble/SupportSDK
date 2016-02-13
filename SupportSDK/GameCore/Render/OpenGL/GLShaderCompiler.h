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

			virtual Shader Compile(const char* ip_vertex,
				const char* ip_tess_control, const char* ip_tess_eval,
				const char* ip_geom,
				const char* ip_fragment,
				const char* ip_compute) override;
			virtual void Release(Shader& io_shader) override;

			virtual void SetUniform(const Shader& i_shader, const std::string& i_name, const Matrix4f& i_matrix) override;
		};
	}

} // SDK

#endif
