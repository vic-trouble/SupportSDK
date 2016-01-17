#include "stdafx.h"

#include "DefaultGlobalObjectGetter.h"

namespace SDK
{

	DefaultGlobalObjectGetter::DefaultGlobalObjectGetter()
	{
		m_cache_objects.push_back(&m_resource_manager);
	}

	GlobalObjectBase* DefaultGlobalObjectGetter::GetGlobalObjectImpl(const std::type_index& i_type) const
	{
		const auto it = std::find_if(m_cache_objects.begin(), m_cache_objects.end(), [i_type](GlobalObjectBase* p_obj)
		{
			return std::type_index(typeid(*p_obj)) == i_type;
		});
		
		if (it != m_cache_objects.end())
			return *it;

		return nullptr;
	}

	void DefaultGlobalObjectGetter::AddGlobalObjectImpl(std::unique_ptr<GlobalObjectBase> ip_object)
	{
#if defined(_DEBUG)
		if (GetGlobalObjectImpl(typeid(*ip_object.get())) != nullptr)
		{
			assert(false);
			return;
		}
#endif
		// cache object and add it to pool
		m_cache_objects.push_back(ip_object.get());
		m_dynamic_objects.emplace_back(std::move(ip_object));

	}

	void DefaultGlobalObjectGetter::RemoveGlobalObjectImpl(const std::type_index& i_type)
	{
		const auto it = std::find_if(m_cache_objects.begin(), m_cache_objects.end(), [i_type](GlobalObjectBase* p_obj)
		{
			return std::type_index(typeid(*p_obj)) == i_type;
		});

		if (it != m_cache_objects.end())
			m_cache_objects.erase(it);

		const auto it_dyn = std::find_if(m_dynamic_objects.begin(), m_dynamic_objects.end(), [i_type](ObjPtr& p_obj)
		{
			return std::type_index(typeid(*p_obj.get())) == i_type;
		});
		if (it != m_cache_objects.end())
			m_dynamic_objects.erase(it_dyn);
	}

} // SDK