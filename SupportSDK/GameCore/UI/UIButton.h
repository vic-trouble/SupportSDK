#ifndef	__GAMECORE_UIBUTTON_H__
#define __GAMECORE_UIBUTTON_H__

#include "UIControl.h"
#include "../Render/VertexDeclaration.h"

namespace SDK
{
	namespace UI
	{

		class UIButton : public UIControl
		{
		private:
			Render::Batch m_batch;

		private:
			virtual void UpdateImpl(float i_elapsed_time) override;
			virtual void DrawImpl() override;
			virtual void LoadImpl(const PropertyElement& i_element) override;

		public:
			GAMECORE_EXPORT UIButton();
			GAMECORE_EXPORT virtual ~UIButton();

			virtual void MouseMoved(MessageDispatcher& o_ui_dispatcher, const MouseEvent& i_evt) override;
			virtual void MousePressed(MessageDispatcher& o_ui_dispatcher, const MouseEvent& i_evt) override;
			virtual void MouseReleased(MessageDispatcher& o_ui_dispatcher, const MouseEvent& i_evt) override;
		};

	} // UI
} // SDK

#endif

