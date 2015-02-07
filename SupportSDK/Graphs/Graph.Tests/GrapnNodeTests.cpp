#include "stdafx.h"

#include <GraphNode.h>
#include <Graph.h>
#include <Connection.h>

using namespace SDK::Graphs;

TEST(GraphNodeTests, BaseConstructor)
	{
	GraphNode node;
	EXPECT_EQ(GraphNode::INVALID_INDEX, node.GetIndex());
	EXPECT_EQ(nullptr, node.GetGraph());
	}

TEST(GraphNodeTests, BaseConstructor_IndexParameter)
	{
	Graph graph;
	GraphNode node(&graph, 1);
	EXPECT_EQ(1, node.GetIndex());
	EXPECT_EQ(&graph, node.GetGraph());
	}
/*
TEST(GraphNodeTests, AddConnection)
	{
	Graph graph;
	GraphNode node(&graph);
	Connection connection(&node, &node, 0);
	
	node.AddConnection(&connection);
	const auto& connections = node.GetConnections();
	ASSERT_EQ(1, connections.size());
	EXPECT_EQ(&connection, connections[0]);
	}

TEST(GraphNodeTests, AddConnection_ShouldNotAddIdeticalConnections)
	{
	Graph graph;
	GraphNode node(&graph);
	Connection connection(&node, &node, 0);

	node.AddConnection(&connection);
	node.AddConnection(&connection);
	const auto& connections = node.GetConnections();
	ASSERT_EQ(1, connections.size());
	EXPECT_EQ(&connection, connections[0]);
	}

TEST(GraphNodeTests, RemoveConnection)
	{
	Graph graph;
	std::unique_ptr<GraphNode> node(new GraphNode(&graph));
	Connection connection(node.get(), node.get(), 0);

	node->AddConnection(&connection);
	node->RemoveConnection(&connection);
	const auto& connections = node->GetConnections();
	ASSERT_EQ(0, connections.size());
	node.reset();
	ASSERT_FALSE(connection.IsValid());
	}

TEST(GraphNodeTests, RemoveConnection_FromEmptyConnectionList)
	{
	Graph graph;
	GraphNode node(&graph, 0);
	Connection connection(&node, &node, 0);

	node.RemoveConnection(&connection);
	const auto& connections = node.GetConnections();
	ASSERT_EQ(0, connections.size());
	}*/