#ifndef __GAMECODE_INPUTSYSTEM_H__
#define __GAMECODE_INPUTSYSTEM_H__

#include "GameCore/GameCoreAPI.h"

#include "GameCore/Core.h"
#include "GameCore/Input/InputEvent.h"

#include <Patterns/Singleton.h>

#if defined(_USRDLL)
#pragma warning ( push )
#pragma warning( disable : 4251 )
#endif

namespace SDK
{
	class InputSubscriber;

	class InputSystem : public Singleton<InputSystem>
	{
		friend void Core::CreateSingletons();
	private:
		GAMECORE_EXPORT InputSystem();
		GAMECORE_EXPORT virtual ~InputSystem();

	private:
		std::vector<InputSubscriber*> m_subscribers;
		// TODO: maybe create prioritize queue of subscribers [Chain of Responsibility]
		InputSubscriber* mp_ui_subscriber;

	private:
		GAMECORE_EXPORT bool ProcessUI(const InputEvent& i_event);

	public:
		GAMECORE_EXPORT void ProcessEvent(const InputEvent& i_event);

		GAMECORE_EXPORT void AddSubscriber(InputSubscriber* ip_subscriber);
		GAMECORE_EXPORT void RemoveSubscriber(InputSubscriber* ip_subscriber);

		GAMECORE_EXPORT void SetUISubscriber(InputSubscriber* ip_subscriber);
	};

} // SDK

#if defined(_USRDLL)
#pragma warning ( pop )
#endif

#endif