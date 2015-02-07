#ifndef __PATHFINDINGLIST_H__
#define __PATHFINDINGLIST_H__

#include "GraphsAPI.h"

namespace SDK
	{
	namespace Graphs
		{

		class GraphNode;
		struct NodeRecord;

		class PathFindingList
			{
			private:
				std::vector<NodeRecord*> m_records;

			public:
				GRAPHS_API PathFindingList();
				GRAPHS_API virtual ~PathFindingList();

				GRAPHS_API PathFindingList& operator += (NodeRecord* ip_record);
				GRAPHS_API PathFindingList& operator -= (NodeRecord* ip_record);

				GRAPHS_API NodeRecord*		GetSmallestElement();
				GRAPHS_API bool					Contains(GraphNode* ip_node);
				GRAPHS_API NodeRecord*		Find(GraphNode* ip_node);

				bool					empty() const { return m_records.empty(); }
			};

		} // Graphs
	} // SDK

#endif