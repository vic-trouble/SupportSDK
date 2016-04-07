#ifndef	__GAMECORE_SYSTEM_H__
#define __GAMECORE_SYSTEM_H__

namespace SDK
{
	namespace Render {
		class RenderWorld;
	} // Render

	class System
	{
	public:
		/*
		Default set of actions.
		For user needs - can extend
		TODO: Extend with what? if there is something - make enum Action or int
		*/
		enum class Action
		{
			Update,
			SubmitDrawCommands,
		};

	public:
		virtual ~System(){}
		
		virtual void Update(float i_elapsed_time) {}
		virtual void SubmitDrawCommands(Render::RenderWorld& i_render_world) {}

		virtual bool Requires(Action i_action) const { return false; }
	};

} // SDK

#endif