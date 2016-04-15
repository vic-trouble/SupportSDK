#include "stdafx.h"

#include "Core.h"

// objects
#include "Render/MeshSystem.h"
#include "EntityManager.h"
#include "Render/ShaderSystem.h"

namespace SDK
{

	namespace GlobalObjects
	{
		void RegisterDefaultGlobalObjects()
		{
			auto p_storage = Core::GetGlobalObjectStorage();
			p_storage->AddGlobalObject<EntityManager>();
		}
	}

} // SDK