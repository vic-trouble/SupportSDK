#ifndef __PATTERNS_SINGLETON_H__
#define __PATTERNS_SINGLETON_H__

namespace SDK
	{

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

			static T& Instance() {	return *mp_instance;	}
			static T* InstancePtr() { return mp_instance; }

			static void ReleaseIfValid()
				{
				if (mp_instance != nullptr)
					delete mp_instance;
				}
		};

	template <typename T>
	T* Singleton<T>::mp_instance = nullptr;

	}

#endif