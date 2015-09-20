#include "stdafx.h"

#include "OpenGLRenderer.h"

#include <Math/VectorConstructor.h>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

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

//////////////////////////////////////////////////////////////////////////

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
		gluOrtho2D(0, m_paint_rectangle.Width(), m_paint_rectangle.Height(), 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void OpenGLRenderer::RenderLine(const Vector3D& i_first_point, const Vector3D& i_second_point, Color i_color, float i_width /* = 1.0 */)
	{
		glLoadIdentity();
		GLColor color = i_color.Convert<GLColor>();
		glColor4f(color[0], color[1], color[2], color[3]);
		glLineWidth(i_width);
		glBegin(GL_LINES);
		glVertex2d(i_first_point[0], i_first_point[1]);
		glVertex2d(i_second_point[0], i_second_point[1]);
		glEnd();
	}

	void OpenGLRenderer::RenderRectangle(const Vector3D& i_center, double i_width, double i_height, Color i_color)
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

	void OpenGLRenderer::RenderCircle(const Vector3D& i_center, double i_radius, Color i_color)
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

	void OpenGLRenderer::RenderText(const Vector3D& i_position, const std::wstring& i_text, Color i_color)
	{
		GLColor color = i_color.Convert<GLColor>();
		glColor4f(color[0], color[1], color[2], color[3]);
		throw std::runtime_error("RenderText is not implemented");
		//freetype::print(m_font_data, i_position.GetX(), m_paint_rectangle.Height() - i_position.GetY(), "%s", i_text);
	}

	void OpenGLRenderer::RenderText(const Vector3D& i_position, const std::string& i_text, Color i_color)
	{
		GLColor color = i_color.Convert<GLColor>();
		glColor4f(color[0], color[1], color[2], color[3]);
		throw std::runtime_error("RenderText is not implemented");
		//freetype::print(m_font_data, i_position.GetX(), m_paint_rectangle.Height() - i_position.GetY(), "%s", i_text.c_str());
	}

	void OpenGLRenderer::BeginFrame()
	{
		_Clear(m_clear_color);
	}

	void OpenGLRenderer::EndFrame()
	{
		glFinish();
		SwapBuffers(wglGetCurrentDC());
	}
}