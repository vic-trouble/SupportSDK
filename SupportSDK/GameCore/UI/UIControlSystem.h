#ifndef	__GAMECORE_UICONTROLSYSTEM_H__
#define __GAMECORE_UICONTROLSYSTEM_H__

#include "../GameCoreAPI.h"
#include "UITypes.h"

#include <Utilities/noncopyable.h>

#include <Patterns/MessageDispatcher/MessageDispatcher.h>

namespace SDK
{
	class InputSystem;

	namespace UI
	{
		class UIScreen;
		class UIControl;

		class UIControlSystem : Utilities::noncopyable
		{
		public:
			template <typename ControlType>
			class UIControlAccessor
			{
			private:
				UIControlHandler m_handler;
				UIControlSystem* mp_owner_system;
			public:
				UIControlAccessor(UIControlSystem* ip_control_system, UIControlHandler i_handler)
					: mp_owner_system(ip_control_system)
					, m_handler(i_handler)
				{}

				// return pointer to control or return nullptr
				ControlType* GetActual() const
				{
					return static_cast<ControlType*>(mp_owner_system->AccessControl(m_handler));
				}
				bool IsControlAlive() const
				{
					return GetActual() != nullptr;
				}
				UIControlHandler GetHandler() const { return m_handler; }
			};
		
		private:
			class UI_InputSubscriber;
			friend class UI_InputSubscriber;

		private:
			
			UIScreen* mp_current_screen;
			UIScreen* mp_prev_screen;
			UIScreen* mp_next_screen;

			typedef std::unique_ptr<UIControl> UIControlPtr;
			typedef std::pair<UIControlHandler, UIControlPtr> UIControlPair;
			std::vector<UIControlPair> m_controls;
			MessageDispatcher m_message_dispatcher;

		public:
			GAMECORE_EXPORT ~UIControlSystem();

			UIControl* AccessControl(UIControlHandler i_handler) const;
			UIControlHandler GetHandlerTo(UIControl* ip_pointer) const;

			template <typename ControlType, typename... Args>
			UIControlAccessor<ControlType> CreateControl(Args... args)
			{
				// Create control
				UIControlPtr p_obj = std::make_unique<ControlType>(args...);
				// get raw pointer and create handler
				UIControl* p_raw = p_obj.get();
				const int index = static_cast<int>(m_controls.size());
				UIControlHandler handler{ index, 0 };
				// push to controls array
				m_controls.push_back(std::make_pair(handler, std::move(p_obj)));
				// initialize this handler - can be done only after adding to m_controls
				p_raw->InitializeThisHandler();
				return UIControlAccessor<ControlType>(this, handler);
			}

			template <typename ControlType, typename... Args>
			UIControlAccessor<ControlType> AppendControl(UIControlHandler i_parent, Args... args)
			{
				UIControlAccessor<ControlType> control_accessor = CreateControl<ControlType>(args...);
				control_accessor.GetActual()->SetParent(i_parent);
				return control_accessor;
			}

			void Update(float i_elapsed_time);
			void Draw();
			GAMECORE_EXPORT void Load(const std::string& i_file_name);
			
			MessageDispatcher& GetMessageDispatcher() { return m_message_dispatcher; }

			void SetInputSystem(InputSystem& i_input_system);
		};

		// TODO: global object
		GAMECORE_EXPORT extern UIControlSystem g_ui_system;
	} // UI
} // SDK

#endif
