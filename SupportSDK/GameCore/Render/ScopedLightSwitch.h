#ifndef	__GAMECORE_SCOPEDLIGHTSWITCH_H__
#define __GAMECORE_SCOPEDLIGHTSWITCH_H__

#include "LightsController.h"
#include <Utilities/CustomScopedObject.h>

namespace SDK
{
	namespace Render
	{
		struct ScopedLight {};

		template <>
		struct ScopedHelper <ScopedLight, LightsController*>
		{
			static bool Push(LightsController* ip_light)
			{
				return ip_light->LightingEnabled();
			}

			static void Pop(bool i_was_enabled, LightsController* ip_light)
			{
				if (i_was_enabled)
					ip_light->EnableLighting();
				else
					ip_light->DisableLighting();
			}
		};

		using ScopedLightSwitch = CustomScopedObject<ScopedLight, LightsController*>;

	} // Render
} // SDK

#endif
