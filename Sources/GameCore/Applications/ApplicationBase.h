#ifndef __GAMECORE_APPLICATIONBASE_H__
#define __GAMECORE_APPLICATIONBASE_H__

#include "../GameCoreAPI.h"

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
		int m_current_fps;
		int m_fps_counter;
		float m_fps_timer;
		
		World				m_world;
		Render::RenderWorld m_render_world;

	private:
		// methods that depends on platform
		virtual void StartInternal() = 0;
		virtual void StopInternal() = 0;

		virtual void OnCreateInternal() = 0;
		virtual void OnTerminateInternal() = 0;

		virtual void OnCollapseInternal() = 0;
		virtual void OnActivateInternal() = 0;

		virtual void RequestShutdownInternal() = 0;

	public:
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

		GAMECORE_EXPORT void RequestShutdown();

		void SetMaxFPS(int i_fps) { m_fps = i_fps; }
		int GetCurrentFPS() const { return m_current_fps; }

		void Update(float i_elapsed_time);
		void Draw();

		int GetHeight() const { return CONTENT_HEIGHT; }
		int GetWidth() const { return CONTENT_WIDTH; }

		World& GetWorld() { return m_world; }
		Render::RenderWorld& GetRenderWorld() { return m_render_world; }

		void OnResize(const IRect& i_rect);
	};

} // SDK

#endif