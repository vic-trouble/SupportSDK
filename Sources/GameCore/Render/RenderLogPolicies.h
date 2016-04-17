#ifndef	__GAMECORE_RENDERLOGPOLICIES_H__
#define __GAMECORE_RENDERLOGPOLICIES_H__

#include "../GameCoreAPI.h"

#include "../Logging/BasicPolicies.h"

#include <Utilities/HashFunctions.h>

namespace SDK
{
	namespace Render
	{
		using RenderFilterPolicy = Log::ChannelFilterPolicy<Utilities::hash_str("Render")>;

		using RenderVSLogger = Log::LoggerImpl<RenderFilterPolicy, Log::ExtendedFormatPolicy, Log::IDEWriter>;


	} // Render
} // SDK

#endif