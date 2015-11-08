#include "stdafx.h"

#include "ApplicationBase.h"

#include "CoreDelegate.h"
#include "Core.h"

#include "Render/IRenderer.h"
#include "Systems/MeshSystem.h"

#include <Utilities/TimeUtilities.h>

namespace SDK
{
	void ApplicationBase::Start()
	{
		StartInternal();
	}

	void ApplicationBase::Stop()
	{
		StopInternal();
	}

	void ApplicationBase::OnCreate()
	{
		OnCreateInternal();
		// TODO: register systems in constructor?
		m_worlds[0].RegisterSystem(&Render::g_mesh_system);
		mp_delegate->OnCreate();
	}

	void ApplicationBase::OnTerminate()
	{
		OnTerminateInternal();
		for (auto& world : m_worlds)
			world.ClearSystems();

		mp_delegate->OnTerminate();
	}

	void ApplicationBase::OnCollapse()
	{
		OnCollapseInternal();
		mp_delegate->OnCollapse();
	}

	void ApplicationBase::OnActivate()
	{
		OnActivateInternal();
		mp_delegate->OnActivate();
	}

	void ApplicationBase::RequestShutdown()
	{
		RequestShutdownInternal();
	}

	void ApplicationBase::Update(float i_elapsed_time)
	{
		UpdateInternal(i_elapsed_time);		

		mp_delegate->Update(i_elapsed_time);

		for (World& world : m_worlds)
			world.Update(i_elapsed_time);

		int sleep_ms = 1;
		const float frame_time = 1000.f / m_fps;
		sleep_ms = static_cast<int>(frame_time - i_elapsed_time);
		if (sleep_ms < 1)
			sleep_ms = 1;
		Utilities::SleepMiliseconds(sleep_ms);
	}

	void ApplicationBase::Draw()
	{
		Core::GetRenderer()->BeginFrame();
		DrawInternal();
		mp_delegate->Draw();
		for (World& world : m_worlds)
		{
			world.SubmitDrawCommands();
			m_render_world.Submit(world.GetViewPort());
		}

		
		Core::GetRenderer()->EndFrame();
	}

} // SDK