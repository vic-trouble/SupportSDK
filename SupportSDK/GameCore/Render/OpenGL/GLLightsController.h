#ifndef	__GAMECORE_GLLIGHTSCONTROLLER_H__
#define __GAMECORE_GLLIGHTSCONTROLLER_H__

#include "Render/LightsController.h"

namespace SDK
{
	namespace Render
	{
		class GLLightsController : public LightsController
		{
		private:
			bool m_lights_enabled;
			std::vector<Light> m_lights;
			int m_active_lights;

		public:
			GLLightsController();
			virtual void EnableLighting() override;
			virtual void DisableLighting() override;
			virtual bool LightingEnabled() const override;

			virtual void ApplyLights() override;
			virtual void ResetLights() override;

			// returns -1 if system cannot control more lights
			virtual LightHandle AddLight(const Light& i_light) override;
			virtual void RemoveLight(LightHandle i_handle) override;
		};
	}
}

#endif
