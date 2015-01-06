#include "stdafx.h"

#include "DecisionTrees/DecisionTreeNode.h"
#include "DecisionTrees/DecitionTree.h"
#include "Exceptions.h"

using namespace SDK::DecisionMaking;
using ::testing::Return;
using ::testing::_;

class DecisionStub : public DecisionTreeNode
	{
	public:
		DecisionStub(int i_id, float a, float b)
			: DecisionTreeNode(i_id)
			{}

		MOCK_METHOD1(MakeDecision, DecisionTreeNode*(long i_available_time));
	};

TEST(DecisionTreeTests, CreateMethod)
	{
	DecisionTree tree;

	auto p_node = tree.Create<DecisionStub>([](int i_id){return std::unique_ptr<DecisionStub>(new DecisionStub(i_id, 1.f, 2.f)); });

	EXPECT_EQ(typeid(DecisionStub), typeid(*p_node));
	EXPECT_EQ(0, p_node->GetId());
	}

TEST(DecisionTreeTests, MakeDecision_NullRootNode_ShouldThrow)
	{
	DecisionTree tree;

	EXPECT_THROW(tree.MakeDecision(1), RootNodeIsNull);
	}

TEST(DecisionTreeTests, MakeDecision_StartWithRootNodeCurrentWillChange)
	{
	DecisionTree tree;

	auto p_node_first = tree.Create<DecisionStub>([](int i_id){return std::unique_ptr<DecisionStub>(new DecisionStub(i_id, 1.f, 2.f)); });
	auto p_node_second = tree.Create<DecisionStub>([](int i_id){return std::unique_ptr<DecisionStub>(new DecisionStub(i_id, 2.f, 3.f)); });

	tree.SetRootNode(p_node_first);

	EXPECT_CALL(*p_node_first, MakeDecision(_)).WillOnce(Return(p_node_second));

	auto p_result_node = tree.MakeDecision(1);
	EXPECT_EQ(p_node_second, p_result_node);
	}