#include "stdafx.h"

#include "UIControlSystem.h"
#include "UIControl.h"

#include "Core.h"
#include "Applications/ApplicationBase.h"
#include "Render/RenderWorld.h"

#include "PropertyReaders.h"

#include "Input/InputSystem.h"
#include "Input/InputSubscriber.h"

#include "Render/Commands.h"
#include "Render/IRenderer.h"
#include "Render/ScopedLightSwitch.h"

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
			/*
			/// TODO: Should UI controls handle key events? Or only mouse
			virtual bool KeyPressed(const KeyEvent& i_evt) override
			{
				return false;
			}
			virtual bool KeyReleased(const KeyEvent& i_evt) override
			{
				return false;
			}
			*/
			virtual bool MouseMoved(const MouseEvent& i_evt) override
			{
				return false;
			}
			virtual bool MousePressed(const MouseEvent& i_evt) override
			{
				return false;
			}
			virtual bool MouseReleased(const MouseEvent& i_evt) override
			{
				auto& controls = g_ui_system.m_controls;
				for (auto& control : controls.m_elements)
				{
					if (control.second == nullptr)
						continue;
					UIControl* p_control = control.second.get();
					if (control.second->IsHited({ i_evt.m_x, i_evt.m_y }))
					{
						p_control->MouseReleased(g_ui_system.m_message_dispatcher, i_evt);
						return true;
					}
				}
				return false;
			}
		};
		
		namespace detail { extern void RegisterBaseUITypes(UIControlSystem&); }

		UIControlSystem::UIControlSystem()
			: m_current_scheme(INVALID_UISCHEME_HANDLER)
		{
			detail::RegisterBaseUITypes(*this);
		}

		UIControlSystem::~UIControlSystem()
		{
			for (auto& control : m_controls.m_elements)
			{
				if (control.second == nullptr)
					continue;
				control.second->SetParent(INVALID_UI_HANDLER);
			}
			m_controls.ClearAll();
		}

		UIControlHandler UIControlSystem::GetHandlerTo(UIControl* ip_pointer) const
		{
			auto it = std::find_if(m_controls.m_elements.begin(), m_controls.m_elements.end(), [ip_pointer](const UIControls::ArrayElement& control)
			{
				return control.second != nullptr && control.second.get() == ip_pointer;
			});
			if (it == m_controls.m_elements.end())
				return INVALID_UI_HANDLER;
			return it->first;
		}

		UIControl* UIControlSystem::AccessControl(UIControlHandler i_handle)
		{
			return m_controls.Access(i_handle);
		}

		void UIControlSystem::Update(float i_elapsed_time)
		{
			if (m_current_scheme == INVALID_UISCHEME_HANDLER)
				return;

			UIScheme& current = m_schemes.m_elements[m_current_scheme.index].second;

			for (auto& handler : current.m_handlers)
			{
				auto p_control = m_controls.Access(handler);
				if (p_control == nullptr)
					continue;				
				// control is updated from parent
				if (p_control->GetParent() != INVALID_UI_HANDLER)
					continue;
				p_control->Update(i_elapsed_time);
			}
		}

		void UIControlSystem::Draw()
		{
			if (m_current_scheme == INVALID_UISCHEME_HANDLER)
				return;

			UIScheme& current = m_schemes.m_elements[m_current_scheme.index].second;

			for (auto& handler : current.m_handlers)
			{
				auto p_control = m_controls.Access(handler);
				if (p_control == nullptr)
					continue;
				// control is updated from parent
				if (p_control->GetParent() != INVALID_UI_HANDLER)
					continue;
				p_control->Draw();
			}

			Render::ScopedLightSwitch scopedLight(Core::GetRenderer()->GetLightsController());
			auto& render_world = Core::GetApplication()->GetRenderWorld();
			render_world.Submit({Render::ProjectionType::Orthographic, Matrix4f::IDENTITY, Matrix4f::IDENTITY });
		}
		
		UIControl* UIControlSystem::FindControlInCurrentScheme(const std::string& i_control_name)
		{
			if (m_current_scheme == INVALID_UISCHEME_HANDLER)
				return nullptr;

			const UIScheme& current = m_schemes.m_elements[m_current_scheme.index].second;
			const size_t hash = Utilities::hash_function(i_control_name);
			for (auto& handler : current.m_handlers)
			{
				auto p_control = m_controls.Access(handler);
				if (p_control == nullptr)
					continue;
				const size_t control_hash = Utilities::hash_function(p_control->GetName());
				if (control_hash == hash)
					return p_control;
			}

			return nullptr;
		}

		void UIControlSystem::SetInputSystem(InputSystem& i_input_system)
		{
			static UI_InputSubscriber g_subscriber(*this);			
		}

		void UIControlSystem::OnResize(const IRect& i_new_size)
		{
			for (auto& control : m_controls.m_elements)
			{
				if (control.second == nullptr)
					continue;
				control.second->OnResize(i_new_size);
			}
		}

		void UIControlSystem::RemoveControl(UIControlHandler i_handler)
		{
			m_controls.Destroy(i_handler);
		}

		void AddControlElement(UIControlSystem::UIScheme& o_scheme, const PropertyElement::iterator<PropertyElement>& i_it, UIControlHandler i_parent = INVALID_UI_HANDLER)
		{
			// type of control
			auto type = i_it.element_name();
			const PropertyElement& element = *i_it;
			UIControlSystem::UIControlAccessor<UIControl> accessor = g_ui_system.GetFactory().Create(type);

			if (!accessor.IsValid())
				return;

			accessor.GetActual()->Load(element);

			if (i_parent != INVALID_UI_HANDLER)
				accessor.GetActual()->SetParent(i_parent);			

			o_scheme.AddControl(accessor.GetHandler());
			
			const auto end = element.end<PropertyElement>();
			for (auto it = element.begin<PropertyElement>(); it != end; ++it)
				AddControlElement(o_scheme, it, accessor.GetHandler());
		}

		UISchemeHandler UIControlSystem::LoadScheme(const std::string& i_file_name)
		{
			PropretyReader<(int)ReaderType::SDKFormat> reader;
			PropertyElement root = reader.Parse(i_file_name);

			std::string scheme_name = root.GetValue<std::string>("name");
			const size_t hash = Utilities::hash_function(scheme_name);
			
			// validate scheme name and existance in UISystem
			{
				if (scheme_name.empty())
				{
					assert(false && "Scheme name is empty");
					return INVALID_UISCHEME_HANDLER;
				}

				UISchemeHandler test_handler = FindScheme(Utilities::hash_function(scheme_name));
				if (test_handler != INVALID_UISCHEME_HANDLER)
				{
					assert(false && "Scheme with same name already exist");
					return INVALID_UISCHEME_HANDLER;
				}
			}

			auto scheme_handle = m_schemes.CreateNew(scheme_name, hash);
			UIScheme* p_scheme = m_schemes.Access(scheme_handle);			

			const auto end = root.end<PropertyElement>();
			for (auto it = root.begin<PropertyElement>(); it != end; ++it)
				AddControlElement(*p_scheme, it);

			return scheme_handle;
		}

		UISchemeHandler UIControlSystem::FindScheme(size_t i_hash)
		{
			auto it = std::find_if(m_schemes.m_elements.begin(), m_schemes.m_elements.end(), [i_hash](const UISchemes::ArrayElement& scheme)
			{
				return scheme.second.GetHash() == i_hash;
			});
			if (it == m_schemes.m_elements.end())
				return INVALID_UISCHEME_HANDLER;
			else
				return it->first;
		}

		void UIControlSystem::SetActiveScheme(const std::string& i_scheme_name)
		{
			m_current_scheme = FindScheme(Utilities::hash_function(i_scheme_name));
		}

		void UIControlSystem::SetActiveScheme(UISchemeHandler i_scheme)
		{			
			UIScheme* p_scheme = m_schemes.Access(i_scheme);

			if (p_scheme == nullptr)
				m_current_scheme = INVALID_UISCHEME_HANDLER;
			else
				m_current_scheme = i_scheme;
		}

		void UIControlSystem::UnloadScheme(UISchemeHandler i_scheme)
		{
			if (i_scheme == INVALID_UISCHEME_HANDLER)
				return;

			auto p_scheme = m_schemes.Access(i_scheme);
			if (p_scheme == nullptr)
				return;

			for (auto& handler : p_scheme->m_handlers)
				RemoveControl(handler);

			m_schemes.Destroy(i_scheme);			
		}

		void UIControlSystem::UnloadScheme(const std::string& i_scheme)
		{
			UnloadScheme(FindScheme(Utilities::hash_function(i_scheme)));
		}

		const UIControlSystem::UIScheme* UIControlSystem::AccessScheme(UISchemeHandler i_scheme) const
		{
			return m_schemes.Access(i_scheme);
		}

	} // UI
} // SDK