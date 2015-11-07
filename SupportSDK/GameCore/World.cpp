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
		for (auto& system : m_systems)
		{
			if (system.Requires(System::Action::Update))
				system.Update(i_elapsed_time);
		}
	}

	void World::SubmitDrawCommands()
	{
		for (auto& system : m_systems)
		{
			if (system.Requires(System::Action::SubmitDrawCommands))
				system.SubmitDrawCommands();
		}
	}

} // SDK