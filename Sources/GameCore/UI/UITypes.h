#ifndef __GAMECORE_UITYPES_H__
#define	__GAMECORE_UITYPES_H__

#include "../GenericHandle.h"


#include <Patterns/Event.h>
namespace SDK
{
	namespace UI
	{
		
		struct UIControlHandlerTag {};
		typedef GenericHandle<int, 12, 20, UIControlHandlerTag> UIControlHandler;

		constexpr static UIControlHandler INVALID_UI_HANDLER = { -1, -1 };

		struct UISchemeHandlerTag {};
		typedef GenericHandle<int, 12, 20, UISchemeHandlerTag> UISchemeHandler;

		constexpr static UISchemeHandler INVALID_UISCHEME_HANDLER = { -1, -1 };

		/////////////////////////////////////////////////////////////////////////////
		// Events
		
		struct UIButtonEvent : public Event
		{
			enum class Type : char
			{
				Pressed,
				Released
			} m_type;

			UIButtonEvent(Type i_type)
				: m_type(i_type)
			{}
		};

	} // UI
} // SDK

#endif
