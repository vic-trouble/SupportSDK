#ifndef __NODERECORD_H__
#define __NODERECORD_H__

namespace SDK
	{
	namespace Graphs
		{
		class GraphNode;
		class Connection;

		struct NodeRecord
			{
			GraphNode* mp_node;
			Connection* mp_connection;
			float m_cost;
			NodeRecord(GraphNode* ip_node, Connection* ip_connection)
				: mp_node(ip_node)
				, mp_connection(ip_connection)
				, m_cost(0)
				{}
			NodeRecord(GraphNode* ip_node, Connection* ip_connection, float i_cost)
				: mp_node(ip_node)
				, mp_connection(ip_connection)
				, m_cost(i_cost)
				{}
			};

		} // Graphs
	} // SDK

#endif