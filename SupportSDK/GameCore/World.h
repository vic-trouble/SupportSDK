#ifndef	__GAMECORE_WORLD_H__
#define __GAMECORE_WORLD_H__

#include "Render/Viewport.h"
#include "Render/Camera.h"
#include "Render/Frustum.h"

namespace SDK
{
	struct System
	{
		enum class Action
		{
			Update,
			SubmitDrawCommands,			
		};

		virtual void Update(float i_elapsed_time){}
		virtual void SubmitDrawCommands(){}

		virtual bool Requires(Action i_action) const { return false; }
	};
	class World
	{
		// TODO: !!!TEMPORARY public
	public:
		System m_systems[10];

		Render::Viewport m_viewport;
		Render::Camera m_camera;
		Render::Frustum m_frustum;

	public:
		World();
		void Update(float i_elapsed_time);
		void SubmitDrawCommands();

		const Render::Viewport& GetViewPort() const { return m_viewport; }
	};

}

#endif