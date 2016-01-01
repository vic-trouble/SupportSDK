#include "stdafx.h"

#include "CommandBucketTests.h"

#include <GameCore/Render/Commands.h>
#include <GameCore/Render/CommandBucket.h>

namespace CmdBucketTest
{
	std::vector<int> g_core;
	std::vector<int> g_inh;

	template <typename T, typename CmdType>
	void AddToBucket(T& o_bucket)
	{
		for (size_t i = 0; i < 10000000; ++i)
		{
			auto p_cmd = o_bucket.Create<CmdType>();
			p_cmd->data = 1;
		}
	}

	template <typename T>
	void Submit(T& o_bucket)
	{
		o_bucket.Submit();
	}
	
	std::pair<TestData, TestData> Test()
	{
		CommandBucket<int> cmd_bucket;
		SDK::Render::Commands::CommandBucket<int> core_bucket;
		TestData cmd;
		TestData core;
		auto beg = clock();
		AddToBucket<SDK::Render::Commands::CommandBucket<int>, SDK::Render::TransformCore>(core_bucket);
		core.add_time = clock() - beg;

		beg = clock();
		AddToBucket<CommandBucket<int>, TransformCmd>(cmd_bucket);
		cmd.add_time = clock() - beg;

		beg = clock();
		Submit(core_bucket);
		core.submit = clock() - beg;

		beg = clock();
		Submit(cmd_bucket);
		cmd.submit = clock() - beg;

		std::cout << "Add time: inh[" << cmd.add_time << "]-CStyle[" << core.add_time << "]" << std::endl
			<< "\t inh/CStyle=" << (1.f*cmd.add_time)/core.add_time << std::endl;
		std::cout << "Submit time: inh[" << cmd.submit << "]-CStyle[" << core.submit << "]" << std::endl
			<< "\t inh/CStyle=" << (1.f*cmd.submit) / core.submit << std::endl;;

		return std::make_pair(cmd, core);
	}
}