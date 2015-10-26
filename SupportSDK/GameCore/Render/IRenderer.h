#ifndef __GAMECORE_IRENDERER_H__
#define __GAMECORE_IRENDERER_H__

#include "RenderTypes.h"

#include "Render/VertexDeclaration.h"

namespace SDK
{

	namespace Render
	{
		class HardwareBufferManagerBase;
	}

	struct Color
	{
		unsigned int m_color;//in RGBA

		Color()
			: m_color(0)
		{    }

		Color::Color(int i_color)
			: m_color(i_color)
		{  }

		Color(unsigned char i_r, unsigned char i_g, unsigned char i_b, unsigned char i_a)
			: m_color((i_r << 24) | (i_g << 16) | (i_b << 8) | i_a)
		{    }

		template <typename TColorFrom>
		void Convert(TColorFrom i_color_from);

		template <typename TColorTo>
		TColorTo Convert();

		unsigned int operator >> (size_t i_shift)
		{
			return m_color >> i_shift;
		}
	};

	class IRenderer
	{
	protected:
		Color m_clear_color;

		// pointer to render context
		SDK::uint64	m_render_context_id;

	public:
		IRenderer() : m_render_context_id(0){}
		virtual ~IRenderer(){}

		SDK::uint64 GetRenderContextID() const { return m_render_context_id; }

		virtual void Initialize() = 0;
		virtual void Release() = 0;
		virtual void Reshape() = 0;

		virtual IRect GetTargetRectangle() = 0;
		virtual void	BeginFrame() = 0;
		virtual void	EndFrame() = 0;

		virtual Render::HardwareBufferManagerBase* GetHardwareBufferMgr() = 0;

		virtual void Draw(Render::Batch i_decl) = 0;

		//virtual void DrawPrimitive(Render::VertexBufferHandle i_vertices, Render::IndexBufferHandle i_indices, Render::PrimitiveType i_primitive) = 0;

		/////////////////////////////////////////////////////////////////

		virtual void RenderLine(const Vector3& i_first_point, const Vector3& i_second_point, Color i_color, float i_width = 1.0) = 0;
		virtual void RenderRectangle(const Vector3& i_center, double i_width, double i_height, Color i_color) = 0;
		virtual void RenderCircle(const Vector3& i_center, double i_radius, Color i_color) = 0;
		virtual void RenderText(const Vector3& i_position, const std::wstring& i_text, Color i_color) = 0;
		virtual void RenderText(const Vector3& i_position, const std::string& i_text, Color i_color) = 0;

		void         SetClearColor(Color i_color)
		{
			m_clear_color = i_color;
		}
	};

} // SDK

#endif