#ifndef __PATTERNS_STATICSINGLETON_H__
#define __PATTERNS_STATICSINGLETON_H__

namespace SDK
{

	/*
	Instance creates at first time user calls Instance
	*/

	template <typename T>
	class StaticSingleton
	{
	private:
		StaticSingleton(){}
		~StaticSingleton(){}

		StaticSingleton(const StaticSingleton& other) = delete;
		StaticSingleton& operator = (const StaticSingleton& other) = delete;
	public:
		static T& Instance()
		{
			static T g_instance;
			return g_instance;
		}
	};

} // SDK

#endif