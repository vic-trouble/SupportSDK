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
		
			class UIScheme
			{
				friend class UIControlSystem;
			private:
				std::vector<UIControlHandler> m_handlers;
				std::string m_name;
				size_t m_name_hash;
				UISchemeHandler m_handler;

			public:
				UIScheme()
					: m_name_hash(0)
					, m_name()
					, m_handler{ -1, -1 }
				{}

				UIScheme(const std::string& i_name, size_t i_hash, UISchemeHandler i_handler)
					: m_name(i_name)
					, m_name_hash(i_hash)
					, m_handler(i_handler)
				{}

				void AddControl(UIControlHandler i_handler)
				{
					m_handlers.push_back(i_handler);
				}
				void RemoveControl(UIControlHandler i_handler)
				{
					auto it = std::find(m_handlers.begin(), m_handlers.end(), i_handler);
					if (it != m_handlers.end())
						m_handlers.erase(it);
				}

				size_t GetHash() const { return m_name_hash; }
				std::string GetName() const { return m_name; }
				UISchemeHandler GetHandler() const { return m_handler; }
			};

		private:
			class UI_InputSubscriber;
			friend class UI_InputSubscriber;

		private:
			typedef std::unique_ptr<UIControl> UIControlPtr;
			typedef std::pair<UIControlHandler, UIControlPtr> UIControlPair;
			typedef std::vector<UIControlPair> UIControls;
			UIControls m_controls;

			std::vector<UIScheme> m_schemes;
			UISchemeHandler m_current_scheme;

			MessageDispatcher m_message_dispatcher;

		private:
			UISchemeHandler FindScheme(size_t i_hash);

		public:
			UIControlSystem();
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
			GAMECORE_EXPORT UISchemeHandler LoadScheme(const std::string& i_file_name);
			void SetActiveScheme(UISchemeHandler i_scheme) { m_current_scheme = i_scheme; }
			GAMECORE_EXPORT void SetActiveScheme(const std::string& i_scheme_name);
			
			MessageDispatcher& GetMessageDispatcher() { return m_message_dispatcher; }

			void SetInputSystem(InputSystem& i_input_system);
			void OnResize(const IRect& i_new_size);
		};

		// TODO: global object
		GAMECORE_EXPORT extern UIControlSystem g_ui_system;
	} // UI
} // SDK

#endif
