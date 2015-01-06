#include "stdafx.h"

#include "DecisionTrees/Decision.h"

using namespace SDK::DecisionMaking;
using ::testing::Return;

class DecisionMock : public Decision
	{
	public:
		DecisionMock(int i_id, DecisionTreeNode* ip_true, DecisionTreeNode* ip_false)
			: Decision(i_id, ip_true, ip_false)
			{}
		MOCK_METHOD0(GetBranch, DecisionTreeNode*());
	};

class DecisionNodeMock : public DecisionTreeNode
	{
	public:
		DecisionNodeMock(int i_id)
			: DecisionTreeNode(i_id)
			{}
		MOCK_METHOD1(MakeDecision, DecisionTreeNode*(long i_available_time));
	};

TEST(DecisionTests, MakeDecision_CallsGetBranchAndMakeDecision)
	{
	DecisionNodeMock first(0);
	DecisionNodeMock second(1);

	DecisionMock decision(2, &first, &second);

	EXPECT_CALL(decision, GetBranch()).WillOnce(Return(&first));
	EXPECT_CALL(first, MakeDecision(1)).WillOnce(Return(&first));

	EXPECT_EQ(&first, decision.MakeDecision(1));
	}

TEST(DecisionTests, PerformAction_DoNotCrashWithNullFunction)
	{
	DecisionNodeMock first(0);
	DecisionNodeMock second(1);

	first.Perform(1);
	}

TEST(DecisionTests, PerformAction_CallFunction)
	{
	DecisionNodeMock first(0);
	DecisionNodeMock second(1);

	bool performed = false;
	first.SetPerformAction([&performed](long i_available_time)
		{
		performed = true;
		});
	first.Perform(1);

	EXPECT_TRUE(performed);
	}
