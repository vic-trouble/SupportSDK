#include "stdafx.h"

#include <GraphNode.h>
#include <Graph.h>
#include <Connection.h>

using namespace SDK::Graphs;

TEST(GraphTests, AddNode)
	{
	Graph graph;
	auto p_node = graph.AddNode(NodePtr(new GraphNode));
	
	ASSERT_EQ(0, p_node->GetIndex());
	ASSERT_EQ(&graph, p_node->GetGraph());
	}

TEST(GraphTests, GetConnectionsForNode)
	{
	Graph graph;
	auto p_node = graph.AddNode(NodePtr(new GraphNode));
	auto p_node_1 = graph.AddNode(NodePtr(new GraphNode));

	auto connection = graph.AddConnection(p_node, p_node_1, 0);
	auto connections_for_node = graph.GetConnections(p_node);

	ASSERT_EQ(1, connections_for_node.size());
	ASSERT_EQ(connection, connections_for_node[0]);
	}

TEST(GraphTests, RemoveNode_ClearConnection)
	{
	Graph graph;
	auto p_node = graph.AddNode(NodePtr(new GraphNode));
	auto p_node_1 = graph.AddNode(NodePtr(new GraphNode));

	auto connection = graph.AddConnection(p_node, p_node_1, 0);

	graph.RemoveNode(p_node);

	auto connections_for_node = graph.GetConnections(p_node);
	ASSERT_TRUE(connections_for_node.empty());
	connections_for_node = graph.GetConnections(p_node_1);
	ASSERT_TRUE(connections_for_node.empty());
	}

TEST(GraphTests, GetNode_ReturnCorrect)
	{
	Graph graph;
	auto p_node = graph.AddNode(NodePtr(new GraphNode));
	auto p_node_1 = graph.AddNode(NodePtr(new GraphNode));

	ASSERT_EQ(p_node, graph.GetNode(0));
	ASSERT_EQ(p_node_1, graph.GetNode(1));
	}

TEST(GraphTests, AddConnection)
	{
	Graph graph;
	auto p_node = graph.AddNode(NodePtr(new GraphNode));
	auto p_node_1 = graph.AddNode(NodePtr(new GraphNode));

	auto connection = graph.AddConnection(p_node, p_node_1, 0);
	graph.AddConnection(p_node_1, p_node, 0);

	auto connections_for_node = graph.GetConnections(p_node);
	ASSERT_EQ(1, connections_for_node.size());
	ASSERT_EQ(connection, connections_for_node[0]);
	connections_for_node = graph.GetConnections(p_node_1);
	ASSERT_EQ(1, connections_for_node.size());
	}

TEST(GraphTests, RemoveConnection)
	{
	Graph graph;
	auto p_node = graph.AddNode(NodePtr(new GraphNode));
	auto p_node_1 = graph.AddNode(NodePtr(new GraphNode));

	auto connection = graph.AddConnection(p_node, p_node_1, 0);
	graph.RemoveConnection(connection);
	
	auto connections_for_node = graph.GetConnections(p_node);
	ASSERT_TRUE(connections_for_node.empty());
	connections_for_node = graph.GetConnections(p_node_1);
	ASSERT_TRUE(connections_for_node.empty());
	}

TEST(GraphTests, GetConnection_ReturnConnectionsFromRequered)
	{
	Graph graph;
	auto p_node = graph.AddNode(NodePtr(new GraphNode));
	auto p_node_1 = graph.AddNode(NodePtr(new GraphNode));

	auto connection = graph.AddConnection(p_node, p_node_1, 0);

	auto connections_for_node = graph.GetConnections(p_node);
	ASSERT_EQ(1, connections_for_node.size());
	connections_for_node = graph.GetConnections(p_node_1);
	ASSERT_TRUE(connections_for_node.empty());
	}