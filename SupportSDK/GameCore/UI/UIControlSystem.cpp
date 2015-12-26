#include "stdafx.h"

#include "UIControlSystem.h"
#include "UIControl.h"

#include "UIScreen.h"
#include "UIButton.h"

#include "Core.h"
#include "Applications/ApplicationBase.h"
#include "Render/RenderWorld.h"

#include "PropertyReaders.h"

namespace SDK
{
	namespace UI
	{
		UIControlSystem g_ui_system;

		UIControlSystem::~UIControlSystem()
		{
			for (auto& p_control : m_controls)
				p_control->SetParent(nullptr);
			m_controls.clear();
		}

		void UIControlSystem::Update(float i_elapsed_time)
		{
			for (auto& p_control : m_controls)
			{
				// control is updated from parent
				if (p_control->GetParent() != nullptr)
					continue;
				p_control->Update(i_elapsed_time);
			}
		}

		void UIControlSystem::Draw()
		{
			for (auto& p_control : m_controls)
			{
				// control is updated from parent
				if (p_control->GetParent() != nullptr)
					continue;
				p_control->Draw();
			}

			auto& render_world = Core::GetApplication()->GetRenderWorld();
			render_world.Submit({Render::ProjectionType::Orthographic, Matrix4f::IDENTITY, Matrix4f::IDENTITY });
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
				static UIControl* Create(const PropertyElement& i_element, UIControl* ip_parent = nullptr)
				{
					auto name = i_element.GetValue<std::string>("name");
					UIControl* p_control = nullptr;
					if (ip_parent == nullptr)
						p_control = g_ui_system.CreateControl<Type>();
					else
						p_control = g_ui_system.AppendControl<Type>(ip_parent);
					p_control->Load(i_element);
					return p_control;
				}
			};
		}

		void AddControlElement(const PropertyElement::iterator<PropertyElement>& i_it, UIControl* ip_parent = nullptr)
		{
			// type of control
			auto type = i_it.element_name();
			const PropertyElement& element = *i_it;
			UIControl* p_control = nullptr;
			if (type == "button")				
				p_control = ObjectCreator<UIButton>::Create(element, ip_parent);
			else if (type == "screen")
				p_control = ObjectCreator<UIScreen>::Create(element, ip_parent);

			if (p_control == nullptr)
				return;
			
			
			const auto end = element.end<PropertyElement>();
			for (auto it = element.begin<PropertyElement>(); it != end; ++it)
				AddControlElement(it, p_control);
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