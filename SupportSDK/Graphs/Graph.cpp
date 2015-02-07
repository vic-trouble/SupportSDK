#include "stdafx.h"

#include "Graph.h"
#include "GraphNode.h"
#include "Connection.h"

namespace SDK
	{
	namespace Graphs
		{

		Graph::Graph()
			: m_next_index(0)
			{

			}

		Graph::~Graph()
			{}

		ConnectionConstPtrs Graph::GetConnections(GraphNode* ip_node) const
			{
			ConnectionConstPtrs connections;

			for (const auto& connection : m_connections)
				{
				if (connection.GetFromNode() == ip_node || connection.GetToNode() == ip_node)
					connections.emplace_back(&connection);
				}

			return connections;
			}

		GraphNode* Graph::AddNode(NodePtr ip_node)
			{
			GraphNode* p_node = ip_node.get();
			p_node->m_index = m_next_index++;
			p_node->mp_graph = this;
			m_nodes.push_back(std::move(ip_node));
			return p_node;
			}

		GraphNode* Graph::GetNode(int i_index) const
			{
			auto  it = std::find_if(m_nodes.begin(), m_nodes.end(), [i_index](const NodePtr& p_node)
				{
				return p_node->GetIndex() == i_index;
				});
			if (it != m_nodes.end())
				return it->get();
			return nullptr;
			}

		void Graph::RemoveNode(GraphNode* ip_node)
			{
			m_connections.erase(
				std::remove_if(m_connections.begin(), m_connections.end(), [ip_node](const Connection& connection){ return connection.GetFromNode() == ip_node || connection.GetToNode() == ip_node; }),
				m_connections.end());

			m_nodes.erase(
				std::find_if(m_nodes.begin(), m_nodes.end(), [ip_node](const NodePtr& p_node)
				{
				return p_node.get() == ip_node;
				}),
					m_nodes.end()
				);
			}

		Connection*	Graph::AddConnection(GraphNode* ip_from, GraphNode* ip_to, int i_cost)
			{
			m_connections.emplace_back(Connection(ip_from, ip_to, i_cost));
			Connection* p_connection = &m_connections.back();
			return p_connection;
			}

		void Graph::RemoveConnection(Connection* ip_connection)
			{
			auto it = std::find_if(m_connections.begin(), m_connections.end(), [ip_connection](const Connection& connection)
				{
				return &connection == ip_connection;
				});

			if (it != m_connections.end())
				m_connections.erase(it, m_connections.end());
			}

		} // Graphs
	} // SDK