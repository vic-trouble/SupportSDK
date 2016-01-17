#include "stdafx.h"

#include "OpenGLRenderer.h"
#include "GlUitlities.h"

#include <Math/VectorConstructor.h>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include <string>

#define M_PI 3.14159265358979323846

//////////////////////////////////////////////////////////////////////////

namespace
{

	struct GLColor
	{
		float color[4];

		float& operator[](int i)
		{
			assert(i < 4);
			return color[i];
		}

	};

}

template <>
void SDK::Color::Convert<GLColor>(GLColor i_color_from)
{
	unsigned int r = static_cast<unsigned int>(i_color_from[0] * 255.f);
	unsigned int g = static_cast<unsigned int>(i_color_from[1] * 255.f);
	unsigned int b = static_cast<unsigned int>(i_color_from[2] * 255.f);
	unsigned int a = static_cast<unsigned int>(i_color_from[3] * 255.f);
	m_color = r << 24 | g << 16 | b << 8 | a;
}

template<>
GLColor SDK::Color::Convert<GLColor>()
{
	GLColor gl_color;
	gl_color[0] = static_cast<unsigned char>(m_color >> 24) / 255.f;
	gl_color[1] = static_cast<unsigned char>(m_color >> 16) / 255.f;
	gl_color[2] = static_cast<unsigned char>(m_color >> 8) / 255.f;
	gl_color[3] = static_cast<unsigned char>(m_color) / 255.f;

	return gl_color;
}

namespace
{

	/*std::string ExePath()
	{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
	}

	void _InitFreeType(freetype::font_data& o_font_data)
	{
	o_font_data.init((ExePath() + "\\font.TTF").c_str(), 8);
	}*/

