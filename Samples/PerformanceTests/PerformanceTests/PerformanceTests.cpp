// PerformanceTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CommandBucketTests.h"
#include "StateMachineTests.h"
#include "FSM_Test_HierarchicalvsTemplate.h"
#include "FactoryTests.h"
#include "GlobalObjectGetterTests.h"
#include "TempTests.h"
#include "MessageDispatcherTests.h"

int main()
{
	GlobalObjectGetterTests::Test();
	TempTests::Test();
	FactoryTests::Test();
	HiararchicalvsTemplate::Test();
	StateMachineTests::Test();
	CmdBucketTest::Test();	
	MessageDispatcherTests::Test();
    return 0;
}