#include "stdafx.h"

#include "CoreDelegateImpl.h"

#include <GameCore/Core.h>
#include <GameCore/Render/IRenderer.h>
#include <GameCore/Render/HardwareBufferManagerBase.h>
#include <GameCore/Input/inputsystem.h>
#include <GameCore/Render/OpenGL/GlUitlities.h>

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

#include <GameCore/Systems/MeshSystem.h>

#include <GameCore/Render/ShaderSystem.h>

namespace Game
{

	void CoreDelegateImpl::LoadModel()
	{
		//E:\Git_Projects\SupportSDK\Samples\Resources\Models\Box.obj
		//loaded_mesh = Render::g_mesh_system.Load("Resources\\Models\\Box.obj", Render::BufferUsageFormat::Static, Render::BufferUsageFormat::Static);
		loaded_mesh = Render::g_mesh_system.Load("..\\..\\..\\Resources\\Models\\Box.obj", Render::BufferUsageFormat::Static, Render::BufferUsageFormat::Static);
		Render::g_mesh_system.Unload(loaded_mesh);
		loaded_mesh = Render::g_mesh_system.Load("..\\..\\..\\Resources\\Models\\Box.obj", Render::BufferUsageFormat::Static, Render::BufferUsageFormat::Static);

		auto& mesh = Render::g_mesh_system.Get(loaded_mesh);
		mesh.position = { 2.f, 0.f, 0.f };

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
		batch[0].vertices = p_mgr->CreateVertexBuffer(sizeof(verts)/sizeof(float), sizeof(float) * 3, Render::BufferUsageFormat::Static, verts);
		batch[0].indices = p_mgr->CreateIndexBuffer(Render::HardwareIndexBuffer::IndexType::Int, sizeof(inds) / sizeof(uint), Render::BufferUsageFormat::Static, inds);
		batch[0].element = p_mgr->CreateElement(3, Render::VertexSemantic::Position, Render::PrimitiveType::Triangles, Render::ComponentType::Float, false);
	}	

	GLUquadricObj* quadricId;
	Render::ShaderHandler shader_handler;
	

	void CoreDelegateImpl::OnCreate()
	{
		InputSystem::Instance().AddSubscriber(&m_input_subs);

		LoadModel();

		CreateMesh();

		auto& world = Core::GetApplication()->GetWorld();
		auto& camera = world.GetCamera();
		
		camera.SetPosition({ 0,0,-15 });
		camera.Pitch(21 * Math::DEG2RAD);
		camera.Yaw(146 * Math::DEG2RAD);

		world.GetFrustum().SetFOV(60 * Math::DEG2RAD);
		// create a GLU quadric object
		quadricId = gluNewQuadric();
		gluQuadricDrawStyle(quadricId, GLU_FILL);
		shader_handler = Render::g_shader_system.Load("SimpleShader", "..\\..\\..\\Resources\\Shaders\\SimpleShader.vertexshader", "..\\..\\..\\Resources\\Shaders\\SimpleShader.fragmentshader");
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
			// disable lighting
			glDisable(GL_LIGHTING);

			glBegin(GL_LINES);

			glColor3f(0.3f, 0.3f, 0.3f);
			for (float i = step; i <= size; i += step)
			{
				glVertex3f(-size, 0, i);   // lines parallel to X-axis
				glVertex3f(size, 0, i);
				glVertex3f(-size, 0, -i);   // lines parallel to X-axis
				glVertex3f(size, 0, -i);

				glVertex3f(i, 0, -size);   // lines parallel to Z-axis
				glVertex3f(i, 0, size);
				glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
				glVertex3f(-i, 0, size);
			}

			// x-axis
			glColor3f(0.5f, 0, 0);
			glVertex3f(-size, 0, 0);
			glVertex3f(size, 0, 0);

			// z-axis
			glColor3f(0, 0, 0.5f);
			glVertex3f(0, 0, -size);
			glVertex3f(0, 0, size);

			glEnd();

			// enable lighting back
			glEnable(GL_LIGHTING);
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
	}

	void CoreDelegateImpl::Draw()
	{
		//Render::Shader shader = Render::g_shader_system.Access(shader_handler);

		draw3(true);
	}

} // Game