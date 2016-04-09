#include "stdafx.h"

#include "DefaultObjectsStorage.h"

namespace SDK
{

	DefaultObjectsStorage::DefaultObjectsStorage()
	{
		// Recalculate hash
		m_resource_manager.RecalcHashCode();

		// Add to cache
		m_cache_objects.push_back(&m_resource_manager);
	}

	GlobalObjectBase* DefaultObjectsStorage::GetGlobalObjectImpl(size_t i_type_code) const
	{
		for (auto* p_object : m_cache_objects)
		{
			if (p_object->GetTypeHashCode() == i_type_code)
				return p_object;
		}
		return nullptr;
	}

	void DefaultObjectsStorage::AddGlobalObjectImpl(std::unique_ptr<GlobalObjectBase> ip_object)
	{
#if defined(_DEBUG)
		if (GetGlobalObjectImpl(typeid(*ip_object.get()).hash_code()) != nullptr)
		{
			assert(false);
			return;
		}
#endif
		// cache object and add it to pool
		m_cache_objects.push_back(ip_object.get());
		m_dynamic_objects.emplace_back(std::move(ip_object));

	}

	void DefaultObjectsStorage::RemoveGlobalObjectImpl(size_t i_type_code)
	{	
		const auto it_dyn = std::find_if(m_dynamic_objects.begin(), m_dynamic_objects.end(), [i_type_code](ObjPtr& p_obj)
		{
			return typeid(*p_obj.get()).hash_code() == i_type_code;
		});
		// we can delete from cache only dynamic objects - not static which are defined inside this class
		//	and will be removed with destruction of getter
		if (it_dyn != m_dynamic_objects.end())
		{
			// remove from cache
			const auto it = std::find_if(m_cache_objects.begin(), m_cache_objects.end(), [i_type_code](GlobalObjectBase* p_obj)
			{
				return typeid(*p_obj).hash_code() == i_type_code;
			});

			if (it != m_cache_objects.end())
				m_cache_objects.erase(it);

			m_dynamic_objects.erase(it_dyn);
		}
	}

} // SDK