#ifndef	__GAMECORE_WORLD_H__
#define __GAMECORE_WORLD_H__

#include "System.h"

#include "Render/Viewport.h"
#include "Render/Camera.h"
#include "Render/Frustum.h"

#include <Utilities/noncopyable.h>

namespace SDK
{
	
	class World : Utilities::noncopyable
	{
	private:
		typedef std::unique_ptr<System> SystemPtr;
		std::vector<System*> m_systems;

		//Render::Viewport m_viewport;

		// TODO: !!!TEMPORARY public
	public:
		Render::Viewport m_viewport;
		Render::Camera m_camera;
		Render::Frustum m_frustum;

	public:
		World();
		void Update(float i_elapsed_time);
		void SubmitDrawCommands();

		const Render::Viewport& GetViewPort() const { return m_viewport; }
		void RegisterSystem(System* ip_system) { m_systems.push_back(ip_system); }
		void ClearSystems() { m_systems.clear(); }
	};

}

#endif