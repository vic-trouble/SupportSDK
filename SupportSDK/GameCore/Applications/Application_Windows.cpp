#include "stdafx.h"

#include "Application_Windows.h"

#include "Core.h"
#include "CoreDelegate.h"

#include "Renderer/OpenGLRenderer.h"
#include "Input/InputSystem.h"

#include <Windows.h>
#include <time.h>


/////////////////////////////////////////////////////////////////

namespace
{

	void RegisterApplicationClass(HINSTANCE ih_instance, WNDPROC i_wnd_proc, LPCWSTR i_class_name)
	{
		// Register Class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = i_wnd_proc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = ih_instance;
		wcex.hIcon = 0;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = 0;
		wcex.lpszClassName = i_class_name;
		wcex.hIconSm = 0;

		RegisterClassEx(&wcex);
	}

	LARGE_INTEGER liFrequency;
	SDK::uint64 GetAbsoluteMS()
	{
		LARGE_INTEGER liCounter;
		QueryPerformanceCounter(&liCounter);
		return static_cast<SDK::uint64>(((double)(liCounter.QuadPart)) / (double)liFrequency.QuadPart * 1000.);
	}


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#ifndef WHEEL_DELTA                     
#define WHEEL_DELTA 120
#endif
		switch (message)
		{
			case WM_ERASEBKGND:
				return 0;

			case WM_KEYUP:
				{
					SDK::InputSystem::Instance().ProcessEvent(SDK::KeyEvent((SDK::int32)wParam, SDK::KeyState::Released));
				}
				break;

			case WM_KEYDOWN:
				{
					SDK::InputSystem::Instance().ProcessEvent(SDK::KeyEvent((SDK::int32)wParam, SDK::KeyState::Pressed));
				}
				break;
				
			case WM_CHAR:
				{
				}
				break;

			case WM_INPUT:
				{
				}
				break;


			case WM_MOUSEMOVE:
				break;

			case WM_NCMOUSEMOVE:
				break;

			case WM_NCMOUSELEAVE:
				break;


			case WM_ACTIVATE:
				{
				}
				break;

			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;

		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

}

/////////////////////////////////////////////////////////////////

namespace SDK
{
	namespace impl
	{

		void InitializeApplication(CoreDelegate* ip_delegate, std::function<void(std::unique_ptr<ApplicationBase>)> i_set_callback)
		{
			std::unique_ptr<ApplicationBase> p_application(new ApplicationWindows(ip_delegate));

			ApplicationBase* p_win_app = p_application.get();

			i_set_callback(std::move(p_application));
			
			p_win_app->OnCreate();
			p_win_app->OnActivate();
			p_win_app->Start();
			p_win_app->OnCollapse();
			p_win_app->OnTerminate();			

			Core::DestroyApplication();
		}

	} // impl

	ApplicationWindows::ApplicationWindows(CoreDelegate* ip_delegate)
		: ApplicationBase(ip_delegate)
	{}

	ApplicationWindows::~ApplicationWindows()
	{}

	bool ApplicationWindows::CreateWin32Window(HINSTANCE ih_instance)
	{
		this->mh_instance = ih_instance;
		return true;
	}

