#include "stdafx.h"

#include "CoreDelegateImpl.h"

#include <GameCore/Core.h>
#include <GameCore/Render/IRenderer.h>
#include <GameCore/Render/HardwareBufferManagerBase.h>
#include <GameCore/Input/inputsystem.h>
#include <GameCore/Applications/ApplicationBase.h>

// for test
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <time.h>

#include <GameCore/Systems/MeshSystem.h>
#include <GameCore/UI/UIControlSystem.h>
using namespace SDK;


// An array of 3 vectors which represents 3 vertices
static const GLfloat g_vertex_buffer_data[][3] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 20.0f, 0.0f, 0.0f },
	{ 20.0f, 20.0f, 0.0f },
	{ 0.0f, 20.0f, 0.0f },
};
#include <GameCore/Render/ScopedMatrixTransformation.h>
namespace Game
{

	void CoreDelegateImpl::CreateMesh()
	{
		uint cubeIndices[] = {
			0, 1, 2,
			3, 0, 2
		};

		vecs[0][0] = 10;
		vecs[0][1] = 15;

		vecs[1][0] = 10;
		vecs[1][1] = 15;

		auto p_renderer = Core::GetRenderer();
		auto p_mgr = p_renderer->GetHardwareBufferMgr();
		// 1st
		batch[0].vertices = p_mgr->CreateVertexBuffer(4, sizeof(float) * 3, Render::BufferUsageFormat::Static, g_vertex_buffer_data);
		batch[0].indices = p_mgr->CreateIndexBuffer(Render::HardwareIndexBuffer::IndexType::Int, 6, Render::BufferUsageFormat::Static, cubeIndices);
		batch[0].element = p_mgr->CreateElement(3, Render::VertexSemantic::Position, Render::PrimitiveType::Triangles, Render::ComponentType::Float, false);

		{
			constexpr static float i_center[] = { 0, 0 };
			const uint half_size[] = { 100 / 2, 50 / 2 };
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
			batch[1].vertices = p_mgr->CreateVertexBuffer(sizeof(verts) / sizeof(float), sizeof(float) * 2, Render::BufferUsageFormat::Static, verts);
			batch[1].indices = p_mgr->CreateIndexBuffer(Render::HardwareIndexBuffer::IndexType::Byte, sizeof(inds) / sizeof(ubyte), Render::BufferUsageFormat::Static, inds);
			batch[1].element = p_mgr->CreateElement(2, Render::VertexSemantic::Position, Render::PrimitiveType::Triangles, Render::ComponentType::Float, false);			
		}		
	}

	void CoreDelegateImpl::InitQuaternions()
	{
		m_rotation_src.FromAngleAxis(0.f, Math::VectorConstructor<float>::Construct(0, 0, 1));
		m_rotation_dest.FromAngleAxis(3.14f, Math::VectorConstructor<float>::Construct(0, 0, 1));
		m_rotation_src.Normalize();
		m_rotation_dest.Normalize();
		m_time = 0.f;

		m_point[0] = 50;// rand() % Core::GetRenderer()->GetTargetRectangle().Width();
		m_point[1] = 50;// rand() % Core::GetRenderer()->GetTargetRectangle().Height();
		m_point[2] = 0.f;
		m_time_to_change = 0.f;

		CreateQuatPoints();
	}

	void CoreDelegateImpl::CreateQuatPoints()
	{
		first_point = m_point;
		points.reserve(50);
		for (size_t i = 0; i < 50; ++i)
		{
			points.emplace_back(Vector3(m_point));
		}

		float t = 0.f;
		const float step = 1.f / points.size();
		m_rotation_curr = m_rotation_src;
		for (Vector3& p : points)
		{
			m_rotation_curr = QuaternionF::Nlerp(t, m_rotation_curr, m_rotation_dest, true);
			p = m_rotation_curr.ApplyTo(p);
			t += step;
		}
		m_rotation_curr = m_rotation_src;
	}

