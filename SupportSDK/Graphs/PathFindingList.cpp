#include "stdafx.h"

#include "PathfindingList.h"

#include "GraphNode.h"
#include "NodeRecord.h"

namespace SDK
	{
	namespace Graphs
		{

		PathFindingList::PathFindingList()
			{}

		PathFindingList::~PathFindingList()
			{}

		PathFindingList& PathFindingList::operator += (NodeRecord* ip_record)
			{
			if (m_records.end() == std::find(m_records.begin(), m_records.end(), ip_record))
				m_records.emplace_back(ip_record);
			return *this;
			}

		PathFindingList& PathFindingList::operator -= (NodeRecord* ip_record)
			{
			m_records.erase(std::remove(m_records.begin(), m_records.end(), ip_record), m_records.end());
			return *this;
			}

		NodeRecord* PathFindingList::GetSmallestElement()
			{
			int smallest_cost = std::numeric_limits<int>::max();
			NodeRecord* p_smallest = nullptr;
			for (NodeRecord* p_record : m_records)
				{
				if (p_record->m_cost < smallest_cost)
					{
					smallest_cost = p_record->m_cost;
					p_smallest = p_record;
					}
				}
			return p_smallest;
			}

		bool PathFindingList::Contains(GraphNode* ip_node)
			{
			return Find(ip_node) != nullptr;
			}

		NodeRecord* PathFindingList::Find(GraphNode* ip_node)
			{
			auto it = std::find_if(m_records.begin(), m_records.end(), [ip_node](NodeRecord* p_record)
				{
				return p_record->mp_node == ip_node;
				});

			if (it != m_records.end())
				return *it;

			return nullptr;
			}

		} // Graphs
	} // SDK