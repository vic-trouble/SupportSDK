#include "stdafx.h"

#include "UIControlSystem.h"
#include "UIControl.h"

#include "UIScreen.h"
#include "UIButton.h"

#include "Core.h"
#include "Applications/ApplicationBase.h"
#include "Render/RenderWorld.h"

#include "PropertyReaders.h"

#include "Input/InputSystem.h"
#include "Input/InputSubscriber.h"

namespace SDK
{
	namespace UI
	{
		UIControlSystem g_ui_system;

		class UIControlSystem::UI_InputSubscriber : public InputSubscriber
		{
		private:
			UIControlSystem& m_system;

		public:
			UI_InputSubscriber(UIControlSystem& i_system)
				: m_system(i_system)
			{
				InputSystem::Instance().SetUISubscriber(this);
			}
			virtual ~UI_InputSubscriber()
			{
				InputSystem::Instance().SetUISubscriber(nullptr);
			}

			virtual bool KeyPressed(const KeyEvent& i_evt) override
			{
				return false;
			}
			virtual bool KeyReleased(const KeyEvent& i_evt) override
			{
				return false;
			}

			virtual bool MouseMoved(const MouseEvent& i_evt) override
			{
				return false;
			}
			virtual bool MousePressed(const MouseEvent& i_evt) override
			{
				auto& controls = g_ui_system.m_controls;
				for (auto& control : controls)
				{
					if (control.second->IsHited({ i_evt.m_x, i_evt.m_y }))
					{
						// TODO: need to return true/false based on HandleMessage result
						g_ui_system.GetMessageDispatcher().HandleMessage(UIEvent{ UIEventType::ButtonPressed }, control.second->GetName());
						return true;
					}
				}
				return false;
			}
			virtual bool MouseReleased(const MouseEvent& i_evt) override
			{
				return false;
			}
		};		

		UIControlSystem::~UIControlSystem()
		{
			for (auto& control : m_controls)
				control.second->SetParent(INVALID_UI_HANDLER);
			m_controls.clear();
		}

		UIControlHandler UIControlSystem::GetHandlerTo(UIControl* ip_pointer) const
		{
			auto it = std::find_if(m_controls.begin(), m_controls.end(), [ip_pointer](const UIControlPair& control)
			{
				return control.second.get() == ip_pointer;
			});
			if (it == m_controls.end())
				return INVALID_UI_HANDLER;
			return it->first;
		}

		UIControl* UIControlSystem::AccessControl(UIControlHandler i_handler) const
		{
			if (i_handler.index == -1 || static_cast<int>(m_controls.size()) <= i_handler.index)
				return nullptr;
			if (m_controls[i_handler.index].first.generation != i_handler.generation)
				return nullptr;

			return m_controls[i_handler.index].second.get();
		}

		void UIControlSystem::Update(float i_elapsed_time)
		{
			for (auto& control : m_controls)
			{
				// control is updated from parent
				if (control.second->GetParent() != INVALID_UI_HANDLER)
					continue;
				control.second->Update(i_elapsed_time);
			}
		}

		void UIControlSystem::Draw()
		{
			for (auto& control : m_controls)
			{
				// control is updated from parent
				if (control.second->GetParent() != INVALID_UI_HANDLER)
					continue;
				control.second->Draw();
			}

			auto& render_world = Core::GetApplication()->GetRenderWorld();
			render_world.Submit({Render::ProjectionType::Orthographic, Matrix4f::IDENTITY, Matrix4f::IDENTITY });
		}
		
		void UIControlSystem::SetInputSystem(InputSystem& i_input_system)
		{
			static UI_InputSubscriber g_subscriber(*this);			
		}

		void UIControlSystem::OnResize(const IRect& i_new_size)
		{
			for (auto& control : m_controls)
				control.second->OnResize(i_new_size);
		}

		/*
			button = {
				name = "my_mega_button"
				position = [ 0.2f 0.1f ]
				scale = [ 1.2f 0.9f ]
				rotation = [ 0.5f 1.8f ]
				texture = "button.png"
			}

			scroll_bar = {
				name = "perfect scroll bar"
				position = [ 0.2f 0.1f ]
				scale = [ 1.2f 0.9f ]
				rotation = [ 0.5f 1.8f ]
				texture = "scroll_bar.png"
				auto_scroll_speed = 0.2f
			}

			screen = {
				name = "menu_screen"
				button = {
					...
				}
			}
		*/

		namespace
		{
			template <typename ObjType>
			struct ObjectCreator
			{
				typedef ObjType Type;
				typedef UIControlSystem::UIControlAccessor<Type> Accessor;
				static Accessor Create(const PropertyElement& i_element, UIControlHandler i_parent)
				{
					Accessor accessor(&g_ui_system, INVALID_UI_HANDLER);
					if (i_parent == INVALID_UI_HANDLER)
						accessor = g_ui_system.CreateControl<Type>();
					else
						accessor = g_ui_system.AppendControl<Type>(i_parent);
					accessor.GetActual()->Load(i_element);
					return accessor;
				}
			};
		} // namespace

		void AddControlElement(const PropertyElement::iterator<PropertyElement>& i_it, UIControlHandler i_parent = INVALID_UI_HANDLER)
		{
			// type of control
			auto type = i_it.element_name();
			const PropertyElement& element = *i_it;
			UIControlHandler handler = INVALID_UI_HANDLER;
			if (type == "button")				
				handler = ObjectCreator<UIButton>::Create(element, i_parent).GetHandler();
			else if (type == "screen")
				handler = ObjectCreator<UIScreen>::Create(element, i_parent).GetHandler();

			if (handler == INVALID_UI_HANDLER)
				return;
			
			
			const auto end = element.end<PropertyElement>();
			for (auto it = element.begin<PropertyElement>(); it != end; ++it)
				AddControlElement(it, handler);
		}

		void UIControlSystem::Load(const std::string& i_file_name)
		{
			PropretyReader<(int)ReaderType::SDKFormat> reader;
			PropertyElement root = reader.Parse(i_file_name);

			UIScreen* p_current_screen;

			const auto end = root.end<PropertyElement>();
			for (auto it = root.begin<PropertyElement>(); it != end; ++it)
				AddControlElement(it);
		}

	} // UI
} // SDK