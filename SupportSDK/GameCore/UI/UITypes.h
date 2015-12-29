#ifndef __GAMECORE_UITYPES_H__
#define	__GAMECORE_UITYPES_H__

#include "../GenericHandle.h"

namespace SDK
{
	namespace UI
	{
		
		struct UIControlHandlerTag {};
		typedef GenericHandle<int, 12, 20, UIControlHandlerTag> UIControlHandler;

		constexpr static UIControlHandler INVALID_UI_HANDLER = { -1, -1 };

	} // UI
} // SDK

#endif
