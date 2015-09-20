#ifndef __GAMECORE_OPENGLRENDERER_H__
#define __GAMECORE_OPENGLRENDERER_H__

#include "IRenderer.h"

namespace SDK
{

	class OpenGLRenderer : public IRenderer
	{
	private:
		IRect				m_paint_rectangle;
#if defined(TARGET_PLATFORM_WIN32)
		HWND				mh_window;
		HDC					mh_dc;
		HGLRC				mh_rc;
#endif
		int         m_pixel_format;

	public:
#if defined(TARGET_PLATFORM_WIN32)
		OpenGLRenderer(HWND i_wnd, IRect i_paint_region);
#else
		//# error Unknown platform!
#endif
		virtual ~OpenGLRenderer();

		virtual void Initialize() override;
		virtual void Release() override;
		virtual void Reshape() override;

		virtual void	BeginFrame() override;
		virtual void	EndFrame() override;

		virtual void RenderLine(const Vector3D& i_first_point, const Vector3D& i_second_point, Color i_color, float i_width = 1.0) override;
		virtual void RenderRectangle(const Vector3D& i_center, double i_width, double i_height, Color i_color) override;
		virtual void RenderCircle(const Vector3D& i_center, double i_radius, Color i_color) override;
		virtual void RenderText(const Vector3D& i_position, const std::wstring& i_text, Color i_color) override;
		virtual void RenderText(const Vector3D& i_position, const std::string& i_text, Color i_color) override;

		virtual IRect GetTargetRectangle() override;
	};
}
#endif