#include "stdafx.h"

#include "Core.h"

// objects
#include "Render/MaterialManager.h"
#include "Render/MeshSystem.h"
#include "Render/ShaderSystem.h"
#include "Render/Fonts/FontManager.h"

#include "EntityManager.h"

namespace SDK
{

	namespace GlobalObjects
	{
		void RegisterDefaultGlobalObjects()
		{
			auto p_storage = Core::GetGlobalObjectStorage();
			p_storage->AddGlobalObject<Render::MaterialManager>();
			p_storage->AddGlobalObject<Render::FontManager>();
			p_storage->AddGlobalObject<Render::ShaderSystem>();
			p_storage->AddGlobalObject<EntityManager>();			
		}
	}

} // SDK