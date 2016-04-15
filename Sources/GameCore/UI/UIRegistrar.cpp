#include "stdafx.h"

#include "UIControlSystem.h"

// types
#include "UIControl.h"
#include "UIScreen.h"
#include "UIButton.h"

namespace SDK {
	namespace UI {
		namespace detail {
			template <typename ControlType>
			UIControlSystem::UIControlAccessor<UIControl> Create()
			{
				return Core::GetGlobalObject<UI::UIControlSystem>()->CreateControl<ControlType>();
			}
			void RegisterBaseUITypes(UIControlSystem& o_system)
			{
				auto& factory = o_system.GetFactory();
				factory.Register("button", &Create<UIButton>);
				factory.Register("screen", &Create<UIScreen>);
			}
		} // detail
	} // UI
} // SDK