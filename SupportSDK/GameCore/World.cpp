#include "stdafx.h"

#include "World.h"

namespace SDK
{

	World::World()
	{
		m_viewport.mp_camera = &m_camera;
		m_viewport.mp_frustum = &m_frustum;
	}

	void World::Update(float i_elapsed_time)
	{
		for (auto& p_system : m_systems)
		{
			if (p_system->Requires(System::Action::Update))
				p_system->Update(i_elapsed_time);
		}
	}

	void World::SubmitDrawCommands()
	{
		for (auto& p_system : m_systems)
		{
			if (p_system->Requires(System::Action::SubmitDrawCommands))
				p_system->SubmitDrawCommands();
		}
	}

} // SDK