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

	void initLights()
	{
		// set up light colors (ambient, diffuse, specular)
		GLfloat lightKa[] = { .0f, .0f, .0f, 1.0f };      // ambient light
		GLfloat lightKd[] = { .9f, .9f, .9f, 1.0f };      // diffuse light
		GLfloat lightKs[] = { 1, 1, 1, 1 };               // specular light
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

		// position the light in eye space
		float lightPos[4] = { 0, 0, 1, 0 };               // directional light
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		glEnable(GL_LIGHT0);                            // MUST enable each light source after configuration
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

		initLights();
	}

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
		m_paint_rectangle.SetCenter(SDK::Math::VectorConstructor<int>::Construct(rc.left + width / 2, rc.bottom + height / 2));
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

	/////////////////////////////////////////////////////////////////////////////

	void OpenGLRenderer::Draw(Render::Batch i_batch)
	{
		auto ver_buf = m_hardware_buffer_mgr.AccessVertexBuffer(i_batch.vertices);
		auto ind_buf = m_hardware_buffer_mgr.AccessIndexBuffer(i_batch.indices);
		auto element = m_hardware_buffer_mgr.AccessElement(i_batch.element);

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

	void OpenGLRenderer::SetProjectionType(Render::ProjectionType i_projection_type)
	{
		if (i_projection_type == Render::ProjectionType::Orthographic)
			gluOrtho2D(0, m_paint_rectangle.Width(), 0, m_paint_rectangle.Height());			
		else
			glViewport(0, 0, m_paint_rectangle.Width(), m_paint_rectangle.Height());

	}

	void OpenGLRenderer::SetProjectionMatrix(Matrix4f&& i_projection_matrix)
	{		
		m_matrices[(int)MatrixMode::Projection] = i_projection_matrix;
		glMatrixMode(GL_PROJECTION);
		GLfloat gl_matrix[16];
		makeGlMatrix(gl_matrix, i_projection_matrix);
		glLoadMatrixf(gl_matrix);
	}

	void OpenGLRenderer::SetModelViewMatrix(Matrix4f&& i_modelview_matrix)
	{
		m_matrices[(int)MatrixMode::ModelView] = i_modelview_matrix;
		glMatrixMode(GL_MODELVIEW);
		GLfloat gl_matrix[16];
		makeGlMatrix(gl_matrix, i_modelview_matrix);
		glLoadMatrixf(gl_matrix);
	}

	void OpenGLRenderer::PushMatrix()
	{
		glPushMatrix();
	}

	void OpenGLRenderer::PopMatrix()
	{
		glPopMatrix();
	}

	void OpenGLRenderer::SetCurrentMatrix(const Matrix4f& i_translation_matrix)
	{
		// TODO: as understood - use shader and not modify current matrix
		auto scale = i_translation_matrix.GetScaleVector();
		auto translate = i_translation_matrix.GetTranslationVector();
		glTranslatef(translate[0], translate[1], translate[2]);
		if (i_translation_matrix.IsIdentity())
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glLoadIdentity();
		}

		/*
		float[16] transMatrix;
		glGetFloatv(GL_MODELVIEW_MATRIX, transMatrix);
		*/
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
		SetProjectionType(Render::ProjectionType::Perspective);
		/*glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();*/
		_Clear(m_clear_color);		
	}

	void OpenGLRenderer::EndFrame()
	{		
		glFinish();
		SwapBuffers(wglGetCurrentDC());
	}
}