	void CoreDelegateImpl::OnCreate()
	{
		auto& world = Core::GetApplication()->GetWorld();
		auto& camera = world.GetCamera();
		camera.m_modelview_matrix = Matrix4f::IDENTITY;
		camera.m_need_recalc = false;
		auto& frustum = world.GetFrustum();
		
		frustum.SetProjectionType(Render::ProjectionType::Orthographic);
		//frustum.SetFrustumExtents(-0.5f, 0.5f, 0.5f, 0.5f)
		InputSystem::Instance().AddSubscriber(&m_input_subs);
		InputSystem::Instance().AddSubscriber(&m_ship);

		InitQuaternions();

		offset = Math::VectorConstructor<float>::Construct(5.f, 1.f, 0.f);

		CreateMesh();
	}

	void CoreDelegateImpl::OnTerminate()
	{
		InputSystem::Instance().RemoveSubscriber(&m_input_subs);
		InputSystem::Instance().RemoveSubscriber(&m_ship);

		auto p_renderer = Core::GetRenderer();
		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[0].vertices);
		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[0].indices);
		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[0].vertices);

		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[1].vertices);
		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[1].indices);
		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[1].vertices);
	}


	void CoreDelegateImpl::Update(float i_elapsed_time)
	{
		m_ship.Update();

		m_time_to_change += 0.01f;

		static float ch = 0.05f;
		if (m_time_to_change < ch)
			return;
		else
			m_time_to_change = 0.f;

		if (m_time > 1.f)
		{
			auto src_rot = m_rotation_src;
			m_rotation_src = m_rotation_dest;
			m_rotation_dest = src_rot;
			m_rotation_curr = m_rotation_src;
			m_time = 0.f;
		}
		const float step = 1.f / points.size();
		m_rotation_curr = QuaternionF::Nlerp(m_time, m_rotation_curr, m_rotation_dest, false);
		m_point = m_rotation_curr.ApplyTo(first_point);
		m_time += step;
	}

	void CoreDelegateImpl::Draw()
	{
		auto p_renderer = Core::GetRenderer();
		//p_renderer->SetMatrix(MatrixMode::Projection, Matrix4f::IDENTITY);
		//p_renderer->SetMatrix(MatrixMode::ModelView, Matrix4f::IDENTITY);
		p_renderer->SetProjectionType(Render::ProjectionType::Orthographic);
		Render::ScopedMatrixTransformation scopedMatrix(p_renderer);
		m_ship.Draw();

		{
			Render::ScopedMatrixTransformation scopedMatrix(p_renderer);
			p_renderer->ModifyCurrentMatrix(Matrix4f::MakeTranslation(Vector3{ 150, 200, 0 }));
			static Vector3 vec = Math::VectorConstructor<float>::Construct(10, 30, 0);
			p_renderer->RenderLine(offset + first_point, offset + first_point + m_point, Color(255, 255, 255, 255));
			size_t i = 1;

			for (Vector3& p : points)
			{
				p_renderer->RenderCircle(offset + first_point + p, 0.3*i, Color(255 % i, 0, 255, 255));
				++i;
			}
		}
		//////////////////////////////////////
		{
			Render::ScopedMatrixTransformation scopedMatrix(p_renderer);
			p_renderer->ModifyCurrentMatrix(Matrix4f::MakeTranslation(Vector3{ 150, 150, 0 }));
			glColor4f(0, 0.5f, 0., 1.f);

			p_renderer->Draw(batch[0]);

			glColor4f(0, 0.5f, 0.5, 1.f);
			p_renderer->ModifyCurrentMatrix(Matrix4f::MakeTranslation(Vector3{ 200, 250, 0 }));
			p_renderer->Draw(batch[1]);
		}
		{
			/*glPushMatrix();
			glTranslatef(100, 500, 0);

			auto beg = clock();

			for (size_t i = 0; i < 100000; ++i)
			{
				p_renderer->Draw(batch[0]);
				p_renderer->Draw(batch[1]);
			}

			volatile auto res = clock() - beg;

			beg = clock();

			for (size_t i = 0; i < 100000; ++i)
			{
				p_renderer->RenderRectangle(vecs[0], 10, 10, Color(255, 255, 255, 255));
				p_renderer->RenderRectangle(vecs[1], 10, 10, Color(255, 255, 255, 255));
			}

			volatile auto res2 = clock() - beg;

			std::cout << res << res2;

			glPopMatrix();*/
		}

	}

} // Game