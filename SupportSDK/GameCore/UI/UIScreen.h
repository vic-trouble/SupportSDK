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
			virtual void LoadImpl(const PropertyElement& element) override;

		public:
			GAMECORE_EXPORT UIScreen();
			GAMECORE_EXPORT virtual ~UIScreen();
		};

	} // UI
} // SDK

#endif
