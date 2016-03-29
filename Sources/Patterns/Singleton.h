#ifndef __PATTERNS_SINGLETON_H__
#define __PATTERNS_SINGLETON_H__

#if defined(_USRDLL)
#pragma warning ( disable : 4661)
#endif

namespace SDK
{

	/*
	Singleton with controlled time of life
	usage Singleton<Type>::Instance
	*/

	template <typename T>
	class Singleton
	{
	private:
		static T* mp_instance;

	public:
		Singleton()
		{
			if (mp_instance == nullptr)
				mp_instance = static_cast<T*>(this);
		}

		virtual ~Singleton()
		{
			mp_instance = nullptr;
		}

		static T& Instance() { return *mp_instance; }
		static T* InstancePtr()
		{
			return mp_instance; 
		}

		void Release()
		{
			if (mp_instance != this)
				delete this;
		}
	};
// not to create many instances of T type
// if compiled with lib version - when we can use this
#if !defined(_USRDLL)
	template <typename T>
	T* Singleton<T>::mp_instance = nullptr;
#else
#define DefineSingletonInstance(T) \
		template <> \
		T* SDK::Singleton<T>::mp_instance = nullptr;
#endif

} // SDK

#endif