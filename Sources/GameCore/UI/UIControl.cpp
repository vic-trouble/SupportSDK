#include "stdafx.h"

#include "UIControl.h"

#include "BaseTypesHashes.h"
#include "PropertyElement.h"

#include "Core.h"
#include "Render/IRenderer.h"
#include "UIControlSystem.h"

#include <Utilities/HashFunctions.h>

namespace SDK
{
	namespace UI
	{
		UIControl::UIControl()
			: m_relative_position()
			, m_global_position()
			, m_parent(INVALID_UI_HANDLER)
			, m_this_handler(INVALID_UI_HANDLER)
		{
			m_scale[0] = m_scale[1] = 1.f;
			m_rotation[0] = m_rotation[1] = 0.f;
			m_size[0] = m_size[1] = 0.01f;
		}

		UIControl::~UIControl()
		{
			UIControl* p_parent = g_ui_system.AccessControl(m_parent);
			if (p_parent)
				p_parent->RemoveChild(m_this_handler);
		}

		void UIControl::InitializeThisHandler()
		{
			m_this_handler = g_ui_system.GetHandlerTo(this);
		}

		void UIControl::Update(float i_elapsed_time)
		{
			UpdateImpl(i_elapsed_time);
			for (auto child : m_children)
			{
				auto p_control = g_ui_system.AccessControl(child);
				if (p_control)
					p_control->Update(i_elapsed_time);
			}
		}

		void UIControl::Draw()
		{
			DrawImpl();
			for (auto child : m_children)
			{
				auto p_control = g_ui_system.AccessControl(child);
				if (p_control)
					p_control->Draw();
			}
				
		}

		void UIControl::SetParent(UIControlHandler i_parent)
		{
			UIControl* p_parent = g_ui_system.AccessControl(m_parent);
			if (p_parent != nullptr)
				p_parent->RemoveChild(m_this_handler);
				
			m_parent = i_parent;

			p_parent = g_ui_system.AccessControl(m_parent);
			if (p_parent != nullptr)
				p_parent->AddChild(m_this_handler);
		}

		UIControl* UIControl::GetParentPtr() const
		{
			return g_ui_system.AccessControl(m_parent);
		}

		void UIControl::AddChild(UIControlHandler i_child)
		{
			assert(i_child != INVALID_UI_HANDLER);
			m_children.push_back(i_child);
		}

		void UIControl::RemoveChild(UIControlHandler i_child)
		{
			assert(i_child != INVALID_UI_HANDLER);
			auto it = std::find(m_children.begin(), m_children.end(), i_child);
			if (it != m_children.end())
				m_children.erase(it);
		}		

		void UIControl::OnResize(const IRect& i_new_size)
		{
			// position
			m_global_position = { static_cast<int>(i_new_size.Width()*m_relative_position[0]), static_cast<int>(i_new_size.Height()*m_relative_position[1]) };
			// size
			m_global_size = { static_cast<int>(i_new_size.Width()*m_size[0]), static_cast<int>(i_new_size.Height()*m_size[1]) };
			// rectangle
			m_rect = IRect(m_global_position, m_global_size[0], m_global_size[1]);
		}

		void UIControl::Load(const PropertyElement& i_element)
		{
			//-------------------------------------------------------------------
			// Get general parameters
			// 0. name
			m_name = i_element.GetValue<std::string>("name");

			// 1. size
			auto p_size = i_element.GetValuePtr<std::vector<float>>("size");
			if (p_size)
			{
				m_size[0] = p_size->at(0);
				m_size[1] = p_size->at(1);
			}

			// 2. position
			auto p_position = i_element.GetValuePtr<std::vector<float>>("position");
			if (p_position)
			{
				m_relative_position[0] = p_position->at(0);
				m_relative_position[1] = p_position->at(1);
			}

			// 3. scale
			auto p_scale = i_element.GetValuePtr<std::vector<float>>("scale");
			if (p_scale)
			{
				m_scale[0] = p_scale->at(0);
				m_scale[1] = p_scale->at(1);
			}
			// 4. rotation
			auto p_rotation = i_element.GetValuePtr<std::vector<float>>("position");
			if (p_rotation)
			{
				m_rotation[0] = p_rotation->at(0);
				m_rotation[1] = p_rotation->at(1);
			}
			// to calculate all values first time
			const auto rect = Core::GetRenderer()->GetTargetRectangle();
			OnResize(rect);
			//-------------------------------------------------------------------
			// Specific for inheritor
			LoadImpl(i_element);
		}

		Vector2i UIControl::GetGlobalPosition() const
		{
			return m_global_position;
		}

		Vector2i UIControl::GetGlobalSize() const
		{
			return m_global_size;
		}

		bool UIControl::IsHited(const Vector2i& i_hit) const
		{
			return m_rect.IsInside(i_hit);
		}

	} // UI
} // SDK