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

namespace Game
{
	void CoreDelegateImpl::OnCreate()
	{
		mp_button = UI::g_ui_system.AppendControl<UI::UIButton>(mp_screen);
		UI::g_ui_system.Load("..\\..\\Resources\\UI\\TestUIProfile.properties");
		
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