	void _Clear(SDK::Color i_color)
	{
		GLColor color = i_color.Convert<GLColor>();
		glClearColor(color[0], color[1], color[2], color[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

} // namespace
#include "../ShaderSystem.h"
namespace SDK
{

	OpenGLRenderer::OpenGLRenderer(HWND i_wnd, IRect i_paint_region)
		: m_paint_rectangle(i_paint_region)
		, mh_window(i_wnd)
		, mh_dc(nullptr)
		, mh_rc(nullptr)
	{
	}

	OpenGLRenderer::~OpenGLRenderer()
	{

	}

	void OpenGLRenderer::ConfigureGl()
	{
		glShadeModel(GL_SMOOTH);                        // shading mathod: GL_SMOOTH or GL_FLAT
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);          // 4-byte pixel alignment

		// enable /disable features
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glEnable(GL_SCISSOR_TEST);

		// track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);

		glClearColor(0.f, 0.f, 0.f, 0.f);   // background color
		glClearStencil(0);                              // clear stencil buffer
		glClearDepth(1.0f);                             // 0 is near, 1 is far
		glDepthFunc(GL_LEQUAL);
	}

	// TODO: 
	//	 Can we make all flags like FlagPair
	///////////////////////////////////////////////////////////
	typedef std::pair<GLenum, bool> FlagPair;
	FlagPair default_flags[] = {
		{GL_LIGHTING, true},
		{GL_COLOR_MATERIAL, true},
		{GL_DEPTH_TEST, true},
		{GL_TEXTURE_2D, true},
		{GL_BLEND, true},
		{GL_SCISSOR_TEST, true}
	};

	void Apply(FlagPair i_pair)
	{
		if (i_pair.second)
			glEnable(i_pair.first);
		else
			glDisable(i_pair.first);
	}
	///////////////////////////////////////////////////////////

	void OpenGLRenderer::Initialize()
	{
		mh_dc = GetDC(mh_window);

		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;
		int pixel_format = ChoosePixelFormat(mh_dc, &pfd);
		SetPixelFormat(mh_dc, pixel_format, &pfd);

		mh_rc = wglCreateContext(mh_dc);
		m_render_context_id = reinterpret_cast<SDK::uint64>(mh_rc);

		wglMakeCurrent(mh_dc, mh_rc);

		glewInit();
		ConfigureGl();
	}

	void OpenGLRenderer::Release()
	{
		wglMakeCurrent(0, 0);
		wglDeleteContext(mh_rc);
		ReleaseDC(mh_window, mh_dc);
	}

	IRect OpenGLRenderer::GetTargetRectangle()
	{
		return m_paint_rectangle;
	}

	void OpenGLRenderer::Reshape()
	{
		RECT rc;
		GetClientRect(mh_window, &rc);

		const int width = rc.right - rc.left;
		const int height = rc.bottom - rc.top;
		m_paint_rectangle.SetCenter(Vector2i{ rc.left + width / 2, rc.bottom + height / 2 });
		m_paint_rectangle.SetSize(width, height);

		glViewport(0, 0, m_paint_rectangle.Width(), m_paint_rectangle.Height());

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	/////////////////////////////////////////////////////////////////////////////

	GLenum GetPrimitiveType(Render::PrimitiveType i_type)
	{
		using namespace Render;
		switch (i_type)
		{
			case PrimitiveType::Points:
				return GL_POINTS;
			case PrimitiveType::LineStrip:
				return GL_LINE_STRIP;
			case PrimitiveType::LineLoop:
				return GL_LINE_LOOP;
			case PrimitiveType::Lines:
				return GL_LINES;
			case PrimitiveType::TriangleStrip:
				return GL_TRIANGLE_STRIP;
			case PrimitiveType::TriangleFan:
				return GL_TRIANGLE_FAN;
			case PrimitiveType::Triangles:
				return GL_TRIANGLES;
			case PrimitiveType::QuadStrip:
				return GL_QUAD_STRIP;
			case PrimitiveType::Quads:
				return GL_QUADS;
			case PrimitiveType::Polygon:
				return GL_POLYGON;
			
		}

		return 0;
	}

	GLenum GetComponentType(Render::ComponentType i_type)
	{
		using namespace Render;
		switch (i_type)
		{
			case ComponentType::Byte:
				return GL_BYTE;
			case ComponentType::UByte:
				return GL_UNSIGNED_BYTE;
			case ComponentType::Shot:
				return GL_UNSIGNED_SHORT;
			case ComponentType::Int:
				return GL_UNSIGNED_INT;
			case ComponentType::HalfFloat:
				return GL_HALF_FLOAT;
			case ComponentType::Float:
				return GL_FLOAT;
			case ComponentType::Double:
				return GL_DOUBLE;
			case ComponentType::Fixed:
				return GL_FIXED;
		}
		return 0;
	}
	
	GLenum GetIndexType(Render::HardwareIndexBuffer::IndexType i_type)
	{
		using namespace Render;
		switch (i_type)
		{
			case HardwareIndexBuffer::IndexType::Byte:
				return GL_UNSIGNED_BYTE;
			case HardwareIndexBuffer::IndexType::Short:
				return GL_UNSIGNED_SHORT;
			case HardwareIndexBuffer::IndexType::Int:
				return GL_UNSIGNED_INT;
		}

		return GL_UNSIGNED_SHORT;
	}

	GLenum GetMatrixName(MatrixMode i_mode)
	{
		switch (i_mode)
		{
			case MatrixMode::ModelView:
				return GL_MODELVIEW_MATRIX;
			case MatrixMode::Projection:
				return GL_PROJECTION_MATRIX;
			default:
				return GL_PROJECTION_MATRIX;
		}
	}

	GLenum GetMatrixMode(MatrixMode i_mode)
	{
		switch (i_mode)
		{
			case MatrixMode::ModelView:
				return GL_MODELVIEW;
			case MatrixMode::Projection:
				return GL_PROJECTION;
			default:
				return GL_PROJECTION;
		}
	}

	/////////////////////////////////////////////////////////////////////////////

	void OpenGLRenderer::Draw(Render::Batch i_batch)
	{
		auto ver_buf = m_hardware_buffer_mgr.AccessVertexBuffer(i_batch.vertices);
		auto ind_buf = m_hardware_buffer_mgr.AccessIndexBuffer(i_batch.indices);
		auto element = m_hardware_buffer_mgr.AccessElement(i_batch.element);

		if (ver_buf.m_hardware_id == 0 || ind_buf.m_hardware_id == 0 || element.m_vertex_size == 0)
		{
			assert(false && "Invalid parameters. Have buffers been created?");
			return;
		}

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, ver_buf.m_hardware_id);
		glVertexAttribPointer(0, // index for shader attribute
			element.m_vertex_size, // size
			GetComponentType(element.m_component), // type
			element.m_normalized ? GL_TRUE : GL_FALSE, // normalized
			element.m_stride, // stride
			0); // pointer
		CHECK_GL_ERRORS;


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind_buf.m_hardware_id);
		glDrawElements(GetPrimitiveType(element.m_primitive), ind_buf.m_num_indices, GetIndexType(ind_buf.m_index_type), 0);
		CHECK_GL_ERRORS;

		glDisableVertexAttribArray(0);
	}

	void OpenGLRenderer::SetMatrix(MatrixMode i_matrix_mode, const Matrix4f& i_matrix)
	{
		m_matrices[(int)i_matrix_mode] = i_matrix;
		glMatrixMode(GetMatrixMode(i_matrix_mode));
		GLfloat gl_matrix[16];
		makeGlMatrix(gl_matrix, i_matrix);
		glLoadMatrixf(gl_matrix);
	}

	void OpenGLRenderer::SetProjectionType(Render::ProjectionType i_projection_type)
	{		
		if (i_projection_type == Render::ProjectionType::Orthographic)
			gluOrtho2D(0, m_paint_rectangle.Width(), 0, m_paint_rectangle.Height());			
		else
			glViewport(0, 0, m_paint_rectangle.Width(), m_paint_rectangle.Height());
		// TODO: All matrices is changed or only ModelView and Projection?
		// Rescan all matrices
		constexpr int matrix_count = sizeof(m_matrices) / sizeof(Matrix4f);
		for (int i = 0; i < matrix_count; ++i)
		{
			glGetFloatv(GetMatrixName(static_cast<MatrixMode>(i)), m_matrices[i][0]);
			m_matrices[i].Transpose();
		}		
	}

	void OpenGLRenderer::PushMatrix()
	{
		glPushMatrix();
		CHECK_GL_ERRORS;
		m_pushed_matrices.push_back(GetMatrixName(m_current_mode));
	}

	void OpenGLRenderer::PopMatrix()
	{
		glPopMatrix();
		CHECK_GL_ERRORS;
		glGetFloatv(m_pushed_matrices.back(), m_matrices[(int)m_current_mode][0]);		
		m_matrices[(int)m_current_mode].Transpose();
		m_pushed_matrices.pop_back();
	}

	void OpenGLRenderer::SetMatrixMode(MatrixMode i_matrix_mode)
	{
		if (m_current_mode == i_matrix_mode)
			return;
		m_current_mode = i_matrix_mode;
		if (m_current_mode == MatrixMode::ModelView)
			glMatrixMode(GL_MODELVIEW);
		else
			glMatrixMode(GL_PROJECTION);
	}

	void OpenGLRenderer::SetCurrentMatrix(const Matrix4f& i_translation_matrix)
	{
		GLfloat gl_matrix[16];
		makeGlMatrix(gl_matrix, i_translation_matrix);
		glLoadMatrixf(gl_matrix);
	}

	void OpenGLRenderer::ModifyCurrentMatrix(const Matrix4f& i_multiplier)
	{		
		m_matrices[(int)m_current_mode] *= i_multiplier;		
		GLfloat gl_matrix[16];
		makeGlMatrix(gl_matrix, m_matrices[(int)m_current_mode]);
		glLoadMatrixf(gl_matrix);
	}

	/////////////////////////////////////////////////////////////////////////////

	void OpenGLRenderer::RenderLine(const Vector3& i_first_point, const Vector3& i_second_point, Color i_color, float i_width /* = 1.0 */)
	{
		GLColor color = i_color.Convert<GLColor>();
		glColor4f(color[0], color[1], color[2], color[3]);
		glLineWidth(i_width);
		glBegin(GL_LINES);
		glVertex2d(i_first_point[0], i_first_point[1]);
		glVertex2d(i_second_point[0], i_second_point[1]);
		glEnd();
	}

	void OpenGLRenderer::RenderRectangle(const Vector3& i_center, double i_width, double i_height, Color i_color)
	{
		GLColor color = i_color.Convert<GLColor>();
		glColor4f(color[0], color[1], color[2], color[3]);

		double x_offset = i_width / 2;
		double y_offset = i_height / 2;

		glBegin(GL_QUADS);
		glVertex3d(i_center[0] - x_offset, i_center[1] - y_offset, 0);
		glVertex3d(i_center[0] + x_offset, i_center[1] - y_offset, 0);
		glVertex3d(i_center[0] + x_offset, i_center[1] + y_offset, 0);
		glVertex3d(i_center[0] - x_offset, i_center[1] + y_offset, 0);
		glEnd();
	}

	void OpenGLRenderer::RenderCircle(const Vector3& i_center, double i_radius, Color i_color)
	{
		//Draw Circle
		GLColor color = i_color.Convert<GLColor>();
		glColor4f(color[0], color[1], color[2], color[3]);
		glBegin(GL_POLYGON);
		//Change the 6 to 12 to increase the steps (number of drawn points) for a smoother circle
		//Note that anything above 24 will have little affect on the circles appearance
		//Play with the numbers till you find the result you are looking for
		//Value 1.5 - Draws Triangle
		//Value 2 - Draws Square
		//Value 3 - Draws Hexagon
		//Value 4 - Draws Octagon
		//Value 5 - Draws Decagon
		//Notice the correlation between the value and the number of sides
		//The number of sides is always twice the value given this range
		for (double i = 0; i < 2 * M_PI; i += M_PI / 4) //<-- Change this Value
			glVertex3d(i_center[0] + cos(i)*i_radius, i_center[1] + sin(i)*i_radius, 0.0);
		glEnd();
		//Draw Circle
	}

	void OpenGLRenderer::RenderText(const Vector3& i_position, const std::wstring& i_text, Color i_color)
	{
		GLColor color = i_color.Convert<GLColor>();
		glColor4f(color[0], color[1], color[2], color[3]);
		throw std::runtime_error("RenderText is not implemented");
		//freetype::print(m_font_data, i_position.GetX(), m_paint_rectangle.Height() - i_position.GetY(), "%s", i_text);
	}

	void OpenGLRenderer::RenderText(const Vector3& i_position, const std::string& i_text, Color i_color)
	{
		GLColor color = i_color.Convert<GLColor>();
		glColor4f(color[0], color[1], color[2], color[3]);
		throw std::runtime_error("RenderText is not implemented");
		//freetype::print(m_font_data, i_position.GetX(), m_paint_rectangle.Height() - i_position.GetY(), "%s", i_text.c_str());
	}

	void OpenGLRenderer::BeginFrame()
	{
		// reset matrices
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// reset projection type
		SetProjectionType(Render::ProjectionType::Perspective);					
		// set to ModelView matrix
		SetMatrixMode(MatrixMode::ModelView);
		// clear screen
		_Clear(m_clear_color);		
	}

	void OpenGLRenderer::EndFrame()
	{		
		glFinish();
		SwapBuffers(wglGetCurrentDC());
	}
}