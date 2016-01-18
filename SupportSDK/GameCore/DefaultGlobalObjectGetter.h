#ifndef	__GAMECORE_DEFAULTGLOBALOBJECTGETTER_H__
#define __GAMECORE_DEFAULTGLOBALOBJECTGETTER_H__

#include "GameCoreAPI.h"

#include "ObjectGetterBase.h"

// objects
#include "Resources/ResourceManager.h"

namespace SDK
{

	class DefaultGlobalObjectGetter : public ObjectGetterBase
	{
	private:
		std::vector<GlobalObjectBase*> m_cache_objects;
		using ObjPtr = std::unique_ptr<GlobalObjectBase>;
		std::vector<ObjPtr> m_dynamic_objects;

		// static objects
		Resources::ResourceManager m_resource_manager;

	private:
		virtual GlobalObjectBase* GetGlobalObjectImpl(const std::type_index& i_type) const override;
		virtual void AddGlobalObjectImpl(std::unique_ptr<GlobalObjectBase> ip_object) override;
		virtual void RemoveGlobalObjectImpl(const std::type_index& i_type) override;

	public:
		GAMECORE_EXPORT DefaultGlobalObjectGetter();		
	};

} // SDK

#endif
