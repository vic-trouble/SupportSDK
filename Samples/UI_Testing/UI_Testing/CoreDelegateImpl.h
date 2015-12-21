#pragma once

#include <GameCore/CoreDelegate.h>
#include <GameCore/UI/UIScreen.h>
#include <GameCore/UI/UIButton.h>
namespace Game
{
	class CoreDelegateImpl : public SDK::CoreDelegate
	{
	private:
		SDK::UI::UIScreen* mp_screen;

		SDK::UI::UIButton* mp_button;

	public:
		virtual ~CoreDelegateImpl(){}

		virtual void OnCreate() override;
		virtual void OnTerminate() override;

		virtual void OnCollapse() override
		{
			std::cout << "Collapse";
		}
		virtual void OnActivate() override
		{
			std::cout << "Activate";
		}

		virtual void Update(float i_elapsed_time) override;
		virtual void Draw() override;
	};
}