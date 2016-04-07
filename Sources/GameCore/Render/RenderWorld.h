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
		using RenderCommandBucket = Commands::CommandBucket<int>;
		// one render world contains render queus;
		//	wraps submition process
		enum class BucketType
		{
			Buffer = 0,
			Lighting = 1,
			Deferred = 2,
			PostProcessing = 3,
			UI = 4,

			Count
		};
		class RenderWorld : Utilities::noncopyable
		{
		private:
			// TODO: need different buffers for different conditions: 
			//	thread specific
			//	different jobs: buffer bucket; lighting; deferred; postprocessing; UI
			RenderCommandBucket m_command_buckets[static_cast<int>(BucketType::Count)];

		public:
			RenderWorld();
			// end command
			GAMECORE_EXPORT void Submit(Viewport&& i_viewport);

			Commands::CommandBucket<int>* const GetBucket(BucketType i_type)
			{
				// TODO: should be thread specific?
				const int index = static_cast<int>(i_type);
				assert(index >= 0 && index < static_cast<int>(BucketType::Count));
				return &m_command_buckets[index];
			}
		};

	}

} // SDK

#endif