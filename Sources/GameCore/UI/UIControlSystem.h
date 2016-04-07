#ifndef	__GAMECORE_UICONTROLSYSTEM_H__
#define __GAMECORE_UICONTROLSYSTEM_H__

#include "../GameCoreAPI.h"
#include "UITypes.h"

#include <Utilities/noncopyable.h>

#include <Patterns/MessageDispatcher/MessageDispatcher.h>
#include <Patterns/Factory/Factory.h>

#include "../GenericHandlesDynamicArray.h"

namespace SDK
{
	class InputSystem;

	namespace Render {
		class RenderWorld;
	} // Render

	namespace UI
	{
		class UIScreen;
		class UIControl;		

		class UIControlSystem;
		// TODO: global object
		GAMECORE_EXPORT extern UIControlSystem g_ui_system;

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
				UIControlAccessor()
					: mp_owner_system(nullptr)
					, m_handler(INVALID_UI_HANDLER)
				{}
				UIControlAccessor(UIControlSystem* ip_control_system, UIControlHandler i_handler)
					: mp_owner_system(ip_control_system)
					, m_handler(i_handler)
				{}
			
				template <typename OtherControl, typename CType = ControlType>
				UIControlAccessor(const UIControlAccessor<OtherControl>& i_other
					, typename std::enable_if< std::is_same<CType, UIControl>::value >::type* = 0)
					: m_handler(i_other.GetHandler())
					, mp_owner_system(i_other.GetSystem())
				{}

				// return pointer to control or return nullptr
				ControlType* GetActual() const
				{
					if (mp_owner_system == nullptr)
						return nullptr;
					return static_cast<ControlType*>(mp_owner_system->AccessControl(m_handler));
				}
				bool IsControlAlive() const
				{
					if (mp_owner_system == nullptr)
						return false;
					return GetActual() != nullptr;
				}
				UIControlHandler GetHandler() const { return m_handler; }
				UIControlSystem* GetSystem() const { return mp_owner_system; }
				bool IsValid() const { return mp_owner_system != nullptr && m_handler != INVALID_UI_HANDLER; }
			};
		
			class UIScheme
			{
				friend class UIControlSystem;
			private:
				std::vector<UIControlHandler> m_handlers;
				std::string m_name;
				size_t m_name_hash;

			public:
				UIScheme()
					: m_name_hash(0)
					, m_name()
				{}

				UIScheme(const std::string& i_name, size_t i_hash)
					: m_name(i_name)
					, m_name_hash(i_hash)
				{}

				void AddControl(UIControlHandler i_handler)
				{
					m_handlers.push_back(i_handler);
				}
				void RemoveControl(UIControlHandler i_handler)
				{
					auto it = std::find(m_handlers.begin(), m_handlers.end(), i_handler);
					if (it != m_handlers.end())
					{
						// if control removed from scheme - it removed from UI system
						g_ui_system.RemoveControl(i_handler);
						m_handlers.erase(it);
					}
				}

				size_t GetHash() const { return m_name_hash; }
				std::string GetName() const { return m_name; }
			};

			typedef Factory<UIControl, std::string, UIControlAccessor<UIControl>> UIFactory;

		private:
			class UI_InputSubscriber;
			friend class UI_InputSubscriber;

		private:
			using UIControls = SDK::GenericHandleDynamicArray<UIControlHandler, std::unique_ptr<UIControl>>;
			UIControls m_controls;

			using UISchemes = SDK::GenericHandleDynamicArray<UISchemeHandler, UIScheme>;
			UISchemes m_schemes;
			UISchemeHandler m_current_scheme;

			MessageDispatcher m_message_dispatcher;
			UIFactory m_factory;
		private:
			UISchemeHandler FindScheme(size_t i_hash);
		
		public:
			UIControlSystem();
			GAMECORE_EXPORT ~UIControlSystem();

			GAMECORE_EXPORT UIControl* AccessControl(UIControlHandler i_handle);
			GAMECORE_EXPORT UIControlHandler GetHandlerTo(UIControl* ip_pointer) const;

			template <typename ControlType, typename... Args>
			UIControlAccessor<ControlType> CreateControl(Args... args)
			{
				auto handle = m_controls.CreateNew<ControlType>(args...);
				UIControl* p_raw = m_controls.Access(handle);
				p_raw->InitializeThisHandler();

				return UIControlAccessor<ControlType>(this, handle);
			}

			template <typename ControlType, typename... Args>
			UIControlAccessor<ControlType> AppendControl(UIControlHandler i_parent, Args... args)
			{
				UIControlAccessor<ControlType> control_accessor = CreateControl<ControlType>(args...);
				control_accessor.GetActual()->SetParent(i_parent);
				return control_accessor;
			}

			GAMECORE_EXPORT void RemoveControl(UIControlHandler i_handler);

			void Update(float i_elapsed_time);
			void Draw(Render::RenderWorld& i_render_world);

			GAMECORE_EXPORT UIControl* FindControlInCurrentScheme(const std::string& i_control_name);

			GAMECORE_EXPORT UISchemeHandler LoadScheme(const std::string& i_file_name);
			GAMECORE_EXPORT void UnloadScheme(UISchemeHandler i_scheme);
			GAMECORE_EXPORT void UnloadScheme(const std::string& i_scheme);

			GAMECORE_EXPORT void SetActiveScheme(const std::string& i_scheme_name);
			GAMECORE_EXPORT void SetActiveScheme(UISchemeHandler i_scheme);
			GAMECORE_EXPORT const UIScheme* AccessScheme(UISchemeHandler i_scheme) const;

			MessageDispatcher& GetMessageDispatcher() { return m_message_dispatcher; }
			UIFactory& GetFactory() { return m_factory; }

			void SetInputSystem(InputSystem& i_input_system);
			void OnResize(const IRect& i_new_size);
		};

	} // UI
} // SDK

#endif
