#include "stdafx.h"

#include <PathfindingList.h>
#include <NodeRecord.h>
#include <GraphNode.h>

using namespace SDK::Graphs;

TEST(PathFindingListTests, CreateList_EmptyShouldBeTrue)
	{
	PathFindingList records;

	ASSERT_TRUE(records.empty());
	}

TEST(PathFindingListTests, Add_5_Records_IdenticalNodes_ListSizeEq5)
	{
	PathFindingList records;

	GraphNode node;

	NodeRecord r0(&node, nullptr);
	NodeRecord r1(&node, nullptr);
	NodeRecord r2(&node, nullptr);
	NodeRecord r3(&node, nullptr);
	NodeRecord r4(&node, nullptr);

	records += &r0;
	records += &r1;
	records += &r2;
	records += &r3;
	records += &r4;

	ASSERT_EQ(5, records.size());
	}

TEST(PathFindingListTests, AddingNewRecord_ContainShouldReturnTrue)
	{
	PathFindingList records;
	GraphNode node;

	NodeRecord record(&node, nullptr);

	records += &record;

	ASSERT_TRUE(records.Contains(&node));
	ASSERT_EQ(&record, records.Find(&node));
	ASSERT_FALSE(records.empty());
	ASSERT_EQ(&record, records.GetSmallestElement());
	}

TEST(PathFindingListTests, AddRecord_ShouldSkipIdenticalRecords)
	{
	PathFindingList records;
	GraphNode node;

	NodeRecord record(&node, nullptr);
	NodeRecord record_0(&node, nullptr);

	records += &record;
	records += &record_0;
	records += &record;

	ASSERT_EQ(2, records.size());
	}

TEST(PathFindingListTests, RemoveRecord_ShouldEraseFromList)
	{
	PathFindingList records;
	GraphNode node;

	NodeRecord record(&node, nullptr);

	records += &record;
	records -= &record;

	ASSERT_TRUE(records.empty());
	}

TEST(PathFindingListTests, GetSmallest_ShouldReturnZeroRecord)
	{
	PathFindingList records;
	GraphNode node_0;
	GraphNode node_1;

	NodeRecord record_0(&node_0, nullptr, 0.5f);
	NodeRecord record_1(&node_1, nullptr, 1.f);

	records += &record_0;
	records += &record_1;

	ASSERT_EQ(&record_0, records.GetSmallestElement());
	}