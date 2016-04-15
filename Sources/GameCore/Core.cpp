#include "stdafx.h"

#include "Core.h"

#include "CoreDelegate.h"
#include "Applications/ApplicationBase.h"
#include "Render/IRenderer.h"

// singletons
#include "Input/InputSystem.h"

#include "UI/UIControlSystem.h"

// global object getter
#include "DefaultObjectsStorage.h"

namespace SDK
{

	namespace impl
	{
		extern void InitializeApplication(CoreDelegate* ip_delegate, std::function<void(std::unique_ptr<ApplicationBase>)> i_set_callback);
	} // impl

	namespace GlobalObjects
	{
		void RegisterDefaultGlobalObjects();
		void ClearDefaultGlobalObjects();
	}

	std::unique_ptr<CoreDelegate> Core::mp_delegate = nullptr;
	std::unique_ptr<ApplicationBase> Core::mp_application = nullptr;
	std::unique_ptr<IRenderer> Core::mp_renderer = nullptr;
	std::unique_ptr<ObjectStorageBase> Core::mp_go_storage = nullptr;
	Options Core::m_options;

	void Core::CreateSingletons()
	{
		GlobalObjects::RegisterDefaultGlobalObjects();
	}

	void Core::ReleaseSingletons()
	{
		GlobalObjects::ClearDefaultGlobalObjects();
	}

	void Core::SetApplication(std::unique_ptr<ApplicationBase> ip_application)
	{
		mp_application = std::move(ip_application);
	}

	void Core::Run(std::unique_ptr<CoreDelegate>&& ip_delegate)
	{
		SetGlobalObjectStorage(std::make_unique<DefaultObjectsStorage>());
		CreateSingletons();
		
		Core::GetGlobalObject<UI::UIControlSystem>()->SetInputSystem(*Core::GetGlobalObject<InputSystem>());

		mp_delegate = std::move(ip_delegate);

		// init app that will be call delegate methods
		impl::InitializeApplication(mp_delegate.get(), &Core::SetApplication);
	}

	CoreDelegate* Core::GetDelegate()
	{
		assert(mp_delegate);
		return mp_delegate.get();
	}

	ApplicationBase* Core::GetApplication()
	{
		assert(mp_application);
		return mp_application.get();
	}

	void Core::SetupRenderer(std::unique_ptr<IRenderer> ip_renderer)
	{
		mp_renderer = std::move(ip_renderer);
	}

	IRenderer* Core::GetRenderer()
	{
		assert(mp_renderer);
		return mp_renderer.get();
	}

	void Core::DestroyApplication()
	{
		mp_application->Stop();
		mp_delegate.reset();
		mp_renderer.reset();
		mp_application.reset();

		ReleaseSingletons();
	}

	void Core::Update(float i_elapsed_time)
	{
		if (!mp_application)
			return;

		mp_application->Update(i_elapsed_time);
	}

	void Core::Draw()
	{
		if (!mp_application)
			return;

		mp_application->Draw();
	}

} // SDK