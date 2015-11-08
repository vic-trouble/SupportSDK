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

namespace Game
{

	void CoreDelegateImpl::LoadModel()
	{
		//E:\Git_Projects\SupportSDK\Samples\Resources\Models\Box.obj
		loaded_mesh = Render::g_mesh_system.Load("Resources\\Models\\Box.obj", Render::BufferUsageFormat::Static, Render::BufferUsageFormat::Static);
		loaded_mesh = Render::g_mesh_system.Load("..\\..\\..\\Resources\\Models\\Box.obj", Render::BufferUsageFormat::Static, Render::BufferUsageFormat::Static);
		Render::g_mesh_system.Unload(loaded_mesh);
		loaded_mesh = Render::g_mesh_system.Load("..\\..\\..\\Resources\\Models\\Box.obj", Render::BufferUsageFormat::Static, Render::BufferUsageFormat::Static);
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

	Matrix4f CreateViewMatrix(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
	{
		float forward[4];
		float up[4];
		float left[4];
		float position[4];
		float invLength;

		// determine forward vector (direction reversed because it is camera)
		forward[0] = posX - targetX;    // x
		forward[1] = posY - targetY;    // y
		forward[2] = posZ - targetZ;    // z
		forward[3] = 0.0f;              // w
		// normalize it without w-component
		invLength = 1.0f / sqrtf(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);
		forward[0] *= invLength;
		forward[1] *= invLength;
		forward[2] *= invLength;

		// assume up direction is straight up
		up[0] = 0.0f;   // x
		up[1] = 0.0f;   // y
		up[2] = 1.0f;   // z
		up[3] = 0.0f;   // w

		// compute left vector with cross product
		left[0] = up[1] * forward[2] - up[2] * forward[1];  // x
		left[1] = up[2] * forward[0] - up[0] * forward[2];  // y
		left[2] = up[0] * forward[1] - up[1] * forward[0];  // z
		left[3] = 1.0f;                                 // w

		// re-compute orthogonal up vector
		up[0] = forward[1] * left[2] - forward[2] * left[1];    // x
		up[1] = forward[2] * left[0] - forward[0] * left[2];    // y
		up[2] = forward[0] * left[1] - forward[1] * left[0];    // z
		up[3] = 0.0f;                                       // w

		// camera position
		position[0] = posX;
		position[1] = posY;
		position[2] = posZ;
		position[3] = 1.0f;

		Matrix4f model_matrix;
		// copy axis vectors to matrix
		model_matrix.Identity();
		model_matrix[0][0] = left[0]; model_matrix[1][0] = left[1]; model_matrix[2][0] = left[2]; model_matrix[3][0] = left[3];
		model_matrix[0][1] = up[0]; model_matrix[1][1] = up[1]; model_matrix[2][1] = up[2]; model_matrix[3][1] = up[3];
		model_matrix[0][2] = forward[0]; model_matrix[1][2] = forward[1]; model_matrix[2][2] = forward[2]; model_matrix[3][2] = forward[3];
		model_matrix[0][3] = position[0]; model_matrix[1][3] = position[1]; model_matrix[2][3] = position[2]; model_matrix[3][3] = position[3];
		return model_matrix;
	}

	GLUquadricObj* quadricId;

	const float DEG2RAD = 3.141593f / 180;
	void SetMatrix()
	{
		auto& world = Core::GetApplication()->GetWorld();
		auto p_renderer = Core::GetRenderer();
		auto rect = p_renderer->GetTargetRectangle();
		//world.m_frustum.SetFrustum(100.f*DEG2RAD, (float)rect.Width() / rect.Height(), 1.f, 100.f);
		world.m_frustum.SetFrustum(60.f*DEG2RAD, (float)rect.Width() / rect.Height(), 1.f, 1000.f);
		world.m_frustum.SetProjectionType(Render::ProjectionType::Perspective);
		Matrix4f matrixModel;
		matrixModel.Identity();
		Matrix4f matrixView;
		matrixView.Identity();
		matrixView.CreateTranslation(Math::VectorConstructor<float>::Construct(0, 0, -15));
		world.m_camera.m_modelview_matrix = matrixView * matrixModel;

		// create a GLU quadric object
		quadricId = gluNewQuadric();
		gluQuadricDrawStyle(quadricId, GLU_FILL);
	}

	void CoreDelegateImpl::OnCreate()
	{
		InputSystem::Instance().AddSubscriber(&m_input_subs);

		LoadModel();

		CreateMesh();

		SetMatrix();
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
			static int windowWidth = 640;
			static int windowHeight = 960;
			static float FOV_Y = 60.f;
			// set bottom viewport (perspective)
			glViewport(0, 0, windowWidth, windowHeight);
			glScissor(0, 0, windowWidth, windowHeight);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(FOV_Y, windowWidth / float(windowHeight), 1, 1000);

			// switch to modelview matrix
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// clear buffer
			glClearColor(0.0f, 0.0f, 0.0f, 0.f);   // background color

			glPushMatrix();
				// First, transform the camera (viewing matrix) from world space to eye space
				glTranslatef(0, 0, -15.f);
				glRotatef(21, 1, 0, 0); // pitch
				glRotatef(146, 0, 1, 0); // heading

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
		auto p_renderer = Core::GetRenderer();
		//p_renderer->Draw(batch[1]);
		draw3(true);		
	}

} // Game