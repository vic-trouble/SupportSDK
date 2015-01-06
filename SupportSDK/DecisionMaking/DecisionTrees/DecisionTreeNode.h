#ifndef __DECISIONMAKING_DECISIONTREENODE_H__
#define __DECISIONMAKING_DECISIONTREENODE_H__

namespace SDK
	{
	namespace DecisionMaking
		{

		class DecisionTreeNode
			{
			private:
				std::function<void(long)> m_perform_action;
				int m_id;

			public:
				DecisionTreeNode(int i_id, std::function<void(long)> i_perform_action = std::function<void(long)>())
					: m_perform_action(i_perform_action)
					, m_id(i_id)
					{}
				virtual ~DecisionTreeNode(){}

				void SetPerformAction(std::function<void(long)> i_perform_action)
					{
					m_perform_action = i_perform_action;
					}

				int GetId() const { return m_id; }

				virtual DecisionTreeNode* MakeDecision(long i_available_time) = 0;

				void Perform(long i_available_time)
					{
					if (m_perform_action)
						m_perform_action(i_available_time);
					}
			};

		// use this class to mark leafs
		class DecisionTreeAction : public DecisionTreeNode
			{
			public:
				DecisionTreeAction(int i_id, std::function<void(long)> i_perform_action)
					: DecisionTreeNode(i_id, i_perform_action)
					{}

				virtual DecisionTreeNode* MakeDecision(long i_available_time) override
					{
					return this;
					}
			};

		} // DecisonMaking
	} // SDK

#endif