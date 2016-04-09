// PerformanceTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CommandBucketTests.h"
#include "StateMachineTests.h"
#include "FactoryTests.h"
#include "GlobalObjectGetterTests.h"
#include "TempTests.h"

int main()
{
	for (int i = 0; i < 10; ++i)
		GlobalObjectGetterTests::Test();
	//TempTests::Test();
	//FactoryTests::Test();
	//StateMachineTests::Test();
	//CmdBucketTest::Test();	
    return 0;
}