#include "stdafx.h"

#include "CoreDelegateImpl.h"

#include <GameCore/Core.h>
#include <GameCore/Render/IRenderer.h>
#include <GameCore/Render/HardwareBufferManagerBase.h>
#include <GameCore/Input/inputsystem.h>
#include <GameCore/Render/OpenGL/GlUitlities.h>
#include <GameCore/Render/TextureManager.h>

// for test
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <time.h>

#include <Math/Matrix4.h>

using namespace SDK;

#pragma comment(lib, "opengl32.lib")
#if defined(_DEBUG)
# pragma comment(lib, "glew32d.lib")
#else
# pragma comment(lib, "glew32.lib")
#endif

#include <GameCore/Applications/ApplicationBase.h>

#include <GameCore/Render/MeshSystem.h>
#include <GameCore/Render/MaterialManager.h>
#include <GameCore/Systems/TransformationsSystem.h>

#include <GameCore/Render/ShaderSystem.h>
#include <GameCore/EntityManager.h>
#include <GameCore/Render/LightsController.h>
#include <GameCore/Render/ScopedLightSwitch.h>
#include <GameCore/PropertyElement.h>
#include <GameCore/Resources/ResourceManager.h>

namespace Game
{
	EntityHandle entity_handler;
	void CoreDelegateImpl::LoadModel()
	{
		//E:\Git_Projects\SupportSDK\Samples\Resources\Models\Box.obj
		//loaded_mesh = Render::g_mesh_system.Load("Resources\\Models\\Box.obj", Render::BufferUsageFormat::Static, Render::BufferUsageFormat::Static);
		auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
		auto handle = p_load_manager->GetHandleToResource("SimpleBox");
		loaded_mesh = { handle.index, handle.generation };
		
		auto mesh_handler = Render::g_mesh_system.CreateInstance(loaded_mesh);
		auto trans_handler = g_transforms_system.CreateInstance();
		auto p_transform = g_transforms_system.GetInstance(trans_handler);
		p_transform->m_position[0] = 1.5f;
		p_transform->m_position[1] = 1.5f;
		p_transform->m_position[2] = -5.5f;

		entity_handler = g_entity_manager.CreateEntity();
		g_entity_manager.AddComponent<Render::MeshComponent>(entity_handler, mesh_handler);
		g_entity_manager.AddComponent<Transform>(entity_handler, trans_handler);

		// test getting of entity and component
		auto entity = g_entity_manager.GetEntity(entity_handler);
		
		auto material_handle_int = p_load_manager->GetHandleToResource("SimpleBox");
		Render::MaterialHandle material_handle = { material_handle_int.index, material_handle_int.generation };
		auto p_material = Render::g_material_mgr.AccessMaterial(material_handle);

		Render::g_mesh_system.AddMaterialTo(mesh_handler, material_handle);

		/*auto p_mgr = Core::GetRenderer()->GetHardwareBufferMgr();

		const float verts[] = {
			1, 1, 1,
			-1, 1, 1,
			-1, -1, 1,
			1, -1, 1,
			1, -1, -1,
			1, 1, -1,
			-1, 1, -1,
			-1, -1, -1
		};

		const ubyte inds[] = {
			// front
			0, 1, 2,
			2, 3, 0,

			// right
			0, 3, 4,
			4, 5, 0,

			// top
			0, 5, 6,
			6, 1, 0,

			// left
			1, 6, 7,
			7, 2, 1,

			// bottom
			7, 4, 3,
			3, 2, 7,

			// back
			4, 7, 6,
			6, 5, 4
		};

		// 1st
		Render::Batch b;
		b.vertices = p_mgr->CreateVertexBuffer(sizeof(verts) / sizeof(float), sizeof(float), Render::BufferUsageFormat::Static, verts);
		b.indices = p_mgr->CreateIndexBuffer(Render::HardwareIndexBuffer::IndexType::Byte, sizeof(inds) / sizeof(ubyte), Render::BufferUsageFormat::Static, inds);
		b.element = p_mgr->CreateElement(3, Render::VertexSemantic::Position, Render::PrimitiveType::Triangles, Render::ComponentType::Float, false);

		loaded_mesh.Set(b.vertices, b.indices, b.element);*/
	}

