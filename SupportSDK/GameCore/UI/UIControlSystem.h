#ifndef	__GAMECORE_UICONTROLSYSTEM_H__
#define __GAMECORE_UICONTROLSYSTEM_H__

#include "../GameCoreAPI.h"

#include <Utilities/noncopyable.h>

namespace SDK
{
	namespace UI
	{
		class UIScreen;
		class UIControl;

		class UIControlSystem : Utilities::noncopyable
		{
		private:
			UIScreen* mp_current_screen;
			UIScreen* mp_prev_screen;
			UIScreen* mp_next_screen;

			typedef std::unique_ptr<UIControl> UIControlPtr;
			std::vector<UIControlPtr> m_controls;

		public:
			GAMECORE_EXPORT ~UIControlSystem();

			template <typename ControlType, typename... Args>
			ControlType* CreateControl(Args... args)
			{
				UIControlPtr p_obj = std::make_unique<ControlType>(args...);				
				auto p_raw = p_obj.get();
				m_controls.push_back(std::move(p_obj));
				return static_cast<ControlType*>(p_raw);
			}

			template <typename ControlType, typename... Args>
			ControlType* AppendControl(UIControl* ip_parent, Args... args)
			{
				ControlType* p_control = CreateControl<ControlType>(args...);
				p_control->SetParent(ip_parent);
				return p_control;
			}

			void Update(float i_elapsed_time);
			void Draw();
			GAMECORE_EXPORT void Load(const std::string& i_file_name);
		};

		// TODO: global object
		GAMECORE_EXPORT extern UIControlSystem g_ui_system;
	} // UI
} // SDK

#endif
