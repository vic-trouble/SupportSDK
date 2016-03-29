#include "stdafx.h"

#include <PathFinding.h>
#include <Graph.h>
#include <GraphNode.h>
#include <Connection.h>

using namespace SDK::Graphs;

namespace
	{

	/*
						*
			|			|			|
			*			*			*
		 / \		|		 /	\
		*   *		|		*		 *
		\  /  \	|
			*	--- *
	*/
	void InitializeTestGraph(Graph& o_graph)
		{
		auto p_root = o_graph.AddNode(NodePtr(new GraphNode));		// 0
		auto p_first_0 = o_graph.AddNode(NodePtr(new GraphNode));	// 1
		auto p_first_1 = o_graph.AddNode(NodePtr(new GraphNode)); // 2
		auto p_first_2 = o_graph.AddNode(NodePtr(new GraphNode)); // 3

		auto p_second_0 = o_graph.AddNode(NodePtr(new GraphNode)); // 4
		auto p_second_1 = o_graph.AddNode(NodePtr(new GraphNode)); // 5
		auto p_second_2 = o_graph.AddNode(NodePtr(new GraphNode)); // 6
		auto p_second_3 = o_graph.AddNode(NodePtr(new GraphNode)); // 7
		auto p_second_4 = o_graph.AddNode(NodePtr(new GraphNode)); // 8

		auto p_third_0 = o_graph.AddNode(NodePtr(new GraphNode)); // 9
		auto p_third_1 = o_graph.AddNode(NodePtr(new GraphNode)); // 10

		o_graph.AddConnection(p_root, p_first_0, 1);
		o_graph.AddConnection(p_root, p_first_1, 1);
		o_graph.AddConnection(p_root, p_first_2, 1);

		o_graph.AddConnection(p_first_0, p_second_0, 1);
		o_graph.AddConnection(p_first_0, p_second_1, 0.2f);

		o_graph.AddConnection(p_first_1, p_third_1, 5.f);

		o_graph.AddConnection(p_first_2, p_second_3, 1.f);
		o_graph.AddConnection(p_first_2, p_second_4, 1.5f);

		o_graph.AddConnection(p_second_0, p_third_0, 1.f);
		o_graph.AddConnection(p_second_1, p_third_0, 1.f);
		o_graph.AddConnection(p_second_1, p_third_1, .3f);

		o_graph.AddConnection(p_third_0, p_third_1, 1.f);
		}

	} // namespace

TEST(PathFindingTests, Dijkstra_PathToSameNode_ShouldBeEmpty)
	{
	Graph graph;
	InitializeTestGraph(graph);

	GraphNode* p_first = graph.GetNode(0);

	DijkstraPath algo;
	auto path = algo.CalculatePath(100, graph, p_first, p_first);

	EXPECT_TRUE(path.empty());
	}

TEST(PathFindingTests, Dijkstra_OnePossibleVariant_NodePath)
	{
	Graph graph;
	InitializeTestGraph(graph);

	GraphNode* p_begin = graph.GetNode(0);
	GraphNode* p_end = graph.GetNode(4);
	GraphNode* p_intermediate = graph.GetNode(1);

	DijkstraPath algo;
	auto path = algo.CalculatePath(100, graph, p_begin, p_end);

	EXPECT_EQ(2, path.size());
	// root -> first layer node
	EXPECT_EQ(p_begin, path[0]->GetFromNode());
	EXPECT_EQ(p_intermediate, path[0]->GetToNode());
	// first layer node -> second layer node
	EXPECT_EQ(p_intermediate, path[1]->GetFromNode());
	EXPECT_EQ(p_end, path[1]->GetToNode());
	}

TEST(PathFindingTests, Dijkstra_TwoPossibleVariants_ReturnWithSmallestCost)
	{
	Graph graph;
	InitializeTestGraph(graph);

	GraphNode* p_begin = graph.GetNode(0);
	GraphNode* p_end = graph.GetNode(10);

	DijkstraPath algo;
	auto path = algo.CalculatePath(100, graph, p_begin, p_end);

	EXPECT_EQ(3, path.size());
	// root -> first layer node
	EXPECT_EQ(p_begin, path[0]->GetFromNode());
	EXPECT_EQ(graph.GetNode(1), path[0]->GetToNode());

	// first layer -> second layer
	EXPECT_EQ(graph.GetNode(1), path[1]->GetFromNode());
	EXPECT_EQ(graph.GetNode(5), path[1]->GetToNode());
	// second layer -> end
	EXPECT_EQ(graph.GetNode(5), path[2]->GetFromNode());
	EXPECT_EQ(p_end, path[2]->GetToNode());
	}