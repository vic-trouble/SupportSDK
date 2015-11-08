#ifndef __GAMECORE_RESOURCEINFORMATION_H__
#define __GAMECORE_RESOURCEINFORMATION_H__

#include "../TypeDefinitions.h"

namespace SDK
{

	namespace Resources
	{

		struct Resource
		{
		protected:
			size_t	m_id;

		public:
			Resource()
				: m_id(0)
			{}
		};


		struct ResInformation
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

				bool operator () (const ResInformation& i_res) const
				{
					return handle == -1 ? i_res.m_resource_id == id : handle == i_res.m_handle;
				}
			};
		};

		// DataType must be POD
		template <typename DataType>
		struct ResourceInformation
		{
			// e.x., hash
			size_t		m_resource_id;
			uint		m_use_count;

			DataType	m_data;

			ResourceInformation()
				: m_use_count(0)
				, m_resource_id(0)
			{}

			ResourceInformation(size_t i_resource_id, DataType i_data)
				: m_resource_id(i_resource_id)
				, m_use_count(0)
				, m_data(i_data)
			{}

			struct FindPredicate
			{
				size_t id;
				FindPredicate(size_t i_id)
					: id(i_id)
				{}

				bool operator () (const ResourceInformation& i_res) const
				{
					return i_res.m_resource_id == id;
				}
			};
		};

		template <typename DataType>
		inline bool operator < (const ResourceInformation<DataType>& i_first, const ResourceInformation<DataType>& i_second)
		{
			return i_first.m_resource_id < i_second.m_resource_id;
		}

	} // Resources

} // SDK

#endif