	void CoreDelegateImpl::CreateMesh()
	{	
		auto p_renderer = Core::GetRenderer();
		auto p_mgr = p_renderer->GetHardwareBufferMgr();

		const float verts[] = {
			1, 1, 1,
			-1, 1, 1,
			-1, -1, 1,
			1, -1, 1,
			1, -1, -1,
			1, 1, -1,
			-1, 1, -1,
			-1, -1, -1
		};

		const uint inds[] = {
			// front
			0, 1, 2,
			2, 3, 0,
			
			// right
			0, 3, 4,
			4, 5, 0,

			// top
			0, 5, 6,
			6, 1, 0,

			// left
			1, 6, 7,
			7, 2, 1,

			// bottom
			7, 4, 3,
			3, 2, 7,

			// back
			4, 7, 6,
			6, 5, 4
		};

		// 1st
		batch[0].vertices = p_mgr->CreateHardwareBuffer(sizeof(verts) / sizeof(float), Render::BufferUsageFormat::Static, verts);
		batch[0].indices = p_mgr->CreateIndexBuffer(Render::HardwareIndexBuffer::IndexType::Int, sizeof(inds) / sizeof(uint), Render::BufferUsageFormat::Static, Render::PrimitiveType::Triangles, inds);
		batch[0].element = p_mgr->CreateLayout(batch[0].vertices, 3, Render::VertexSemantic::Position, Render::ComponentType::Float, false, 0, 0);
	}

	GLUquadricObj* quadricId;
	
	
	Render::Viewport g_vp;
	void CoreDelegateImpl::OnCreate()
	{
		InputSystem::Instance().AddSubscriber(&m_input_subs);
		auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
		p_load_manager->LoadResourceSet("..\\..\\Resources\\ResourceSets\\render_testing.res");

		LoadModel();

		CreateMesh();

		auto& world = Core::GetApplication()->GetWorld();
		auto& camera = world.GetCamera();

		camera.LookAt({ 0,0,-15 }, { -10,-10,-10 });

		world.GetFrustum().SetFOV(60 * Math::DEG2RAD);
		g_vp = { world.GetFrustum().GetProjectionType(),
			world.GetFrustum().GetProjectionMatrix(),
			world.GetCamera().GetModelViewMatrix() };
		// create a GLU quadric object
		quadricId = gluNewQuadric();
		gluQuadricDrawStyle(quadricId, GLU_FILL);		
	}

