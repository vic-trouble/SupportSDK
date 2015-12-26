#include "stdafx.h"

#include "UIButton.h"

#include "Core.h"
#include "Applications/ApplicationBase.h"
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
		Render::Batch batch;
		void CreateDummy()
		{
			static bool once = true;
			auto p_renderer = Core::GetRenderer();
			if (once)
			{
				constexpr static float i_center[] = { 200, 500 };
				constexpr static float x_offset = 100.f;
				constexpr static float y_offset = 50.f;
				const float verts[] = {
					i_center[0] - x_offset, i_center[1] - y_offset,
					i_center[0] + x_offset, i_center[1] - y_offset,
					i_center[0] + x_offset, i_center[1] + y_offset,
					i_center[0] - x_offset, i_center[1] + y_offset
				};

				uint inds[] = {
					0, 1, 2,
					3, 0, 2
				};

				auto p_mgr = p_renderer->GetHardwareBufferMgr();
				batch.vertices = p_mgr->CreateVertexBuffer(sizeof(verts) / sizeof(float), sizeof(float) * 2, Render::BufferUsageFormat::Static, verts);
				batch.indices = p_mgr->CreateIndexBuffer(Render::HardwareIndexBuffer::IndexType::Int, sizeof(inds) / sizeof(uint), Render::BufferUsageFormat::Static, inds);
				batch.element = p_mgr->CreateElement(2, Render::VertexSemantic::Position, Render::PrimitiveType::Triangles, Render::ComponentType::Float, false);

				once = false;
			}
		}

		void UIButton::DrawImpl()
		{
			using namespace Render;
			CreateDummy();
			Commands::Transform* p_transform_cmd = Render::gBuffer.Create<Commands::Transform>();
			p_transform_cmd->Translate(Vector3{50, 50, 0});

			Commands::Draw* p_cmd = Render::gBuffer.Append<Commands::Draw>(p_transform_cmd);
			p_cmd->vertices = batch.vertices;
			p_cmd->layout = batch.element;
			p_cmd->indices = batch.indices;
		}

		void UIButton::Load(const PropertyElement& i_element)
		{

		}

	} // UI
} // SDK