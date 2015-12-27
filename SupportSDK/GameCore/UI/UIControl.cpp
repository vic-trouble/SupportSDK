#include "stdafx.h"

#include "UIControl.h"

#include "BaseTypesHashes.h"
#include "PropertyElement.h"

#include "Core.h"
#include "Render/IRenderer.h"

#include <Utilities/HashFunctions.h>

namespace SDK
{
	namespace UI
	{
		UIControl::UIControl()
			: mp_parent(nullptr)
			, m_relative_position()
			, m_global_position()
		{
			m_scale[0] = m_scale[1] = 1.f;
			m_rotation[0] = m_rotation[1] = 0.f;
			m_size[0] = m_size[1] = 0.01f;
		}

		UIControl::~UIControl()
		{
			if (mp_parent)
				mp_parent->RemoveChild(this);
		}

		void UIControl::Update(float i_elapsed_time)
		{
			UpdateImpl(i_elapsed_time);
			for (auto p_child : m_children)
				p_child->Update(i_elapsed_time);
		}

		void UIControl::Draw()
		{
			DrawImpl();
			for (auto p_child : m_children)
				p_child->Draw();
		}

		void UIControl::SetParent(UIControl* ip_parent)
		{
			if (mp_parent)
				mp_parent->RemoveChild(this);
			mp_parent = ip_parent;
			if (mp_parent)
				mp_parent->AddChild(this);
		}

		void UIControl::AddChild(UIControl* ip_child)
		{
			assert(ip_child != nullptr);
			m_children.push_back(ip_child);
		}

		void UIControl::RemoveChild(UIControl* ip_child)
		{
			assert(ip_child != nullptr);
			auto it = std::find(m_children.begin(), m_children.end(), ip_child);
			if (it != m_children.end())
				m_children.erase(it);
		}		

		void UIControl::RecalculateGlobalValues()
		{
			auto rect = Core::GetRenderer()->GetTargetRectangle();			
			// position
			m_global_position = { static_cast<uint>(rect.Width()*m_relative_position[0]), static_cast<uint>(rect.Height()*m_relative_position[1]) };
			// size
			m_global_size = { static_cast<uint>(rect.Width()*m_size[0]), static_cast<uint>(rect.Height()*m_size[1]) };
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
			RecalculateGlobalValues();
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

	} // UI
} // SDK