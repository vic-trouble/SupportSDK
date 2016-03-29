#ifndef __GRAPHS_PATHFINDING__
#define __GRAPHS_PATHFINDING__

#include "GraphsAPI.h"

namespace SDK
	{
	namespace Graphs
		{

		class Graph;
		class GraphNode;
		class Connection;

		typedef std::vector<Connection*> ConnectionPtrs;

		class PathFinding
			{
			public:
				virtual ~PathFinding(){}

				virtual ConnectionPtrs CalculatePath(ulong i_available_time, Graph& i_graph, GraphNode* ip_begin, GraphNode* ip_end) = 0;
			};

		class DijkstraPath : public PathFinding
			{
			public:
				GRAPHS_API DijkstraPath();
				GRAPHS_API virtual ~DijkstraPath();

				GRAPHS_API virtual ConnectionPtrs CalculatePath(ulong i_available_time, Graph& i_graph, GraphNode* ip_begin, GraphNode* ip_end) override;
			};

		class AStarPath : public PathFinding
			{
			public:
				GRAPHS_API AStarPath();
				GRAPHS_API virtual ~AStarPath();

				GRAPHS_API virtual ConnectionPtrs CalculatePath(ulong i_available_time, Graph& i_graph, GraphNode* ip_begin, GraphNode* ip_end) override;
			};

		} // Graphs
	} // SDK

#endif