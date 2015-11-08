#ifndef __GAMECORE_RESOURCEINFORMATION_H__
#define __GAMECORE_RESOURCEINFORMATION_H__

#include "../TypeDefinitions.h"

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
			int			m_handle;

			struct FindPredicate
			{
				size_t id;
				int handle;
				explicit FindPredicate(size_t i_id)
					: id(i_id)
					, handle(-1)
				{}

				explicit FindPredicate(int i_handle)
					: handle(i_handle)
					, id(0)
				{}

				bool operator () (const ResourceInformation& i_res) const
				{
					return handle == -1 ? i_res.m_resource_id == id : handle == i_res.m_handle;
				}
			};
		};

	} // Resources

} // SDK

#endif