#ifndef	__GAMECORE_WORLD_H__
#define __GAMECORE_WORLD_H__

#include "System.h"

#include "Render/Camera.h"
#include "Render/Frustum.h"

#include <Utilities/noncopyable.h>

namespace SDK
{
	
	class World : Utilities::noncopyable
	{
	private:
		std::vector<System*> m_systems;

		Render::Camera m_camera;
		Render::Frustum m_frustum;

	public:
		World();
		void Update(float i_elapsed_time);
		void SubmitDrawCommands();

		void RegisterSystem(System* ip_system) { m_systems.push_back(ip_system); }
		void UnregisterSystem(System* ip_system)
		{
			auto it = std::find(m_systems.begin(), m_systems.end(), ip_system);
			if (it != m_systems.end())
				m_systems.erase(it);
		}
		void ClearSystems() { m_systems.clear(); }

		Render::Camera& GetCamera() { return m_camera; }
		Render::Frustum& GetFrustum() { return m_frustum; }
	};

}

#endif