#include "stdafx.h"

#include "UIButton.h"

#include "Core.h"
#include "Applications/ApplicationBase.h"

#include <Patterns/MessageDispatcher/MessageDispatcher.h>

// TODO: separate include for bufferes - mechanism of buffers...
#include "Render/RenderWorld.h"
#include "Render/Commands.h"


#include "Render/IRenderer.h"
#include "Render/HardwareBufferManagerBase.h"

#include <GL/glew.h>
#include "Render/ShaderSystem.h"
#include "Render/OpenGL/GlUitlities.h"
#include "Render/OpenGL/HardwareBufferManager.h"
#include "Resources/ResourceManager.h"
#include "Render/TextureManager.h"

namespace SDK
{
	namespace UI
	{
		UIButton::UIButton()
		{}

		UIButton::~UIButton()
		{}

		void UIButton::UpdateImpl(float i_elapsed_time)
		{

		}

		void UIButton::DrawImpl(Render::RenderCommandBucket& i_bucket)
		{
			using namespace Render;
			size_t current_texture = 0;
			switch (m_state)
			{
				case State::Idle:
					current_texture = 0;
					break;
				case State::Hovered:
					current_texture = 1;
					break;
				case State::Pressed:
					current_texture = 2;
					break;
			}

			Commands::Transform* p_transform_cmd = i_bucket.Create<Commands::Transform>();
			p_transform_cmd->Translate(Vector3{ static_cast<float>(m_global_position[0]), static_cast<float>(m_global_position[1]), 0.f });

			Commands::SetupShader<2, 1>* p_shader_cmd = i_bucket.Append<Commands::SetupShader<2, 1>>(p_transform_cmd);
			p_shader_cmd->m_shader = Core::GetGlobalObject<Resources::ResourceManager>()->GetHandleToResource<Render::Shader>("TextureShader");
			p_shader_cmd->m_layouts[0] = m_ver_layout;
			p_shader_cmd->m_layouts[1] = m_uv_layout;

			static const int target = 0;
			auto p_shader = Core::GetGlobalObject<Render::ShaderSystem>()->Access("TextureShader");
			p_shader_cmd->SetValue(p_shader->GetUniform("text").location, Render::ShaderVariableType::Sampler2D, target);
			void* p_parent = p_shader_cmd;
			if (m_tex_handles[current_texture] != Render::TextureHandle::InvalidHandle())
			{
				Commands::BindTexture* p_bind_tex = i_bucket.Append<Commands::BindTexture>(p_shader_cmd);
				p_bind_tex->target = target;
				p_bind_tex->texture_handle = InternalHandle{ m_tex_handles[current_texture].index, m_tex_handles[current_texture].generation };
				p_parent = p_bind_tex;
			}

			Commands::Draw* p_draw = i_bucket.Append<Commands::Draw>(p_parent);
			p_draw->indices = m_indices;
		}

		void UIButton::LoadImpl(const PropertyElement& i_element)
		{
			float xpos = static_cast<float>(-m_global_size[0] / 2);
			float ypos = static_cast<float>(-m_global_size[1] / 2);
			float w = static_cast<float>(m_global_size[0]);
			float h = static_cast<float>(m_global_size[1]);
			const float vertices[] = {
				xpos,     ypos,
				xpos,     ypos + h,
				xpos + w, ypos + h,
				xpos + w, ypos,
			};

			ubyte inds[] = {
				0, 1, 2,
				3, 0, 2
			};
			// TODO: can optimize allocations?
			auto p_mgr = Core::GetRenderer()->GetHardwareBufferMgr();
			m_vertices = p_mgr->CreateHardwareBuffer(sizeof(vertices), Render::BufferUsageFormat::Static, vertices);
			m_indices = p_mgr->CreateIndexBuffer(Render::HardwareIndexBuffer::IndexType::Byte, sizeof(inds) / sizeof(ubyte), Render::BufferUsageFormat::Static, Render::PrimitiveType::Triangles, inds);
			m_ver_layout = p_mgr->CreateLayout(m_vertices, 2, Render::VertexSemantic::Position, Render::ComponentType::Float, false, 0, 0);

			static const float uvs[] = {
				0.f, 0.f,
				0.f, 1.f,
				1.f, 1.f,
				1.f, 0.f
			};
			m_uvs = Core::GetRenderer()->GetHardwareBufferMgr()->CreateHardwareBuffer(sizeof(uvs), Render::BufferUsageFormat::Static, uvs);
			m_uv_layout = Core::GetRenderer()->GetHardwareBufferMgr()->CreateLayout(m_uvs, 2, Render::VertexSemantic::TextureCoordinates, Render::ComponentType::Float, false, 0, 0);

			static const float hd_buf[] = {
				xpos,     ypos,			0.f, 0.f,
				xpos,     ypos + h,		0.f, 1.f,
				xpos + w, ypos + h,		1.f, 1.f,
				xpos + w, ypos,			1.f, 0.f
			};
			m_hd_buffer = p_mgr->CreateHardwareBuffer(sizeof(hd_buf), Render::BufferUsageFormat::Static, hd_buf);
			// some problem with strides
			//m_ver_layout = p_mgr->CreateLayout(m_hd_buffer, 2, Render::VertexSemantic::Position, Render::ComponentType::Float, false, sizeof(float)*4, 0);
			//m_uv_layout = p_mgr->CreateLayout(m_hd_buffer, 2, Render::VertexSemantic::TextureCoordinates, Render::ComponentType::Float, false, sizeof(float)*4, sizeof(float)*2);

			const std::string idle_tex = i_element.GetValue<std::string>("texture_idle");
			const std::string hovered_tex = i_element.GetValue<std::string>("texture_hovered");
			const std::string pressed_tex = i_element.GetValue<std::string>("texture_pressed");
			m_tex_handles[0] = Core::GetGlobalObject<Resources::ResourceManager>()->GetHandleToResource<Render::Texture>(idle_tex);
			m_tex_handles[1] = Core::GetGlobalObject<Resources::ResourceManager>()->GetHandleToResource<Render::Texture>(hovered_tex);
			m_tex_handles[2] = Core::GetGlobalObject<Resources::ResourceManager>()->GetHandleToResource<Render::Texture>(pressed_tex);
		}
		
		void UIButton::MouseMoved(MessageDispatcher& o_ui_dispatcher, const MouseEvent& i_evt)
		{
			//TODO: correct mechanism of hovering
		}

		void UIButton::MousePressed(MessageDispatcher& o_ui_dispatcher, const MouseEvent& i_evt)
		{
			if (IsHited({ i_evt.m_x, i_evt.m_y }))
				m_state = State::Pressed;
			o_ui_dispatcher.HandleMessage(UIButtonEvent{ UIButtonEvent::Type::Pressed }, GetName());
		}

		void UIButton::MouseReleased(MessageDispatcher& o_ui_dispatcher, const MouseEvent& i_evt)
		{
			if (IsHited({ i_evt.m_x, i_evt.m_y }))
				m_state = State::Idle;
			o_ui_dispatcher.HandleMessage(UIButtonEvent{ UIButtonEvent::Type::Released }, GetName());
		}

	} // UI
} // SDK