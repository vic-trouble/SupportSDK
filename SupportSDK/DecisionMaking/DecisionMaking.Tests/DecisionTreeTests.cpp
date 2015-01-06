#include "stdafx.h"

#include "DecisionTrees/DecisionTreeNode.h"
#include "DecisionTrees/DecitionTree.h"
#include "DecisionTrees/Decision.h"
#include "Exceptions.h"

using namespace SDK::DecisionMaking;
using ::testing::Return;
using ::testing::_;

class DecisionNodeStub : public DecisionTreeNode
	{
	public:
		DecisionNodeStub(int i_id)
			: DecisionTreeNode(i_id)
			{}
		DecisionNodeStub(int i_id, float a, float b)
			: DecisionTreeNode(i_id)
			{}

		MOCK_METHOD1(MakeDecision, DecisionTreeNode*(long i_available_time));
	};

class DecisionStub : public Decision
	{
	public:
		DecisionStub(int i_id)
			: Decision(i_id)
			{}
		DecisionStub(int i_id, DecisionTreeNode* ip_true_node, DecisionTreeNode* ip_false_node)
			: Decision(i_id, ip_true_node, ip_false_node)
			{}

		MOCK_METHOD1(GetBranch, DecisionTreeNode*(long i_available_time));
	};

TEST(DecisionTreeTests, CreateMethod)
	{
	DecisionTree tree;

	auto p_node_0 = tree.Create<DecisionNodeStub>([](int i_id){return std::unique_ptr<DecisionNodeStub>(new DecisionNodeStub(i_id, 1.f, 2.f)); });
	auto p_node_1 = tree.Create<DecisionNodeStub>([](int i_id){return std::unique_ptr<DecisionNodeStub>(new DecisionNodeStub(i_id, 1.f, 2.f)); });

	EXPECT_EQ(typeid(DecisionNodeStub), typeid(*p_node_0));
	EXPECT_EQ(0, p_node_0->GetId());
	EXPECT_EQ(1, p_node_1->GetId());
	}

TEST(DecisionTreeTests, MakeDecision_NullRootNode_ShouldThrow)
	{
	DecisionTree tree;

	EXPECT_THROW(tree.MakeDecision(1), RootNodeIsNull);
	}

TEST(DecisionTreeTests, MakeDecision_StartWithRootNodeCurrentWillChange)
	{
	DecisionTree tree;

	auto p_node_first = tree.Create<DecisionNodeStub>([](int i_id){return std::unique_ptr<DecisionNodeStub>(new DecisionNodeStub(i_id, 1.f, 2.f)); });
	auto p_node_second = tree.Create<DecisionNodeStub>([](int i_id){return std::unique_ptr<DecisionNodeStub>(new DecisionNodeStub(i_id, 2.f, 3.f)); });

	tree.SetRootNode(p_node_first);

	EXPECT_CALL(*p_node_first, MakeDecision(_)).WillOnce(Return(p_node_second));

	auto p_result_node = tree.MakeDecision(1);
	EXPECT_EQ(p_node_second, p_result_node);
	}

TEST(DecisionTreeTests, TestUsage)
	{
	/*
							*
					*				*
				*		*		*
							*
	*/

	DecisionTree tree;

	// construct tree
	auto p_result_node = tree.Create<DecisionTreeAction >([](int i_id){return std::unique_ptr<DecisionTreeAction >(new DecisionTreeAction(i_id, [](long i_elapsed_time){throw std::exception(); })); });

	auto p_true_node_1 = tree.Create<DecisionStub>([p_result_node](int i_id){return std::unique_ptr<DecisionStub>(new DecisionStub(i_id, p_result_node, nullptr)); });
	auto p_false_node_11 = tree.Create<DecisionStub>([p_result_node](int i_id){return std::unique_ptr<DecisionStub>(new DecisionStub(i_id, p_result_node, nullptr)); });
	auto p_false_node_1 = tree.Create<DecisionStub>([p_result_node](int i_id){return std::unique_ptr<DecisionStub>(new DecisionStub(i_id, nullptr, p_result_node)); });
	

	auto p_true_node_0 = tree.Create<DecisionStub>([p_false_node_1](int i_id){return std::unique_ptr<DecisionStub>(new DecisionStub(i_id, nullptr, p_false_node_1)); });
	auto p_false_node_0 = tree.Create<DecisionStub>([p_true_node_1, p_false_node_11](int i_id){return std::unique_ptr<DecisionStub>(new DecisionStub(i_id, p_true_node_1, p_false_node_11)); });
	auto p_root = tree.Create<DecisionStub>([p_true_node_0, p_false_node_0](int i_id){return std::unique_ptr<DecisionStub>(new DecisionStub(i_id, p_true_node_0, p_false_node_0)); });

	// set root node	
	tree.SetRootNode(p_root);

	EXPECT_CALL(*p_root, GetBranch(_)).WillOnce(Return(p_true_node_0));
	EXPECT_CALL(*p_true_node_0, GetBranch(_)).WillOnce(Return(p_true_node_1));
	EXPECT_CALL(*p_true_node_1, GetBranch(_)).WillOnce(Return(p_result_node));

	// make decision
	auto p_result = tree.MakeDecision(1);
	EXPECT_EQ(p_result_node, p_result);

	// perform decision that were found
	EXPECT_THROW(p_result->Perform(1), std::exception);
	}