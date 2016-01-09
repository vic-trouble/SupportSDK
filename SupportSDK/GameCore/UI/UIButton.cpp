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

		void UIButton::DrawImpl()
		{
			using namespace Render;
			Commands::Transform* p_transform_cmd = Render::gBuffer.Create<Commands::Transform>();
			p_transform_cmd->Translate(Vector3{ static_cast<float>(m_global_position[0]), static_cast<float>(m_global_position[1]), 0.f });

			Commands::Draw* p_cmd = Render::gBuffer.Append<Commands::Draw>(p_transform_cmd);
			p_cmd->vertices = m_batch.vertices;
			p_cmd->layout = m_batch.element;
			p_cmd->indices = m_batch.indices;
		}

		void UIButton::LoadImpl(const PropertyElement& i_element)
		{
			constexpr static float i_center[] = { 0, 0 };
			const uint half_size[] = { m_global_size[0] / 2, m_global_size[1] / 2 };
			const float verts[] = {
				i_center[0] - half_size[0], i_center[1] - half_size[1],
				i_center[0] + half_size[0], i_center[1] - half_size[1],
				i_center[0] + half_size[0], i_center[1] + half_size[1],
				i_center[0] - half_size[0], i_center[1] + half_size[1]
			};

			ubyte inds[] = {
				0, 1, 2,
				3, 0, 2
			};
			// TODO: can optimize allocations?
			auto p_mgr = Core::GetRenderer()->GetHardwareBufferMgr();
			m_batch.vertices = p_mgr->CreateVertexBuffer(sizeof(verts) / sizeof(float), sizeof(float) * 2, Render::BufferUsageFormat::Static, verts);
			m_batch.indices = p_mgr->CreateIndexBuffer(Render::HardwareIndexBuffer::IndexType::Byte, sizeof(inds) / sizeof(ubyte), Render::BufferUsageFormat::Static, inds);
			m_batch.element = p_mgr->CreateElement(2, Render::VertexSemantic::Position, Render::PrimitiveType::Triangles, Render::ComponentType::Float, false);
		}
		
		void UIButton::MouseMoved(MessageDispatcher& o_ui_dispatcher, const MouseEvent& i_evt)
		{
		}

		void UIButton::MousePressed(MessageDispatcher& o_ui_dispatcher, const MouseEvent& i_evt)
		{
			o_ui_dispatcher.HandleMessage(UIButtonEvent{ UIButtonEvent::Type::Pressed }, GetName());
		}

		void UIButton::MouseReleased(MessageDispatcher& o_ui_dispatcher, const MouseEvent& i_evt)
		{
			o_ui_dispatcher.HandleMessage(UIButtonEvent{ UIButtonEvent::Type::Released }, GetName());
		}

	} // UI
} // SDK