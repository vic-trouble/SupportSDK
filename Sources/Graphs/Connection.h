#ifndef __GRAPHS_CONNECTION_H__
#define __GRAPHS_CONNECTION_H__

namespace SDK
	{
	namespace Graphs
		{

		class GraphNode;

		class Connection
			{
			private:
				GraphNode*	mp_from;
				GraphNode*	mp_to;
				float				m_cost;

			public:
				Connection(GraphNode* ip_from_node, GraphNode* ip_to_node, float i_cost)
					: mp_from(ip_from_node)
					, mp_to(ip_to_node)
					, m_cost(i_cost)
					{}
				virtual ~Connection(){}
				float				GetCost() const { return m_cost; }
				GraphNode*	GetFromNode() const { return mp_from; }
				GraphNode*	GetToNode() const { return mp_to; }
			};

		} // Graphs
	} // SDK

#endif