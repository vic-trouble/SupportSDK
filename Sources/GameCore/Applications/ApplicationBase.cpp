#include "stdafx.h"

#include "ApplicationBase.h"

#include "CoreDelegate.h"
#include "Core.h"

#include "Render/IRenderer.h"
#include "Render/MeshSystem.h"

#include "UI/UIControlSystem.h"

#include <Utilities/TimeUtilities.h>

namespace SDK
{
	namespace Resources
	{
		extern void RegisterLoaders();
		extern void ReleaseLoaders();
	} // Resources
	namespace GlobalObjects
	{
		extern void RegisterDefaultGlobalObjects();
	}

	

	void ApplicationBase::Start()
	{
		m_fps_counter = 0;
		m_fps_timer = 0;
		m_current_fps = 0;
		StartInternal();
	}

	void ApplicationBase::Stop()
	{
		StopInternal();
	}

	void ApplicationBase::OnCreate()
	{
		OnCreateInternal();
		Resources::RegisterLoaders();		
		// TODO: register systems in constructor?
		m_world.RegisterSystem(Core::GetGlobalObject<Render::MeshSystem>());
		mp_delegate->OnCreate();
	}

	void ApplicationBase::OnTerminate()
	{
		OnTerminateInternal();
		m_world.ClearSystems();

		mp_delegate->OnTerminate();

		Resources::ReleaseLoaders();
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
		Core::GetGlobalObject<UI::UIControlSystem>()->Update(i_elapsed_time);

		mp_delegate->Update(i_elapsed_time);

		m_world.Update(i_elapsed_time);

		m_fps_timer += i_elapsed_time;
		if (m_fps_timer >= 1.f)
		{
			m_fps_timer = 0.f;
			m_current_fps = m_fps_counter;
			m_fps_counter = 0;
		}

		++m_fps_counter;
	}

	void ApplicationBase::Draw()
	{
		Core::GetRenderer()->BeginFrame();

		m_world.SubmitDrawCommands(m_render_world);
		m_render_world.Submit({
			m_world.GetFrustum().GetProjectionType(),
			m_world.GetFrustum().GetProjectionMatrix(),
			m_world.GetCamera().GetModelViewMatrix()
		});
		mp_delegate->Draw();
		Core::GetGlobalObject<UI::UIControlSystem>()->Draw(m_render_world);

		Core::GetRenderer()->EndFrame();
	}

	void ApplicationBase::OnResize(const IRect& i_rect)
	{
		Core::GetGlobalObject<UI::UIControlSystem>()->OnResize(i_rect);
	}

} // SDK