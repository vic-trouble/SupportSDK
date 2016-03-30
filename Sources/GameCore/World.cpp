#include "stdafx.h"

#include "World.h"

namespace SDK
{

	World::World()
	{
	}

	void World::Update(float i_elapsed_time)
	{
		for (auto& p_system : m_active_systems)
		{
			if (p_system->Requires(System::Action::Update))
				p_system->Update(i_elapsed_time);
		}
	}

	void World::SubmitDrawCommands()
	{
		for (auto& p_system : m_active_systems)
		{
			if (p_system->Requires(System::Action::SubmitDrawCommands))
				p_system->SubmitDrawCommands();
		}
	}

	void World::RegisterSystem(System* ip_system)
	{
		m_active_systems.push_back(ip_system);
	}

	void World::UnregisterSystem(System* ip_system)
	{
		auto it = std::find(m_active_systems.begin(), m_active_systems.end(), ip_system);
		if (it != m_active_systems.end())
			m_active_systems.erase(it);
	}

	void World::ClearSystems()
	{
		m_active_systems.clear();
	}

} // SDK