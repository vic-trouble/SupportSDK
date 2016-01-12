// PerformanceTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CommandBucketTests.h"
#include "StateMachineTests.h"
#include "FactoryTests.h"

#include "TempTests.h"

int main()
{
	TempTests::Test();
	FactoryTests::Test();
	StateMachineTests::Test();
	CmdBucketTest::Test();	
    return 0;
}

