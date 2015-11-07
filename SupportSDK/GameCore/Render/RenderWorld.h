#ifndef __GAMECORE_RENDERWORLD_H__
#define __GAMECORE_RENDERWORLD_H__

#include "../GameCoreAPI.h"

#include "RenderTypes.h"

#include "RenderBucket.h"
#include "Viewport.h"

namespace SDK
{

	namespace Render
	{
		// one render world contains render queus;
		//	wraps submition process
		class RenderWorld
		{
			// TODO: TEMPORARY public for testing!!!!!!!!!!!
		public:
			// TODO: vector or another structure?
			std::vector<RenderBucket> m_buckets;



		public:
			RenderWorld();
			// end command
			GAMECORE_EXPORT void Submit(const Viewport& i_viewport);
			GAMECORE_EXPORT void AddCommand(Batch&& i_batch);
		};

	}

} // SDK

#endif