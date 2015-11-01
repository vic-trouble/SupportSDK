#include "stdafx.h"

#include <GameCore/Core.h>

#include "CoreDelegateImpl.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
{
	try
	{
		SDK::Core::Run(std::move(std::make_unique<Game::CoreDelegateImpl>()));
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
	return 0;
}
