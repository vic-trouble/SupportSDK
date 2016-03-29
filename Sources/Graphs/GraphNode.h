#ifndef __GRAPHNODE_H__
#define __GRAPHNODE_H__

#include "GraphsAPI.h"

namespace SDK
	{
	namespace Graphs
		{
		
		class Connection;
		class Graph;

		class GraphNode
			{
			friend class Graph;
			public:
				static const int INVALID_INDEX = -1;

			private:
				int m_index;
				Graph* mp_graph;

				//std::vector<Connection*> m_connections;
				
			public:
				GRAPHS_API GraphNode();
				GRAPHS_API GraphNode(Graph* ip_graph, int i_index);
				GRAPHS_API virtual ~GraphNode();

				int GetIndex() const { return m_index; }
				Graph* GetGraph() const { return mp_graph; }
				/*
				temporary erase this from functionality
				 reason: it is time to synchronize state in graph and here
					also it is waste of space of PC
					if the performance issues will appear as for getting connections
					from graph->GetConnections(node) than it should be added
				GRAPHS_API void AddConnection(Connection* ip_connection);
				GRAPHS_API void RemoveConnection(Connection* ip_connection);
				const std::vector<Connection*>& GetConnections() const { return m_connections; }*/
			};

		} // Graphs
	} // SDK

#endif