#ifndef	__GAMECORE_ENTITYDEFINES_H__
#define __GAMECORE_ENTITYDEFINES_H__

#include "GenericHandle.h"
#include "ObjectSubset.h"

#include <Utilities/HashFunctions.h>

namespace SDK
{

	// 70368744177663 indices; 65535 generations
	// TODO: this is too many; no?	
	struct EntityTag {};
	typedef GenericHandle<int64, 47, 17, EntityTag> EntityHandle;

	using EntitySubset = ObjectSet<EntityHandle>;


#define ENTITY_DEFINITION(ProcessorSystem_, ComponentName) \
	typedef ProcessorSystem_ ProcessorSystem; \
	static constexpr int ID = static_cast<int>(SDK::Utilities::hash_str(ComponentName));

} // SDK

#endif
