#include "stdafx.h"

#include "PathFinding.h"

#include "Graph.h"
#include "GraphNode.h"
#include "Connection.h"

#include "NodeRecord.h"
#include "PathFindingList.h"

namespace SDK
	{
	namespace Graphs
		{

		/////////////////////////////////////////////////////////////////////////////////////////////
		// Dijkstra algorithm

		DijkstraPath::DijkstraPath()
			{}

		DijkstraPath::~DijkstraPath()
			{}

		ConnectionPtrs DijkstraPath::CalculatePath(ulong i_available_time, Graph& i_graph, GraphNode* ip_begin, GraphNode* ip_end)
			{
			std::vector<NodeRecord> records;
			records.reserve(100);
			// initialize record for start node
			records.emplace_back(NodeRecord(ip_begin, nullptr));
			// initialize opened and closed lists
			//	before first iteration opened list contains one record
			//		and closed list is empty
			PathFindingList opened;
			NodeRecord& p_record = records.back();
			opened += &p_record;
			PathFindingList closed;

			NodeRecord* p_current = nullptr;
			while (!opened.empty())
				{
				p_current = opened.GetSmallestElement();

				if (p_current->mp_node == ip_end)
					break;

				// loop through each connection and update opened list
				auto& connections = i_graph.GetConnections(p_current->mp_node);
				for (auto p_connection : connections)
					{
					GraphNode* p_end_node = p_connection->GetToNode();
					auto end_node_cost = p_current->m_cost + p_connection->GetCost();
					// if the node is closed already
					if (closed.Contains(p_end_node))
						continue;
					// if it is opened and we`ve found a worse route
					if (NodeRecord* p_end_node_record = opened.Find(p_end_node))
						{
						if (p_end_node_record->m_cost <= end_node_cost)
							continue;
						}
					// here we get an unvisited node
					records.push_back(NodeRecord(p_end_node, p_connection, end_node_cost));
					opened += &records.back();

					} // connections loop
				
				// current node is finished -> update opened and closed lists
				opened -= p_current;
				closed += p_current;
				} // while

			// here we can have two situations (p_current cannot be nullptr, at least it will be ip_begin)
			// 1. we have no nodes to search and we did not find our goal so
			//	there is no solution and we return empty list
			if (p_current->mp_node != ip_end || closed.empty())
				return ConnectionPtrs();
			
			// 2. we found node we search
			// compile list of connections
			ConnectionPtrs reverse_connections_path;
			while (p_current->mp_node != ip_begin)
				{
				reverse_connections_path.push_back(p_current->mp_connection);
				p_current = closed.Find(p_current->mp_connection->GetFromNode());
				}

			std::reverse(reverse_connections_path.begin(), reverse_connections_path.end());
			return reverse_connections_path;
			}

		/////////////////////////////////////////////////////////////////////////////////////////////
		// AStar algorithm


		} // Graphs
	} // SDK