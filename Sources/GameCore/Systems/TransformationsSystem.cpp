#include "stdafx.h"

#include "Core.h"

#include "TransformationsSystem.h"

namespace SDK
{
	////////////////////////////////////////////////////////////////////////////////
	// System

	void TransformationsSystem::Update(float i_elapsed_time)
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	// Own methods

	TransformHandler TransformationsSystem::CreateInstance()
	{
		static TransformHandler error_handler{ -1, -1 };
		// get free index
		int new_index = -1;
		for (size_t i = 0; i < m_handlers.size(); ++i)
		{
			if (m_handlers[i].index == -1)
			{
				new_index = static_cast<int>(i);
				m_handlers[i].index = i;
				break;
			}
		}
		// create index
		if (new_index == -1)
		{
			// TODO: resize vector normally - with some strategy - not push_back
			const size_t new_count = m_handlers.size() + 1;
			m_handlers.resize(new_count);
			m_transforms.resize(new_count);
			new_index = new_count - 1;
			m_handlers[new_index].generation = 0;
			m_handlers[new_index].index = new_index;


		}

		//////////////////
		// set data for MeshComponent			
		m_transforms[new_index] = Transform();
		assert(m_transforms.size() == m_handlers.size());
		return m_handlers[new_index];
	}

	Transform* TransformationsSystem::GetInstance(TransformHandler i_handler)
	{
		if (m_handlers[i_handler.index].generation != i_handler.generation)
		{
			assert(false && "Generation is not identical");
			return nullptr;
		}
		return &m_transforms[i_handler.index];
	}

	void TransformationsSystem::RemoveInstance(TransformHandler i_handler)
	{
		m_handlers[i_handler.index].index = -1;
		++m_handlers[i_handler.index].generation;
	}

	///////////////////////////////////////////////////////////////////////////////
	// Extension for EntityManager

	Transform* TransformationsSystem::Get(int i_in_system_id, int i_in_system_generation)
	{
		TransformHandler inst_handler{ i_in_system_id, i_in_system_generation };
		Transform* component = Core::GetGlobalObject<TransformationsSystem>()->GetInstance(inst_handler);
		return component;
	}

	void TransformationsSystem::Remove(int i_in_system_id, int i_in_system_generation)
	{
		Core::GetGlobalObject<TransformationsSystem>()->RemoveInstance({ i_in_system_id, i_in_system_generation });
	}

} // SDK