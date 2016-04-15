#ifndef	__GAMECORE_OBJECTSTORAGEBASE_H__
#define __GAMECORE_OBJECTSTORAGEBASE_H__

#include "GlobalObjectBase.h"
#include <Utilities/type_index.h>

namespace SDK
{
	class ObjectStorageBase
	{
	private:
		virtual GlobalObjectBase* GetGlobalObjectImpl(size_t i_type_code) const = 0;
		virtual void AddGlobalObjectImpl(std::unique_ptr<GlobalObjectBase> ip_object) = 0;
		virtual void RemoveGlobalObjectImpl(size_t i_type_code) = 0;

	public:
		virtual ~ObjectStorageBase() {}
		
		template <typename ObjectType, typename... Args>
		void AddGlobalObject(Args... args)
		{
			auto p_object = std::make_unique<ObjectType>(args...);
			p_object->RecalcHashCode();
			AddGlobalObjectImpl(std::move(p_object));
		}

		template <typename ObjectType>
		ObjectType* GetGlobalObject() const
		{
#if defined(_DEBUG)
			auto p_obj = GetGlobalObjectImpl(typeid(ObjectType).hash_code());
			assert(p_obj != nullptr && "There is no registered global object");
			assert(dynamic_cast<ObjectType*>(p_obj) != nullptr && "Cannot convert type.");
#endif
			return static_cast<ObjectType*>( GetGlobalObjectImpl(typeid(ObjectType).hash_code()) );
		}

		template <typename ObjectType>
		void RemoveGlobalObject()
		{
			RemoveGlobalObjectImpl(typeid(ObjectType).hash_code());
		}
	};
} // SDK

#endif
