#include "stdafx.h"

#include "DecitionTree.h"
#include "DecisionTreeNode.h"

#include "../Exceptions.h"

namespace
	{
	struct FindNode_Id
		{
		int m_id;

		FindNode_Id(int i_id)
			: m_id(i_id)
			{}

		bool operator == (const std::unique_ptr<SDK::DecisionMaking::DecisionTreeNode>& ip_node) const
			{
			return ip_node->GetId() == m_id;
			}

		bool operator () (const std::unique_ptr<SDK::DecisionMaking::DecisionTreeNode>& ip_node) const
			{
			return ip_node->GetId() == m_id;
			}
		};

	struct FindNode_Pointer
		{
		SDK::DecisionMaking::DecisionTreeNode* mp_node;

		FindNode_Pointer(SDK::DecisionMaking::DecisionTreeNode* ip_node)
			: mp_node(ip_node)
			{}

		bool operator == (const std::unique_ptr<SDK::DecisionMaking::DecisionTreeNode>& ip_node) const
			{
			return ip_node.get() == mp_node;
			}

		bool operator () (const std::unique_ptr<SDK::DecisionMaking::DecisionTreeNode>& ip_node) const
			{
			return ip_node.get() == mp_node;
			}
		};
	}

namespace SDK
	{

	namespace DecisionMaking
		{

		DecisionTree::DecisionTree()
			: mp_root_node(nullptr)
			, mp_current_node(nullptr)
			{
			}

		DecisionTree::~DecisionTree()
			{
			}

		DecisionTreeNode* DecisionTree::GetNode(int i_id) const
			{
			auto it = std::find_if(m_nodes.begin(), m_nodes.end(), FindNode_Id(i_id));

			if (it == m_nodes.end())
				throw NotFoundException("Node does not find");

			return (*it).get();
			}

		void DecisionTree::SetRootNode(int i_id)
			{
			if (mp_root_node != nullptr)
				throw RootNodeAlreadyExist();

			auto p_node = GetNode(i_id);

			mp_root_node = p_node;
			mp_current_node = mp_root_node;
			}

		void DecisionTree::SetRootNode(DecisionTreeNode* ip_root_node)
			{
			SetRootNode(ip_root_node->GetId());
			}

		DecisionTreeNode* DecisionTree::MakeDecision(long i_available_time)
			{
			if (mp_current_node == nullptr)
				throw RootNodeIsNull();

			auto p_node = mp_current_node->MakeDecision(i_available_time);
			mp_current_node = p_node;

			if (mp_current_node == nullptr)
				mp_current_node = mp_root_node;

			return mp_current_node;
			}

		} // DecisionMaking

	} // SDK