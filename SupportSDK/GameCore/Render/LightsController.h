#ifndef	__GAMECORE_LIGHTSCONTROLLER_H__
#define __GAMECORE_LIGHTSCONTROLLER_H__

#include "RenderTypes.h"

namespace SDK
{
	namespace Render
	{
		// base class - implementations depends on API - Gl/DirectX
		class LightsController
		{
		public:
			virtual void EnableLighting() = 0;
			virtual void DisableLighting() = 0;
			virtual bool LightingEnabled() const = 0;

			virtual void ApplyLights() = 0;
			virtual void ResetLights() = 0;

			// returns -1 if system cannot control more lights
			virtual LightHandle AddLight(const Light& i_light) = 0;
			virtual void RemoveLight(LightHandle i_handle) = 0;
		};
	}
}

#endif
