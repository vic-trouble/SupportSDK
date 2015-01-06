#ifndef __DECISIONMAKING_DECISIONTREE_H__
#define __DECISIONMAKING_DECISIONTREE_H__

#include "../DecisionMakingAPI.h"

namespace SDK
	{

	namespace DecisionMaking
		{

		class DecisionTreeNode;

		/*
		Does not make any calculations for available time or parallelism
		If user needs this - inherit class from DecisionTree and add funcional
		*/
		class DecisionTree
			{
			private:
				std::vector<std::unique_ptr<DecisionTreeNode>> m_nodes;

				int m_next_id;

				// root node is always the same
				DecisionTreeNode* mp_root_node;
				// current node can change depends on decisions that were made earlier
				DecisionTreeNode* mp_current_node;

			public:
				DECISIONMAKING_API DecisionTree();
				DECISIONMAKING_API virtual ~DecisionTree();

				/*
				Wait for C++11 in compilers
				template <typename T, typename... Args>
				T* Create(Args&&... i_args)
					{
					static_assert(std::is_base_of<DecisionTreeNode, T>::value,
												"T must be derived from DecisionTreeNode");

					auto p_node(std::make_unique<T>(std::forward<Args>(i_args)...));

					return p_node.get();
					}*/

				template <typename T>
				T* Create(std::function<std::unique_ptr<T>(int)> i_creator)
					{
					static_assert(std::is_base_of<DecisionTreeNode, T>::value,
						"T must be derived from DecisionTreeNode");

					auto p_node = i_creator(0);
					T* p_type = p_node.get();

					m_nodes.emplace_back(std::move(p_node));

					return p_type;
					}

				DECISIONMAKING_API DecisionTreeNode* GetNode(int i_id) const;

				// throws exception if root node is not nullptr
				DECISIONMAKING_API void SetRootNode(int i_id);
				DECISIONMAKING_API void SetRootNode(DecisionTreeNode* ip_root_node);

				DECISIONMAKING_API DecisionTreeNode* MakeDecision(long i_available_time);
			};

		} // DecisionMaking

	} // SDK

#endif