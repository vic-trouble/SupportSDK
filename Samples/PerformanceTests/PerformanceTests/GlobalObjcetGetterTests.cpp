#include "stdafx.h"

#include <GameCore/ObjectStorageBase.h>
#include <GameCore/DefaultObjectsStorage.h>

#include <time.h>

namespace GlobalObjectGetterTests
{

	struct Dummy : public SDK::GlobalObjectBase
	{
		bool x;
	};
	struct Dummy1 : public SDK::GlobalObjectBase
	{
		bool x;
	};
	struct Dummy2 : public SDK::GlobalObjectBase
	{
		bool x;
	};
	struct Dummy3 : public SDK::GlobalObjectBase
	{
		bool x;
	};
	struct Dummy4 : public SDK::GlobalObjectBase
	{
		bool x;
	};

	struct First : public SDK::GlobalObjectBase
	{
		int x;
		void SetX(bool val)
		{
			x = rand()%1;
		}
	};

	struct First0 : public SDK::GlobalObjectBase
	{
		int x;
		void SetX(bool val)
		{
			x = rand() % 1;
		}
	};

	struct First1 : public SDK::GlobalObjectBase
	{
		int x;
		void SetX(bool val)
		{
			x = rand() % 1;
		}
	};

	struct Second : public SDK::GlobalObjectBase
	{
		int x;
		void SetX(bool val)
		{
			x = rand() % 1;
		}
	};
	struct Second0 : public SDK::GlobalObjectBase
	{
		int x;
		void SetX(bool val)
		{
			x = rand() % 1;
		}
	};
	struct Second1 : public SDK::GlobalObjectBase
	{
		int x;
		void SetX(bool val)
		{
			x = rand() % 1;
		}
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
				g_first[i].SetX(true);
				g_second[i].SetX(false);
			}
			else if (i < NUMBER_3 * 2)
			{
				g_first_0[i].SetX(true);
				g_second_0[i].SetX(false);
			}
			else
			{
				g_first_1[i].SetX(true);
				g_second_1[i].SetX(false);
			}
		}
	}

	void TestGetter(SDK::ObjectStorageBase& i_getter)
	{
		constexpr static size_t NUMBER_3 = NUMBER / 3;
		for (size_t i = 0; i < NUMBER; ++i)
		{
			if (i < NUMBER_3)
			{
				i_getter.GetGlobalObject<First>()->SetX(true);
				i_getter.GetGlobalObject<Second>()->SetX(false);
			}
			else if (i < NUMBER_3 * 2)
			{
				i_getter.GetGlobalObject<First0>()->SetX(true);
				i_getter.GetGlobalObject<Second0>()->SetX(false);
			}
			else
			{
				i_getter.GetGlobalObject<First1>()->SetX(true);
				i_getter.GetGlobalObject<Second1>()->SetX(false);
			}
		}
	}

	clock_t Runner(std::function<void()> func, size_t i_nums)
	{
		clock_t begin = clock();
		for (size_t i = 0; i < i_nums; ++i)
		{
			func();
		}
		return clock() - begin;
	}

	void Test()
	{
		std::cout << "==========================================================" << std::endl
			<< "\t\tGlobal objects getter tests" << std::endl;
		ConstructIndices();
		SDK::DefaultObjectsStorage g_getter;
		{
			constexpr static size_t NUMBER_3 = NUMBER / 3;
			g_getter.AddGlobalObject<First>();
			g_getter.AddGlobalObject<Dummy>();
			g_getter.AddGlobalObject<Second>();
			g_getter.AddGlobalObject<Dummy1>();
			g_getter.AddGlobalObject<First0>();
			g_getter.AddGlobalObject<Dummy2>();
			g_getter.AddGlobalObject<Second0>();
			g_getter.AddGlobalObject<Dummy3>();
			g_getter.AddGlobalObject<First1>();
			g_getter.AddGlobalObject<Dummy4>();
			g_getter.AddGlobalObject<Second1>();
		}

		constexpr static size_t CALL_COUNT = 100000;
		const clock_t gl_obj = Runner(&TestGlobalVars, CALL_COUNT);
		const clock_t getter_t = Runner([&g_getter]() { TestGetter(g_getter); }, CALL_COUNT);
		std::cout << "\tGlobal object: " << gl_obj << std::endl
			<< "\tGetter: " << getter_t << std::endl;
	}

} // GlobalObjectGetterTests