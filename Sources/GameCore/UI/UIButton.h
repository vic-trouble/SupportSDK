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
			Render::IndexBufferHandle m_indices;
			Render::VertexBufferHandle m_hd_buffer;
			Render::VertexLayoutHandle m_ver_layout;
			Render::VertexLayoutHandle m_uv_layout;

			Render::VertexBufferHandle m_vertices;
			Render::VertexBufferHandle m_uvs;

			enum class State
			{
				Idle,
				Hovered,
				Pressed
			};
			State m_state;

			Render::TextureHandle m_tex_handles[3];

		private:
			virtual void UpdateImpl(float i_elapsed_time) override;
			virtual void DrawImpl(Render::RenderCommandBucket& i_bucket) override;
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

