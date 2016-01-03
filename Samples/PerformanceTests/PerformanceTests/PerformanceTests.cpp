// PerformanceTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CommandBucketTests.h"
#include "StateMachineTests.h"

int main()
{
	StateMachineTests::Test();
	CmdBucketTest::Test();	
    return 0;
}