	void CoreDelegateImpl::OnTerminate()
	{
		InputSystem::Instance().RemoveSubscriber(&m_input_subs);

		auto p_renderer = Core::GetRenderer();
		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[0].vertices);
		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[0].indices);
		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[0].vertices);

		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[1].vertices);
		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[1].indices);
		p_renderer->GetHardwareBufferMgr()->DestroyBuffer(batch[1].vertices);

		g_entity_manager.RemoveEntity(entity_handler);

		auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
		p_load_manager->UnloadSet(p_load_manager->GetHandleToSet("render_testing"));
	}


	void CoreDelegateImpl::Update(float i_elapsed_time)
	{
	}
	

	namespace
	{
		const int SLICES = 36;
		const int STACKS = 24;
		const float RADIUS = 0.2f;
		void DrawSpheres()
		{
			float color0[3] = { 0.5, 0.5, 0.5 };
			float color1[3] = { 1, 0, 0 };
			float color2[3] = { 1, 0.5f, 0 };
			float color3[3] = { 1, 1, 0 };
			float color4[3] = { 0, 1, 0 };
			float color5[3] = { 0, 1, 1 };
			float color6[3] = { 0, 0, 1 };
			float color7[3] = { 1, 0, 1 };

			// set default specular and shiniess using glMaterial
			float shininess = 64.0f;
			float specularColor[] = { 1.0, 1.0f, 1.0f, 1.0f };
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

			glPushMatrix();
			glColor3fv(color0);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 0, 3);
			glColor3fv(color1);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1, 0, 2);
			glColor3fv(color2);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-1, 0, 2);
			glColor3fv(color2);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 1, 2);
			glColor3fv(color2);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, -1, 2);
			glColor3fv(color2);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(2, 0, 1);
			glColor3fv(color3);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-2, 0, 1);
			glColor3fv(color3);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 2, 1);
			glColor3fv(color3);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, -2, 1);
			glColor3fv(color3);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(3, 0, 0);
			glColor3fv(color4);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-3, 0, 0);
			glColor3fv(color4);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 3, 0);
			glColor3fv(color4);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, -3, 0);
			glColor3fv(color4);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(4, 0, -1);
			glColor3fv(color5);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-4, 0, -1);
			glColor3fv(color5);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 4, -1);
			glColor3fv(color5);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, -4, -1);
			glColor3fv(color5);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(5, 0, -2);
			glColor3fv(color6);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-5, 0, -2);
			glColor3fv(color6);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 5, -2);
			glColor3fv(color6);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, -5, -2);
			glColor3fv(color6);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(6, 0, -3);
			glColor3fv(color7);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-6, 0, -3);
			glColor3fv(color7);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 6, -3);
			glColor3fv(color7);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, -6, -3);
			glColor3fv(color7);
			gluSphere(quadricId, RADIUS, SLICES, STACKS);
			glPopMatrix();
		}

		void drawGrid(float size, float step)
		{
			auto p_render = SDK::Core::GetRenderer();
			SDK::Render::ScopedLightSwitch scopedLight(Core::GetRenderer()->GetLightsController());
			Core::GetRenderer()->GetLightsController()->DisableLighting();
			// disable lighting
			for (float i = step; i <= size; i += step)
			{
				p_render->RenderLine(SDK::Vector3{ -size, 0, i }, SDK::Vector3{ size, 0, i }, SDK::Color(76, 76, 76, 200));
				p_render->RenderLine(SDK::Vector3{ -size, 0, -i }, SDK::Vector3{ size, 0, -i }, SDK::Color(76, 76, 76, 200));

				p_render->RenderLine(SDK::Vector3{ i, 0, -size }, SDK::Vector3{ i, 0, size }, SDK::Color(76, 76, 76, 200));
				p_render->RenderLine(SDK::Vector3{ -i, 0, -size }, SDK::Vector3{ -i, 0, size }, SDK::Color(76, 76, 76, 200));
			}

			// x-axis
			p_render->RenderLine(SDK::Vector3{ -size, 0, 0 }, SDK::Vector3{ size, 0, 0 }, SDK::Color(127, 0, 0, 200));

			// z-axis
			p_render->RenderLine(SDK::Vector3{ 0, 0, -size }, SDK::Vector3{ 0, 0, size }, SDK::Color(0, 0, 127, 200));
		}

		void draw1()
		{
			glPushMatrix();
			glTranslatef(-4, -2, 0); // move to upper left corner
			glBegin(GL_TRIANGLES);
				
				// front faces
				glNormal3f(0, 0, 1);
				// face v0-v1-v2
				glColor3f(1, 1, 1);
				 glVertex3f(1, 1, 1);
				glColor3f(1, 1, 0);
				 glVertex3f(-1, 1, 1);
				glColor3f(1, 0, 0);
				 glVertex3f(-1, -1, 1);
				// face v2-v3-v0
				glColor3f(1, 0, 0);
				 glVertex3f(-1, -1, 1);
				glColor3f(1, 0, 1);
				 glVertex3f(1, -1, 1);
				glColor3f(1, 1, 1);
				 glVertex3f(1, 1, 1);

				// right faces
				glNormal3f(1, 0, 0);
				// face v0-v3-v4
				glColor3f(1, 1, 1);
				glVertex3f(1, 1, 1);
				glColor3f(1, 0, 1);
				glVertex3f(1, -1, 1);
				glColor3f(0, 0, 1);
				glVertex3f(1, -1, -1);
				// face v4-v5-v0
				glColor3f(0, 0, 1);
				glVertex3f(1, -1, -1);
				glColor3f(0, 1, 1);
				glVertex3f(1, 1, -1);
				glColor3f(1, 1, 1);
				glVertex3f(1, 1, 1);

				// top faces
				glNormal3f(0, 1, 0);
				// face v0-v5-v6
				glColor3f(1, 1, 1);
				glVertex3f(1, 1, 1);
				glColor3f(0, 1, 1);
				glVertex3f(1, 1, -1);
				glColor3f(0, 1, 0);
				glVertex3f(-1, 1, -1);
				// face v6-v1-v0
				glColor3f(0, 1, 0);
				glVertex3f(-1, 1, -1);
				glColor3f(1, 1, 0);
				glVertex3f(-1, 1, 1);
				glColor3f(1, 1, 1);
				glVertex3f(1, 1, 1);

				// left faces
				glNormal3f(-1, 0, 0);
				// face  v1-v6-v7
				glColor3f(1, 1, 0);
				glVertex3f(-1, 1, 1);
				glColor3f(0, 1, 0);
				glVertex3f(-1, 1, -1);
				glColor3f(0, 0, 0);
				glVertex3f(-1, -1, -1);
				// face v7-v2-v1
				glColor3f(0, 0, 0);
				glVertex3f(-1, -1, -1);
				glColor3f(1, 0, 0);
				glVertex3f(-1, -1, 1);
				glColor3f(1, 1, 0);
				glVertex3f(-1, 1, 1);

				// bottom faces
				glNormal3f(0, -1, 0);
				// face v7-v4-v3
				glColor3f(0, 0, 0);
				glVertex3f(-1, -1, -1);
				glColor3f(0, 0, 1);
				glVertex3f(1, -1, -1);
				glColor3f(1, 0, 1);
				glVertex3f(1, -1, 1);
				// face v3-v2-v7
				glColor3f(1, 0, 1);
				glVertex3f(1, -1, 1);
				glColor3f(1, 0, 0);
				glVertex3f(-1, -1, 1);
				glColor3f(0, 0, 0);
				glVertex3f(-1, -1, -1);

				// back faces
				glNormal3f(0, 0, -1);
				// face v4-v7-v6
				glColor3f(0, 0, 1);
				glVertex3f(1, -1, -1);
				glColor3f(0, 0, 0);
				glVertex3f(-1, -1, -1);
				glColor3f(0, 1, 0);
				glVertex3f(-1, 1, -1);
				// face v6-v5-v4
				glColor3f(0, 1, 0);
				glVertex3f(-1, 1, -1);
				glColor3f(0, 1, 1);
				glVertex3f(1, 1, -1);
				glColor3f(0, 0, 1);
				glVertex3f(1, -1, -1);
			glEnd();

			glPopMatrix();
		}


		namespace
		{

			GLfloat vertices2[] = { 1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1,   // v0,v1,v2,v3 (front)
				1, 1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1,   // v0,v3,v4,v5 (right)
				1, 1, 1, 1, 1, -1, -1, 1, -1, -1, 1, 1,   // v0,v5,v6,v1 (top)
				-1, 1, 1, -1, 1, -1, -1, -1, -1, -1, -1, 1,   // v1,v6,v7,v2 (left)
				-1, -1, -1, 1, -1, -1, 1, -1, 1, -1, -1, 1,   // v7,v4,v3,v2 (bottom)
				1, -1, -1, -1, -1, -1, -1, 1, -1, 1, 1, -1 }; // v4,v7,v6,v5 (back)

			// normal array
			GLfloat normals2[] = { 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,   // v0,v1,v2,v3 (front)
				1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,   // v0,v3,v4,v5 (right)
				0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,   // v0,v5,v6,v1 (top)
				-1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0,   // v1,v6,v7,v2 (left)
				0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0,   // v7,v4,v3,v2 (bottom)
				0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1 }; // v4,v7,v6,v5 (back)

			// color array
			GLfloat colors2[] = { 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1,   // v0,v1,v2,v3 (front)
				1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1,   // v0,v3,v4,v5 (right)
				1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0,   // v0,v5,v6,v1 (top)
				1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,   // v1,v6,v7,v2 (left)
				0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0,   // v7,v4,v3,v2 (bottom)
				0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1 }; // v4,v7,v6,v5 (back)

			// index array of vertex array for glDrawElements() & glDrawRangeElement()
			GLubyte indices[] = { 0, 1, 2, 2, 3, 0,      // front
				4, 5, 6, 6, 7, 4,      // right
				8, 9, 10, 10, 11, 8,      // top
				12, 13, 14, 14, 15, 12,      // left
				16, 17, 18, 18, 19, 16,      // bottom
				20, 21, 22, 22, 23, 20 };    // back

		} // coords for drawTest

		void drawTest()
		{
			glPushMatrix();
			glTranslatef(2, 2, 0);			

			// enable and specify pointers to vertex arrays
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			glNormalPointer(GL_FLOAT, 0, normals2);
			glColorPointer(3, GL_FLOAT, 0, colors2);
			glVertexPointer(3, GL_FLOAT, 0, vertices2);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

			glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);

			glPopMatrix();
		}

		void draw3(bool draw_test)
		{
			// clear buffer
			glClearColor(0.0f, 0.0f, 0.0f, 0.f);   // background color


			glPushMatrix();
				drawGrid(10, 1);
				DrawSpheres();
				if (draw_test)
					drawTest();
				draw1();
			glPopMatrix();
		}
	} // namespace

	Render::Batch batch;
	void Render2D()
	{
		static bool once = true;
		auto p_renderer = Core::GetRenderer();
		if (once)
		{
			constexpr static float i_center[] = { 200, 500 };
			constexpr static float x_offset = 100.f;
			constexpr static float y_offset = 50.f;
			const GLfloat verts[] = {
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
			batch.vertices = p_mgr->CreateHardwareBuffer(sizeof(verts) / sizeof(float), Render::BufferUsageFormat::Static, verts);
			batch.indices = p_mgr->CreateIndexBuffer(Render::HardwareIndexBuffer::IndexType::Int, sizeof(inds) / sizeof(uint), Render::BufferUsageFormat::Static, Render::PrimitiveType::Triangles, inds);
			batch.element = p_mgr->CreateLayout(batch.vertices, 3, Render::VertexSemantic::Position, Render::ComponentType::Float, false, 0, 0);
			once = false;
		}

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		p_renderer->SetProjectionType(Render::ProjectionType::Orthographic);
		
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_ONE, GL_SRC_ALPHA);
		glColor4f(1.f, 1.f, 1.f, 0.2f);		
		p_renderer->Draw(batch);
		
		glColor4f(1.f, 1.f, 1.f, .5f);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		//p_renderer->SetProjectionType(Render::ProjectionType::Perspective);
		CHECK_GL_ERRORS;
	}
	void CoreDelegateImpl::Draw()
	{
		auto p_renderer = Core::GetRenderer();
		p_renderer->SetMatrix(MatrixMode::Projection, g_vp.m_projection_matrix);
		p_renderer->SetMatrix(MatrixMode::ModelView, g_vp.m_modelview_matrix);
		draw3(true);
		//Render2D();
	}

} // Game