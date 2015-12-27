#ifndef __GAMECORE_OPENGLRENDERER_H__
#define __GAMECORE_OPENGLRENDERER_H__

#include "Render/IRenderer.h"

#include "HardwareBufferManager.h"
#include "GLShaderCompiler.h"

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

		Render::HardwareBufferManager m_hardware_buffer_mgr;
		Render::GLShaderCompiler m_shader_compiler;
		
		MatrixMode m_current_mode;
		Matrix4f m_matrices[2];

	private:
		// TODO: more precise configurations
		void ConfigureGl();

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

		virtual void Draw(Render::Batch i_decl) override;

		virtual void SetProjectionMatrix(Matrix4f&& i_projection_matrix) override;
		virtual void SetModelViewMatrix(Matrix4f&& i_modelview_matrix) override;
		virtual void SetProjectionType(Render::ProjectionType i_projection_type) override;

		virtual Render::HardwareBufferManager* GetHardwareBufferMgr() override { return &m_hardware_buffer_mgr; }
		virtual Render::ShaderCompiler* GetShaderCompiler() override { return &m_shader_compiler; }

		virtual void PushMatrix() override;
		virtual void PopMatrix() override;
		virtual void SetMatrixMode(MatrixMode i_matrix_mode) override;
		virtual void SetCurrentMatrix(const Matrix4f& i_translation_matrix) override;

		////////////////////////////////////////////////////////////////////
		// TODO: is it still needed

		virtual void RenderLine(const Vector3& i_first_point, const Vector3& i_second_point, Color i_color, float i_width = 1.0) override;
		virtual void RenderRectangle(const Vector3& i_center, double i_width, double i_height, Color i_color) override;
		virtual void RenderCircle(const Vector3& i_center, double i_radius, Color i_color) override;
		virtual void RenderText(const Vector3& i_position, const std::wstring& i_text, Color i_color) override;
		virtual void RenderText(const Vector3& i_position, const std::string& i_text, Color i_color) override;

		virtual IRect GetTargetRectangle() override;
	};
}
#endif