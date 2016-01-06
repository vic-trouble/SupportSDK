// PerformanceTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CommandBucketTests.h"
#include "StateMachineTests.h"
#include "FactoryTests.h"

int main()
{
	FactoryTests::Test();
	StateMachineTests::Test();
	CmdBucketTest::Test();	
    return 0;
}

