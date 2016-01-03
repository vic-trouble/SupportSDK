#include "stdafx.h"

#include "CoreDelegateImpl.h"

#include <GameCore/Core.h>
#include <GameCore/Render/IRenderer.h>
#include <GameCore/Render/HardwareBufferManagerBase.h>
#include <GameCore/Input/inputsystem.h>
#include <GameCore/Render/OpenGL/GlUitlities.h>

// for test
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <time.h>

#include <Math/Matrix4.h>

using namespace SDK;

#pragma comment(lib, "opengl32.lib")
#if defined(_DEBUG)
# pragma comment(lib, "glew32d.lib")
#else
# pragma comment(lib, "glew32.lib")
#endif

#include <GameCore/Applications/ApplicationBase.h>

#include <GameCore/UI/UIControlSystem.h>
#include <string>

#include <Utilities/_Link.h>
#include <Patterns/_Link.h>

namespace Game
{
	typedef int ConnectionHandler;
	typedef int/*index*/ Publisher;
	struct Connection
	{
		ConnectionHandler handler;
		typedef int EventType;
		typedef Publisher EventPublisherHandler;
		
		Connection(EventType, EventPublisherHandler, void (*handlerFunction))
		{

		}
		~Connection() { disconnect(); }
		bool connected() const
		{
			// UI::system.TestConnection(handler);
		}
		void disconnect() {}
	};

	struct Ev : public Event
	{
		std::string name;
		Ev(const std::string& n) : name(n){}
	};

	struct TestHandler
	{
		void Handle(const UI::UIEvent& e)
		{
			std::cout << "Handle";
		}
	};
	TestHandler handler;
	void CoreDelegateImpl::OnCreate()
	{
		UI::g_ui_system.LoadScheme("..\\..\\Resources\\UI\\test.scheme");
		UI::g_ui_system.UnloadScheme("test");
		UI::g_ui_system.SetActiveScheme("test");
		auto scheme_handler = UI::g_ui_system.LoadScheme("..\\..\\Resources\\UI\\TestUIProfile.scheme");				
		UI::g_ui_system.SetActiveScheme(scheme_handler);
		auto accessor = UI::g_ui_system.CreateControl<UI::UIButton>();
		UI::g_ui_system.RemoveControl(accessor.GetHandler());
		auto accessor1 = UI::g_ui_system.CreateControl<UI::UIButton>();

		auto& msg_dsp = UI::g_ui_system.GetMessageDispatcher();		
		msg_dsp.RegisterHandler<TestHandler, const UI::UIEvent&>(handler, &TestHandler::Handle, "my_mega_button1");
		// Connection x = UI::g_ui_system.Subscribe(EventType, EventPublisher, &EventListener);
		//		EventType - specific - int?enum?
		//		EventPublisher - UIControl* - need index (in vector it all stores)
		//		Function
		// x.Test();
		//UI::g_ui_system.Load("..\\..\\Resources\\UI\\test.properties");
	}

	void CoreDelegateImpl::OnTerminate()
	{
	}

	void CoreDelegateImpl::Update(float i_elapsed_time)
	{
	}

	void CoreDelegateImpl::Draw()
	{
	}

} // Game