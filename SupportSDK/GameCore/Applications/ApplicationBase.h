#ifndef __GAMECORE_APPLICATIONBASE_H__
#define __GAMECORE_APPLICATIONBASE_H__

#include "../World.h"
#include "../Render/RenderWorld.h"

namespace SDK
{
	class CoreDelegate;

	class ApplicationBase
	{
	protected:
		int CONTENT_WIDTH;
		int CONTENT_HEIGHT;

		CoreDelegate* mp_delegate;

		int m_fps;
		
		World				m_worlds[1];
		Render::RenderWorld m_render_world;
	private:
		virtual void StartInternal() = 0;
		virtual void StopInternal() = 0;

		virtual void OnCreateInternal() = 0;
		virtual void OnTerminateInternal() = 0;

		virtual void OnCollapseInternal() = 0;
		virtual void OnActivateInternal() = 0;

		virtual void UpdateInternal(float i_elapsed_time) = 0;
		virtual void DrawInternal() = 0;

		virtual void RequestShutdownInternal() = 0;
	public:
		World& GetWorld() { return m_worlds[0]; }
		Render::RenderWorld& GetRenderWorld() { return m_render_world; }
		ApplicationBase(CoreDelegate* ip_delegate)
			: mp_delegate(ip_delegate)
			, CONTENT_WIDTH(0)
			, CONTENT_HEIGHT(0)
			, m_fps(60)
		{}
		virtual ~ApplicationBase(){}

		void Start();
		void Stop();

		void OnCreate();
		void OnTerminate();

		void OnCollapse();
		void OnActivate();

		void RequestShutdown();

		void SetMaxFPS(int i_fps) { m_fps = i_fps; }

		void Update(float i_elapsed_time);
		void Draw();
	};

} // SDK

#endif