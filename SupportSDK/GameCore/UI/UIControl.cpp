#include "stdafx.h"

#include "UIControl.h"

#include "BaseTypesHashes.h"

#include <Utilities/HashFunctions.h>

namespace SDK
{
	namespace UI
	{
		UIControl::UIControl()
			: mp_parent(nullptr)
		{}

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

	} // UI
} // SDK