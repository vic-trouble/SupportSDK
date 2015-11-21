#ifndef	__GAMECORE_SHADERSYSTEM_H__
#define __GAMECORE_SHADERSYSTEM_H__

#include "../GameCoreAPI.h"
#include "RenderTypes.h"

#include "Shader.h"

namespace SDK
{
	namespace Resources {	namespace Serialization {
			template <typename ResInfo>
			struct LoaderImpl;
		} // Serialization
	} // Resources

	namespace Render
	{
		class ShaderSystem
		{
			friend struct Resources::Serialization::LoaderImpl<Shader>;
		private:
			// TODO: limitations to 1024 shader loaded - is it enough?
			typedef GenericBuffersArray<ShaderHandler, Shader, 1024> Shaders;
			Shaders m_shaders;

		public:
			ShaderSystem();
			GAMECORE_EXPORT ShaderHandler Load(const std::string& i_vertex_shader_file, const std::string& i_fragment_shader_file);
			GAMECORE_EXPORT void Unload(ShaderHandler i_handler);
			inline Shader Access(ShaderHandler i_handler)
			{
				if (i_handler.index == -1 || i_handler.generation != m_shaders.m_handlers[i_handler.index].generation)
					return Shader(0);
				return m_shaders.m_buffer[i_handler.index];
			}
		};

		// TODO: global object
		GAMECORE_EXPORT extern ShaderSystem g_shader_system;

	} // Render

} // SDK

#endif
