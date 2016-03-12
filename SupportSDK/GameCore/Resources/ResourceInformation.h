#ifndef __GAMECORE_RESOURCEINFORMATION_H__
#define __GAMECORE_RESOURCEINFORMATION_H__

#include "../TypeDefinitions.h"
#include "../GenericHandle.h"

namespace SDK
{

	namespace Resources
	{
		struct ResourceInformation
		{
			enum class State
			{
				Unloaded,
				Loaded
			};
			// e.x., hash - used internally with ResourceManager
			size_t		m_resource_id;
			uint		m_use_count;
			State		m_state;

			// 
			InternalHandle	m_handle;

			struct FindPredicate
			{
				size_t id;
				InternalHandle handle;
				explicit FindPredicate(size_t i_id)
					: id(i_id)
					, handle(InternalHandle::InvalidHandle())
				{}

				explicit FindPredicate(InternalHandle i_handle)
					: handle(i_handle)
					, id(0)
				{}

				bool operator () (const ResourceInformation& i_res) const
				{
					return handle.index == -1 ? i_res.m_resource_id == id : handle == i_res.m_handle;
				}
			};
		};

	} // Resources

} // SDK

#endif