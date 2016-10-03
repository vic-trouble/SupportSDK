#include "stdafx.h"

#include "FactoryTests.h"

#include <Patterns/Factory/Factory.h>

namespace FactoryTests
{
	namespace VariadicArgsInCreator
	{
		class Base
		{
		public:
			virtual ~Base() {}
			size_t GetType() const
			{
				return typeid(*this).hash_code();
			}
		};
		struct MyParameters
		{
			int x = 0;
		};
		using TestFactory = SDK::Factory<Base, std::string, std::unique_ptr<Base>, SDK::DefaultErrorPolicy, &SDK::Utilities::hash_function<std::string>,
			MyParameters&, const MyParameters&>;

		class Test1 : public Base
		{
		public:
			void Init(const MyParameters&)
			{}
		};

		class Test2 : public Base
		{
		public:
			void Init(const MyParameters&)
			{}
		};

		template <typename T>
		std::unique_ptr<Base> CreateType(const MyParameters& params)
		{
			std::unique_ptr<T> p_obj(new T());
			p_obj->Init(params);
			return std::move(p_obj);
		}

		template <typename T>
		void Write(MyParameters& o_writer, const T& obj)
		{
			o_writer.x = 1;
		}

		void Test()
		{
			TestFactory factory;
			factory.Register("test1", &CreateType<Test1>);
			factory.Register("test2", &CreateType<Test2>);
			
			factory.Register<Test1>(&Write<Test1>);
			auto p_test1 = factory.Create("test1", MyParameters());
			auto p_test2 = factory.Create("test2", MyParameters());

			MyParameters out;
			factory.Write(out, *p_test1);
		}

	} // VariadicArgsInCreator

	class Base
	{
	public:
		virtual ~Base() {}

		virtual std::string Name() = 0;
	};

	class Monster : public Base
	{
	public:
		virtual std::string Name() override
		{
			return "Monster";
		}
	};

	class Car : public Base
	{
	public:
		virtual std::string Name() override
		{
			return "Car";
		}
	};

	class SpaceShip : public Base
	{
	public:
		virtual std::string Name() override
		{
			return "SpaceShip";
		}
	};

	std::unique_ptr<Base> CreateMonster()
	{
		return std::make_unique<Monster>();
	}

	std::unique_ptr<Base> CreateSpaceShip()
	{
		return std::make_unique<SpaceShip>();
	}

	std::unique_ptr<Base> CreateCar()
	{
		return std::make_unique<Car>();
	}

	const static std::string TestData[] = {
		"monster",
		"car",
		"space_ship"
		,
		"monster1",
		"car1",
		"space_ship1",

		"monster2",
		"car2",
		"space_ship2",

		"monster3",
		"car3",
		"space_ship3",
	};

	enum class Types
	{
		Monster,
		Car,
		Space_ship
	};

	const static int TestData_int[] = {
		static_cast<int>(Types::Monster),
		static_cast<int>(Types::Car),
		static_cast<int>(Types::Space_ship),

		static_cast<int>(Types::Monster),
		static_cast<int>(Types::Car),
		static_cast<int>(Types::Space_ship),

		static_cast<int>(Types::Monster),
		static_cast<int>(Types::Car),
		static_cast<int>(Types::Space_ship),

		static_cast<int>(Types::Monster),
		static_cast<int>(Types::Car),
		static_cast<int>(Types::Space_ship)
	};

	static std::unique_ptr<Base>(*CreateFunc[])() = {
		&CreateMonster,
		&CreateCar,
		&CreateSpaceShip,
		
		&CreateMonster,
		&CreateCar,
		&CreateSpaceShip,

		&CreateMonster,
		&CreateCar,
		&CreateSpaceShip,

		&CreateMonster,
		&CreateCar,
		&CreateSpaceShip,
	};

	template <typename KeyType>
	std::vector<std::unique_ptr<Base>> TestFactory(const KeyType* ip_test_array, size_t i_num, size_t i_obj_to_create)
	{
		SDK::Factory<Base, KeyType> factory;
		for (size_t i = 0; i < i_num; ++i)
			factory.Register(ip_test_array[i], CreateFunc[i]);
		
		std::vector<std::unique_ptr<Base>> objects;
		static const size_t buffer_count = 100000;
		objects.resize(buffer_count);
		for (size_t i = 0; i < i_obj_to_create; ++i)
			objects[i%buffer_count] = std::move(factory.Create(ip_test_array[i%i_num]));
		return objects;
	}

	/*
	template <typename KeyType>
	std::vector<std::unique_ptr<Base>> TestFactoryTest(const KeyType* ip_test_array, size_t i_num, size_t i_obj_to_create)
	{
		SDK::Factory<Base, KeyType> factory;
		for (size_t i = 0; i < i_num; ++i)
			factory.Register(ip_test_array[i], CreateFunc[i]);

		std::vector<std::unique_ptr<Base>> objects;
		for (size_t i = 0; i < i_obj_to_create; ++i)
			objects.push_back(std::move(factory.CreateTest(ip_test_array[i%i_num])));
		return objects;
	}
	*/
	void Out(const std::vector<std::unique_ptr<Base>>& i_objects)
	{
		for (const auto& obj : i_objects)
			std::cout << obj->Name() << std::endl;
	}

	void Test()
	{
		static const size_t OBJ_NUMBER = 100000;
		std::cout << "==========================================================" << std::endl
			<< "\t\t\tFactory tests" << std::endl;
		/*size_t sum = 0;
		clock_t begint = clock();
		for (int i = 0; i < (int)OBJ_NUMBER; ++i)
			sum += SDK::Utilities::hash_function(i);
		clock_t endt = clock() - begint;
		std::cout << "Int hash - " << sum << "-time:" << endt << std::endl;
		std::string x("adqwerqwr");
		begint = clock();
		for (int i = 0; i < (int)OBJ_NUMBER; ++i)
			sum += SDK::Utilities::hash_function(x);
		endt = clock() - begint;
		std::cout << "Sting hash - "  << sum << "-time: " << endt << std::endl;*/
		clock_t begin = clock();
		auto objects = TestFactory<std::string>(TestData, sizeof(TestData)/sizeof(std::string), OBJ_NUMBER);
		clock_t end = clock() - begin;
		std::cout << "\tString: " << OBJ_NUMBER << "-" << end << std::endl;
		//Out(objects);
		begin = clock();
		//objects = TestFactoryTest<std::string>(TestData, sizeof(TestData) / sizeof(std::string), OBJ_NUMBER);
		objects = TestFactory<int>(TestData_int, sizeof(TestData_int) / sizeof(int), OBJ_NUMBER);
		end = clock() - begin;
		std::cout << "\tInt: " << OBJ_NUMBER << "-" << end << std::endl;
		//Out(objects);
		std::cout << "==========================================================" << std::endl;
		VariadicArgsInCreator::Test();
	}

} // AbstractFactoryTests