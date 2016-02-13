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

			virtual Shader Compile(const char* ip_vertex, 
				const char* ip_tess_control, const char* ip_tess_eval, 
				const char* ip_geom, 
				const char* ip_fragment, 
				const char* ip_compute) = 0;
			virtual void Release(Shader& io_shader) = 0;

			virtual void SetUniform(const Shader& i_shader, const std::string& i_name, const Matrix4f& i_matrix) = 0;
		};
	}

} // SDK

#endif
