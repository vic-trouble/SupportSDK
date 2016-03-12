#ifndef	__GAMECORE_MATERIALMANAGER_H__
#define __GAMECORE_MATERIALMANAGER_H__

#include "../GameCoreAPI.h"

#include "Render/Material.h"
#include "../GenericHandlesDynamicArray.h"

namespace SDK
{
	namespace Resources {
		namespace Serialization {

			template <typename ResInfo>
			struct LoaderImpl;

		} // Serialization
	} // Resources

	namespace Render
	{

		class MaterialManager
		{
		private:
			friend struct Resources::Serialization::LoaderImpl<Material>;
		private:
			using Materials = SDK::GenericHandleDynamicArray<MaterialHandle, Material>;
			Materials m_materials;

		public:
			GAMECORE_EXPORT MaterialHandle Load(const std::string& i_resource_name, const std::string& i_path);
			GAMECORE_EXPORT const Material* AccessMaterial(MaterialHandle i_handle) const;
			GAMECORE_EXPORT void RemoveMaterial(MaterialHandle i_handle);

			void CreateCommand(MaterialHandle i_material, const void* ip_parent_command) const;
		};

		// TODO: global
		GAMECORE_EXPORT extern MaterialManager& g_material_mgr;

	} // Render
} // SDK

#endif