#ifndef __DECISIONMAKING_DECISION_H__
#define __DECISIONMAKING_DECISION_H__

#include "DecisionTreeNode.h"

namespace SDK
	{
	namespace DecisionMaking
		{

		class Decision : public DecisionTreeNode
			{
			private:
				DecisionTreeNode* mp_true_node;
				DecisionTreeNode* mp_false_node;

			public:
				Decision(int i_id, DecisionTreeNode* ip_true_node, DecisionTreeNode* ip_false_node)
					: DecisionTreeNode(i_id)
					, mp_true_node(ip_true_node)
					, mp_false_node(ip_false_node)
					{}
				virtual ~Decision(){}

				virtual DecisionTreeNode* GetBranch() = 0;
				virtual DecisionTreeNode* MakeDecision(long i_available_time) override
					{
					auto p_branch = GetBranch();
					return p_branch->MakeDecision(i_available_time);
					}
			};

		} // DecisonMaking
	} // SDK

#endif