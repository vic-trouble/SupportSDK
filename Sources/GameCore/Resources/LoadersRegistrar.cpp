#include "stdafx.h"

#include "ResourceManager.h"
#include "Core.h"
#include "Render/IRenderer.h"
// Loaders
#include "Render/MaterialManager.h"
#include "Render/ShaderSystem.h"
#include "Render/TextureManager.h"
#include "Render/MeshSystem.h"

namespace SDK
{
	namespace Resources
	{

		void RegisterLoaders()
		{
			Render::g_material_mgr.Initialize();
			Render::g_shader_system.Initialize();
			Render::g_mesh_system.Initialize();
			Core::GetRenderer()->GetTextureManager()->Initialize();
		}

		void ReleaseLoaders()
		{
			Render::g_material_mgr.Release();
			Render::g_shader_system.Release();
			Render::g_mesh_system.Release();
			Core::GetRenderer()->GetTextureManager()->Release();
		}

	} // Resources
} // SDK