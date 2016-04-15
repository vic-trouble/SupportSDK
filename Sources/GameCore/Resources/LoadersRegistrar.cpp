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
			Core::GetGlobalObject<Render::MaterialManager>()->Initialize();
			Core::GetGlobalObject<Render::ShaderSystem>()->Initialize();
			Render::g_mesh_system.Initialize();
			Core::GetRenderer()->GetTextureManager()->Initialize();
		}

		void ReleaseLoaders()
		{
			Core::GetGlobalObject<Render::MaterialManager>()->Release();
			Core::GetGlobalObject<Render::ShaderSystem>()->Release();
			Render::g_mesh_system.Release();
			Core::GetRenderer()->GetTextureManager()->Release();
		}

	} // Resources
} // SDK