	const int WINDOWED_STYLE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	void ApplicationWindows::OnCreateInternal()
	{
		CONTENT_WIDTH = 640;
		CONTENT_HEIGHT = 960;

		HINSTANCE instance = ::GetModuleHandle(NULL);
		const bool windowCreated = CreateWin32Window(instance);
		if (!windowCreated)
			throw std::runtime_error("Window cannot be created.");

		//single instance check
		TCHAR fileName[MAX_PATH];
		GetModuleFileName(NULL, fileName, MAX_PATH);
		fileName[MAX_PATH - 1] = 0; //string can be not null-terminated on winXP
		for (int i = 0; i < MAX_PATH; ++i)
		{
			if (fileName[i] == L'\\') //symbol \ is not allowed in CreateMutex mutex name
				fileName[i] = ' ';
		}

		SetLastError(0);

		LPCWSTR class_name = L"Tetris";
		RegisterApplicationClass(instance, (WNDPROC)WndProc, class_name);

		// calculate client size
		RECT clientSize;
		clientSize.top = 0;
		clientSize.left = 0;
		clientSize.right = CONTENT_WIDTH;
		clientSize.bottom = CONTENT_HEIGHT;

		ULONG style = WINDOWED_STYLE | WS_CLIPCHILDREN;
		AdjustWindowRect(&clientSize, style, FALSE);

		int realWidth = clientSize.right - clientSize.left;
		int realHeight = clientSize.bottom - clientSize.top;

		int windowLeft = -10000;
		int windowTop = -10000;

		// create window
		mh_window = CreateWindow(class_name, L"", style, windowLeft, windowTop,
			realWidth, realHeight, NULL, NULL, mh_instance, NULL);

		ShowWindow(mh_window, SW_SHOW);
		UpdateWindow(mh_window);

		// fix ugly ATI driver bugs. Thanks to ariaci (Taken from Irrlight).
		MoveWindow(mh_window, windowLeft, windowTop, realWidth, realHeight, TRUE);
		SetWindowText(mh_window, class_name);

		// move window
		clientSize.top = 0;
		clientSize.left = 0;
		clientSize.right = CONTENT_WIDTH;
		clientSize.bottom = CONTENT_HEIGHT;

		AdjustWindowRect(&clientSize, style, FALSE);

		realWidth = clientSize.right - clientSize.left;
		realHeight = clientSize.bottom - clientSize.top;

		windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
		windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;
		MoveWindow(mh_window, windowLeft, windowTop, realWidth, realHeight, TRUE);

		// TODO initialize renderer
		Core::SetupRenderer(std::unique_ptr<IRenderer>(new OpenGLRenderer(mh_window, IRect())));
		auto p_renderer = Core::GetRenderer();
		p_renderer->Initialize();
		p_renderer->Reshape();

		RAWINPUTDEVICE Rid[2];
		// Keyboard
		Rid[0].usUsagePage = 0x01;
		Rid[0].usUsage = 0x06;
		Rid[0].dwFlags = 0;
		Rid[0].hwndTarget = NULL;
		// Mouse
		Rid[1].usUsagePage = 0x01;
		Rid[1].usUsage = 0x02;
		Rid[1].dwFlags = 0;
		Rid[1].hwndTarget = NULL;

		if (RegisterRawInputDevices(Rid, 2, sizeof(RAWINPUTDEVICE)) == FALSE)
			throw std::runtime_error("Cannot register input devices");

		// initialize timer
		QueryPerformanceFrequency(&liFrequency);
	}

	void ApplicationWindows::OnTerminateInternal()
	{
		
	}

	void ApplicationWindows::OnCollapseInternal()
	{

	}

	void ApplicationWindows::OnActivateInternal()
	{

	}

	void ApplicationWindows::RequestShutdownInternal()
	{
		m_working = false;
	}

	void ApplicationWindows::StartInternal()
	{
		m_working = true;

		MSG msg;
		while (m_working)
		{
			SDK::uint64 start_time = GetAbsoluteMS();
			// process messages
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT)
				{
					/*ApplicationCore * appCore = Core::Instance()->GetApplicationCore();
					if (appCore && appCore->OnQuit())
					{
					exit(0);
					}
					else
					{
					m_working = true;
					}*/
					m_working = false;
				}
			} // peek message cycle


			SDK::uint64 elapsed_time = GetAbsoluteMS() - start_time;
			Core::Update(elapsed_time*1.f);
			Core::Draw();
		} // main cycle
	}

	void ApplicationWindows::StopInternal()
	{

	}

	void ApplicationWindows::UpdateInternal(float i_elapsed_time)
	{

	}

	void ApplicationWindows::DrawInternal()
	{

	}

} // SDK

