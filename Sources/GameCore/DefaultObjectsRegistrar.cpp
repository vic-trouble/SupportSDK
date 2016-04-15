#include "stdafx.h"

#include "Core.h"

// objects
#include "Render/MaterialManager.h"
#include "Render/MeshSystem.h"
#include "Render/ShaderSystem.h"
#include "Render/Fonts/FontManager.h"

#include "Systems/TransformationsSystem.h"

#include "EntityManager.h"

#include "UI/UIControlSystem.h"

#include "Input/InputSystem.h"

namespace SDK
{

	namespace GlobalObjects
	{
		void RegisterDefaultGlobalObjects()
		{
			auto p_storage = Core::GetGlobalObjectStorage();
			p_storage->AddGlobalObject<InputSystem>();

			p_storage->AddGlobalObject<Render::MeshSystem>();
			p_storage->AddGlobalObject<Render::MaterialManager>();
			p_storage->AddGlobalObject<Render::FontManager>();
			p_storage->AddGlobalObject<Render::ShaderSystem>();

			p_storage->AddGlobalObject<EntityManager>();

			p_storage->AddGlobalObject<TransformationsSystem>();

			p_storage->AddGlobalObject<UI::UIControlSystem>();
		}

		void ClearDefaultGlobalObjects()
		{
			auto p_storage = Core::GetGlobalObjectStorage();
			p_storage->RemoveGlobalObject<Render::MeshSystem>();
			p_storage->RemoveGlobalObject<Render::MaterialManager>();
			p_storage->RemoveGlobalObject<Render::FontManager>();
			p_storage->RemoveGlobalObject<Render::ShaderSystem>();

			p_storage->RemoveGlobalObject<EntityManager>();

			p_storage->RemoveGlobalObject<TransformationsSystem>();

			p_storage->RemoveGlobalObject<UI::UIControlSystem>();

			p_storage->RemoveGlobalObject<InputSystem>();
		}
	}

} // SDK