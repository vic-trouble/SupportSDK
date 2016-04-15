#ifndef __GAMECODE_INPUTSYSTEM_H__
#define __GAMECODE_INPUTSYSTEM_H__

#include "GameCore/GameCoreAPI.h"

#include "Keyboard.h"

#include "GameCore/Core.h"
#include "GameCore/Input/InputEvent.h"

#include "GlobalObjectBase.h"

#if defined(_USRDLL)
#pragma warning ( push )
#pragma warning( disable : 4251 )
#endif

namespace SDK
{
	class InputSubscriber;
	// need to export class as singleton
	// TODO: remove singlton
	class GAMECORE_EXPORT InputSystem : public GlobalObjectBase
	{
	private:
		std::vector<InputSubscriber*> m_subscribers;
		// TODO: maybe create prioritize queue of subscribers [Chain of Responsibility]
		InputSubscriber* mp_ui_subscriber;

		Keyboard m_keyboard;

	private:
		bool ProcessUI(const InputEvent& i_event);

	public:
		InputSystem();
		~InputSystem();
		void ProcessEvent(const InputEvent& i_event);

		void AddSubscriber(InputSubscriber* ip_subscriber);
		void RemoveSubscriber(InputSubscriber* ip_subscriber);

		void SetUISubscriber(InputSubscriber* ip_subscriber);
		const Keyboard& GetKeyboard() const { return m_keyboard; }
	};

} // SDK

#if defined(_USRDLL)
#pragma warning ( pop )
#endif

#endif