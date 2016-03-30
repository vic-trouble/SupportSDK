#ifndef __GAMECORE_APPLICATION_WINDOWS_H__
#define __GAMECORE_APPLICATION_WINDOWS_H__

#include "ApplicationBase.h"

#include <Windows.h>

namespace SDK
{

	class ApplicationWindows : public ApplicationBase
	{
	private:
		bool m_working;

	// win specific
	private:
		HINSTANCE mh_instance;
		HWND			mh_window;

		HDC hDC;
		HGLRC hRC;
		HANDLE hMutex;

	private:
		bool CreateWin32Window(HINSTANCE hInstance);
	public:
		ApplicationWindows(CoreDelegate* ip_delegate);
		virtual ~ApplicationWindows();

	// ApplicationBase
	private:
		virtual void StartInternal() override;
		virtual void StopInternal() override;

		virtual void OnCreateInternal() override;
		virtual void OnTerminateInternal() override;

		virtual void OnCollapseInternal() override;
		virtual void OnActivateInternal() override;

		virtual void RequestShutdownInternal() override;
	};

} // SDK

#endif