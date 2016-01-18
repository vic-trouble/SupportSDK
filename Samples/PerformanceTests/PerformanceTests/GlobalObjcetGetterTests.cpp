#include "stdafx.h"

#include <GameCore/ObjectGetterBase.h>
#include <GameCore/DefaultGlobalObjectGetter.h>

#include <time.h>

namespace GlobalObjectGetterTests
{

	struct First : public SDK::GlobalObjectBase
	{
		bool x;
	};

	struct First0 : public SDK::GlobalObjectBase
	{
		bool x;
	};

	struct First1 : public SDK::GlobalObjectBase
	{
		bool x;
	};

	struct Second : public SDK::GlobalObjectBase
	{
		bool x;
	};
	struct Second0 : public SDK::GlobalObjectBase
	{
		bool x;
	};
	struct Second1 : public SDK::GlobalObjectBase
	{
		bool x;
	};
	constexpr size_t NUMBER = 15;
	size_t indices[NUMBER];

	First g_first[NUMBER / 3];
	Second g_second[NUMBER / 3];
	First0 g_first_0[NUMBER / 3];
	Second0 g_second_0[NUMBER / 3];
	First1 g_first_1[NUMBER / 3];
	Second1 g_second_1[NUMBER / 3];

	void ConstructIndices()
	{
		for (size_t i = 0; i < NUMBER; ++i)
			indices[i] = rand() % NUMBER;
	}

	void TestGlobalVars()
	{
		constexpr static size_t NUMBER_3 = NUMBER / 3;
		for (size_t i = 0; i < NUMBER; ++i)
		{
			if (i < NUMBER_3)
			{
				g_first[i].x = true;
				g_second[i].x = false;
			}
			else if (i < NUMBER_3 * 2)
			{
				g_first_0[i].x = true;
				g_second_0[i].x = false;
			}
			else
			{
				g_first_1[i].x = true;
				g_second_1[i].x = false;
			}
		}
	}

	SDK::DefaultGlobalObjectGetter g_getter;
	void TestGetter()
	{
		constexpr static size_t NUMBER_3 = NUMBER / 3;
		for (size_t i = 0; i < NUMBER; ++i)
		{
			if (i < NUMBER_3)
			{
				g_getter.GetGlobalObject<First>()->x = true;
				g_getter.GetGlobalObject<Second>()->x = false;
			}
			else if (i < NUMBER_3 * 2)
			{
				g_getter.GetGlobalObject<First0>()->x = true;
				g_getter.GetGlobalObject<Second0>()->x = false;
			}
			else
			{
				g_getter.GetGlobalObject<First1>()->x = true;
				g_getter.GetGlobalObject<Second1>()->x = false;
			}
		}

		g_getter.GetGlobalObject<First>()->x = true;
	}

	clock_t Runner(void(*Func)(), size_t i_nums)
	{
		clock_t begin = clock();
		for (size_t i = 0; i < i_nums; ++i)
		{
			Func();
		}
		return clock() - begin;
	}

	void Test()
	{
		std::cout << "==========================================================" << std::endl
			<< "\t\t\Global objects getter tests" << std::endl;
		ConstructIndices();
		{
			constexpr static size_t NUMBER_3 = NUMBER / 3;
			for (size_t i = 0; i < NUMBER; ++i)
			{
				if (i < NUMBER_3)
				{
					g_getter.AddGlobalObject<First>();
					g_getter.AddGlobalObject<Second>();					
				}
				else if (i < NUMBER_3 * 2)
				{
					g_getter.AddGlobalObject<First0>();
					g_getter.AddGlobalObject<Second0>();
				}
				else
				{
					g_getter.AddGlobalObject<First1>();
					g_getter.AddGlobalObject<Second1>();
				}
			}
			
		}

		constexpr static size_t CALL_COUNT = 1000000;
		const clock_t gl_obj = Runner(&TestGlobalVars, CALL_COUNT);
		const clock_t getter_t = Runner(&TestGetter, CALL_COUNT);
		std::cout << "\tGlobal object: " << gl_obj << std::endl
			<< "\tGetter: " << getter_t << std::endl;
	}

} // GlobalObjectGetterTests