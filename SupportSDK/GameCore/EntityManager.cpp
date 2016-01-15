#include "stdafx.h"

#include "EntityManager.h"

#include "Render/Mesh.h"
#include "Systems/MeshSystem.h"

namespace SDK
{
	// TODO: global
	EntityManager g_entity_manager;

	EntityHandle EntityManager::CreateEntity()
	{
		// find free entity slot

		// create entity
		m_entities.push_back(Entity());
		EntityHandle handler;
		handler.index = m_entities.size() - 1;
		handler.generation = 0;
		m_handlers.push_back(handler);

		return handler;
	}

	void EntityManager::RemoveEntity(EntityHandle i_handler)
	{
		if (m_handlers[i_handler.index].generation != i_handler.generation)
		{
			assert("Generations does not identical");
			return;
		}
		// remove as entity
		auto& components = m_entities[i_handler.index].m_components;
		for (auto& component : components)
			component.m_remove_function(component.m_in_system_id, component.m_in_system_generation);
		m_entities[i_handler.index].m_components.clear();	

		m_handlers[i_handler.index].index = -1;
		++m_handlers[i_handler.index].generation;
	}

	EntitySubset EntityManager::CreateSubset()
	{
		return EntitySubset();
	}

	void EntityManager::RemoveSubset(const EntitySubset& i_set)
	{
		const auto& entities = i_set.GetHandles();
		for (auto& entity : entities)
			RemoveEntity(entity);
	}

} // SDK