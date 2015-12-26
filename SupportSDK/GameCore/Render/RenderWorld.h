#ifndef __GAMECORE_RENDERWORLD_H__
#define __GAMECORE_RENDERWORLD_H__

#include "../GameCoreAPI.h"

#include "RenderTypes.h"

#include "CommandBucket.h"
#include "Viewport.h"

#include <Utilities/noncopyable.h>

namespace SDK
{

	namespace Render 
	{
		// TODO: what type of global objects
		extern Commands::CommandBucket<int64> gBuffer;
		// one render world contains render queus;
		//	wraps submition process
		class RenderWorld : Utilities::noncopyable
		{
		public:
			RenderWorld();
			// end command
			GAMECORE_EXPORT void Submit(Viewport&& i_viewport);
		};

	}

} // SDK

#endif