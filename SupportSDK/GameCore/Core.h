#ifndef __GAMECORE_CORE_H__
#define __GAMECORE_CORE_H__

#include "GameCoreAPI.h"
#include "ObjectGetterBase.h"

namespace SDK
{

	class ApplicationBase;
	class CoreDelegate;
	class IRenderer;

	// todo: what we need...
	class Core
	{
	private:
		static std::unique_ptr<CoreDelegate> mp_delegate;
		static std::unique_ptr<ApplicationBase> mp_application;
		static std::unique_ptr<IRenderer> mp_renderer;
		static std::unique_ptr<ObjectGetterBase> mp_object_getter;

	private:
		static void SetApplication(std::unique_ptr<ApplicationBase> ip_application);
		static void ReleaseSingletons();

	public:
		static GAMECORE_EXPORT void Run(std::unique_ptr<CoreDelegate>&& ip_delegate);
		static GAMECORE_EXPORT void DestroyApplication();
		static GAMECORE_EXPORT CoreDelegate* GetDelegate();
		static GAMECORE_EXPORT ApplicationBase* GetApplication();

		static void SetupRenderer(std::unique_ptr<IRenderer> ip_renderer);
		static GAMECORE_EXPORT IRenderer* GetRenderer();
		
		static GAMECORE_EXPORT void Update(float i_elapsed_time);
		static GAMECORE_EXPORT void Draw();

		static GAMECORE_EXPORT void CreateSingletons();

		static void SetGlobalObjectGetter(std::unique_ptr<ObjectGetterBase> ip_getter)
		{
			mp_object_getter = std::move(ip_getter);
		}
		static ObjectGetterBase* GetGlobalObjectGetter() { return mp_object_getter.get(); }
	};

} // SDK

#endif