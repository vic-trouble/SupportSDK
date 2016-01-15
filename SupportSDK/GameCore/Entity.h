#ifndef __GAMECORE_ENTITY_H__
#define __GAMECORE_ENTITY_H__

#include "GenericHandle.h"

namespace SDK
{

	class EntityManager;

	class Entity
	{
		friend class EntityManager;
	private:
		// TODO: vector or other structure - can we know on creation what size to allocate
		std::vector<Component> m_components;

	public:
		template <typename ComponentClass>
		ComponentClass* GetComponent()
		{
			for (Component& component : m_components)
			{
				if (component.m_component_id == ComponentClass::ID)
					return ComponentClass::ProcessorSystem::Get(component.m_in_system_id, component.m_in_system_generation);
			}
			return nullptr;
		}

		void AddComponent(Component i_component)
		{
			m_components.push_back(std::move(i_component));
		}
	};

	// 70368744177663 indices; 65535 generations
	// TODO: this is too many; no?	
	struct EntityTag {};
	typedef GenericHandle<int64, 47, 17, EntityTag> EntityHandle;

} // SDK

#endif
