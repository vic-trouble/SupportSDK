#ifndef __GAMECORE_RESOURCEINFORMATION_H__
#define __GAMECORE_RESOURCEINFORMATION_H__

#include "../TypeDefinitions.h"
#include "../GenericHandle.h"
#include <Utilities/type_index.h>

namespace SDK
{
	namespace Resources
	{

		struct ResourceSetHandleTag {};
		using ResourceSetHandle = GenericHandle<int, 12, 20, ResourceSetHandleTag>;

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

			// handle int specific system (Mesh, Material) that should be cast to
			//	handle of that system (MeshHandle, MaterialHandle)
			InternalHandle	m_handle;

			std::type_index m_resource_type;

			ResourceSetHandle m_belongs_to_set;

			struct FindByHash
			{
				size_t res_hash;
				const std::type_index& res_type;
				explicit FindByHash(size_t i_hash, const std::type_index& i_type)
					: res_hash(i_hash)
					, res_type(i_type)
				{}

				bool operator () (const ResourceInformation& i_res) const
				{
					return i_res.m_resource_id == res_hash && i_res.m_resource_type == res_type;
				}
			};

			struct FindByHandle
			{
				InternalHandle res_handle;
				const std::type_index& res_type;
				explicit FindByHandle(InternalHandle i_handle, const std::type_index& i_type)
					: res_handle(i_handle)
					, res_type(i_type)
				{}

				bool operator () (const ResourceInformation& i_res) const
				{
					return i_res.m_handle == res_handle && i_res.m_resource_type == res_type;
				}
			};
		};

	} // Resources
} // SDK

#endif