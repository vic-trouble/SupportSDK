#ifndef __GAMECORE_ENTITYMANAGER_H__
#define __GAMECORE_ENTITYMANAGER_H__

#include "GameCoreAPI.h"

#include "GenericHandle.h"

#include "Component.h"
#include "Entity.h"

namespace SDK
{
	class EntityManager
	{
	private:
		std::vector<Entity> m_entities;
		std::vector<EntityHandle> m_handlers;

	public:

		GAMECORE_EXPORT EntityHandle CreateEntity();
		GAMECORE_EXPORT void RemoveEntity(EntityHandle i_handler);

		template <typename ComponentClass, typename T, T N1, T N2, typename Tag>
		void AddComponent(EntityHandle i_entity_handle, GenericHandle<T, N1, N2, Tag> i_component_handle)
		{
			Component component;
			component.m_component_id = ComponentClass::ID;
			component.m_in_system_id = static_cast<int>(i_component_handle.index);
			component.m_in_system_generation = static_cast<int>(i_component_handle.generation);
			component.m_remove_function = &ComponentClass::ProcessorSystem::Remove;

			auto& entity = m_entities[i_entity_handle.index];

			entity.AddComponent(std::move(component));
			auto p_component = entity.GetComponent<ComponentClass>();
			p_component->SetEntity(i_entity_handle);
		}

		Entity* GetEntity(EntityHandle i_handler)
		{
			if (i_handler.index != -1 && i_handler.generation != m_handlers[i_handler.index].generation)
			{
				//assert(false && "Generations are not identical");
				return nullptr;
			}
			return &m_entities[i_handler.index];
		}

		GAMECORE_EXPORT EntitySubset CreateSubset();
		GAMECORE_EXPORT void RemoveSubset(const EntitySubset& i_set);
	};

	// TODO: global variable
	GAMECORE_EXPORT extern EntityManager g_entity_manager;

} // SDK

#endif
