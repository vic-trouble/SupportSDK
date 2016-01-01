#ifndef __GAMECORE_UITYPES_H__
#define	__GAMECORE_UITYPES_H__

#include "../GenericHandle.h"


#include <Patterns/MessageDispatcher/Event.h>
namespace SDK
{
	namespace UI
	{
		
		struct UIControlHandlerTag {};
		typedef GenericHandle<int, 12, 20, UIControlHandlerTag> UIControlHandler;

		constexpr static UIControlHandler INVALID_UI_HANDLER = { -1, -1 };

		/////////////////////////////////////////////////////////////////////////////
		// Events
		enum class UIEventType
		{
			ButtonPressed
		};
		struct UIEvent : public Event
		{
			UIEventType m_ev_type;
			UIEvent(UIEventType t) : m_ev_type(t) {}
		};
	} // UI
} // SDK

#endif
