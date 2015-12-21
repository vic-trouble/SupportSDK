#ifndef	__GAMECORE_UISCREEN_H__
#define __GAMECORE_UISCREEN_H__

#include "UIControl.h"

namespace SDK
{
	namespace UI
	{

		class UIScreen : public UIControl
		{
		private:


		private:
			virtual void UpdateImpl(float i_elapsed_time) override;
			virtual void DrawImpl() override;

		public:
			GAMECORE_EXPORT UIScreen();
			GAMECORE_EXPORT virtual ~UIScreen();

			virtual void Load(const PropertyElement& element) override {}
		};

	} // UI
} // SDK

#endif
