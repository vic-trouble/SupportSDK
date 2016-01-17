#ifndef	__GAMECORE_OBJECTGETTERBASE_H__
#define __GAMECORE_OBJECTGETTERBASE_H__

#include "GlobalObjectBase.h"
#include <Utilities/type_index.h>

namespace SDK
{
	class ObjectGetterBase
	{
	private:
		virtual GlobalObjectBase* GetGlobalObjectImpl(const std::type_index& i_type) const = 0;
		virtual void AddGlobalObjectImpl(std::unique_ptr<GlobalObjectBase> ip_object) = 0;
		virtual void RemoveGlobalObjectImpl(const std::type_index& i_type) = 0;

	public:
		virtual ~ObjectGetterBase() {}
		
		template <typename ObjectType>
		void AddGlobalObject()
		{
			AddGlobalObjectImpl(std::make_unique<ObjectType>());
		}

		template <typename ObjectType>
		ObjectType* GetGlobalObject() const
		{
#if defined(_DEBUG)
			auto p_obj = GetGlobalObjectImpl(typeid(ObjectType));
			assert(p_obj != nullptr && "There is no registered global object");
			assert(dynamic_cast<ObjectType*>(p_obj) != nullptr && "Cannot convert type.");
#endif
			return static_cast<ObjectType*>(GetGlobalObjectImpl(typeid(ObjectType)));
		}

		template <typename ObjectType>
		void RemoveGlobalObject()
		{
			RemoveGlobalObjectImpl(typeid(ObjectType));
		}
	};
} // SDK

#endif