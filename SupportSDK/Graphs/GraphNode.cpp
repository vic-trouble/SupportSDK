#include "stdafx.h"

#include "GraphNode.h"
#include "Connection.h"

namespace SDK
	{
	namespace Graphs
		{

		GraphNode::GraphNode()
			: mp_graph(nullptr)
			, m_index(INVALID_INDEX)
			{}

		GraphNode::GraphNode(Graph* ip_graph, int i_index)
			: mp_graph(ip_graph)
			, m_index(i_index)
			{}

		GraphNode::~GraphNode()
			{}

		/*void GraphNode::AddConnection(Connection* ip_connection)
			{
			auto it = std::find(m_connections.begin(), m_connections.end(), ip_connection);
			if (it == m_connections.end())
				m_connections.push_back(ip_connection);
			}

		void GraphNode::RemoveConnection(Connection* ip_connection)
			{
			m_connections.erase(
				std::find(m_connections.begin(), m_connections.end(), ip_connection),
				m_connections.end()
				);
			}*/

		} // Graphs
	} // SDK