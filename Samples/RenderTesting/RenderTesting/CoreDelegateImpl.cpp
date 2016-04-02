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
#include <GameCore/Render/ShaderCompiler.h>
#include <GameCore/Render/Fonts/FontManager.h>

namespace Game
{
	struct CameraController : public SDK::InputSubscriber
	{
		float m_player_speed = 1.5f;
		SDK::Vector3 m_direction;
		SDK::Vector3 m_position;

		virtual bool KeyPressed(const SDK::KeyEvent& i_evt)
		{
			if (i_evt.m_key_code == VK_LEFT)
				m_direction[0] = m_player_speed;
			else if (i_evt.m_key_code == VK_RIGHT)
				m_direction[0] = -m_player_speed;
			else if (i_evt.m_key_code == VK_UP)
				m_direction[2] = m_player_speed;
			else if (i_evt.m_key_code == VK_DOWN)
				m_direction[2] = -m_player_speed;
			else if (i_evt.m_key_code == 0x5A) // z
				m_direction[1] = m_player_speed;
			else if (i_evt.m_key_code == 0x58) // X
				m_direction[1] = -m_player_speed;
			return false;
		}
		virtual bool KeyReleased(const SDK::KeyEvent& i_evt)
		{
			if (i_evt.m_key_code == VK_LEFT || i_evt.m_key_code == VK_RIGHT)
				m_direction[0] = 0;
			else if (i_evt.m_key_code == VK_UP || i_evt.m_key_code == VK_DOWN)
				m_direction[2] = 0;
			else if (i_evt.m_key_code == 0x5A || i_evt.m_key_code == 0x58)
				m_direction[1] = 0;
			return false;
		}

		void Update()
		{
			auto& camera = Core::GetApplication()->GetWorld().GetCamera();
			static SDK::Vector3 look_at{ -500,-500, 1500 };
			static SDK::Vector3 shift{ 5.f, -5.f, -50.f };

			auto pos = camera.GetPosition() + m_direction;
			auto target = camera.GetPosition() + look_at;
			camera.LookAt(pos, target);

		}
	} g_camera_controller;

	EntityHandle entity_handle;
	void CoreDelegateImpl::LoadModel()
	{
		//E:\Git_Projects\SupportSDK\Samples\Resources\Models\Box.obj
		//loaded_mesh = Render::g_mesh_system.Load("Resources\\Models\\Box.obj", Render::BufferUsageFormat::Static, Render::BufferUsageFormat::Static);
		auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
		loaded_mesh = p_load_manager->GetHandleToResource<Render::Mesh>("Nanosuit");
		
		const Vector3 start_pos{ -100, 0, -100 };
		for (int i = 0; i < 20; ++i)
		{
			for (int j = 0; j < 20; ++j)
			{
				auto mesh_handle = Render::g_mesh_system.CreateInstance(loaded_mesh);
				auto trans_handle = g_transforms_system.CreateInstance();
				auto p_transform = g_transforms_system.GetInstance(trans_handle);
				p_transform->m_position[0] = start_pos[0] + i*10;
				p_transform->m_position[2] = start_pos[2] + j*10;

				entity_handle = g_entity_manager.CreateEntity();
				g_entity_manager.AddComponent<Render::MeshComponent>(entity_handle, mesh_handle);
				g_entity_manager.AddComponent<Transform>(entity_handle, trans_handle);

				// test getting of entity and component
				auto entity = g_entity_manager.GetEntity(entity_handle);
			}
		}
	}

	void CoreDelegateImpl::CreateMesh()
	{	
		auto p_renderer = Core::GetRenderer();
		auto p_mgr = p_renderer->GetHardwareBufferMgr();

	}

	void CoreDelegateImpl::OnCreate()
	{
		InputSystem::Instance().AddSubscriber(&m_input_subs);
		InputSystem::Instance().AddSubscriber(&g_camera_controller);
		auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
		p_load_manager->LoadResourceSet("..\\..\\Resources\\ResourceSets\\render_testing.res");

		LoadModel();		
		CreateMesh();
		Render::g_font_manager.LoadFont("Arial", "..\\..\\Resources\\Fonts\\arial.ttf");
		auto& world = Core::GetApplication()->GetWorld();
		auto& camera = world.GetCamera();
		static SDK::Vector3 shift{ 5.f, -5.f, -50.f };
		static SDK::Vector3 look_at{ -500,-500, 1500 };
		camera.LookAt(shift, look_at);

		world.GetFrustum().SetFOV(80 * Math::DEG2RAD);
		world.GetFrustum().SetFarClipDistance(1000.f);
		world.GetFrustum().SetNearClipDistance(0.5f);
		Core::GetRenderer()->SetClearColor(Color(128, 112, 112, 255));
	}

	void CoreDelegateImpl::OnTerminate()
	{
		InputSystem::Instance().RemoveSubscriber(&m_input_subs);
		InputSystem::Instance().RemoveSubscriber(&g_camera_controller);

		auto p_renderer = Core::GetRenderer();

		g_entity_manager.RemoveEntity(entity_handle);

		auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
		p_load_manager->UnloadSet(p_load_manager->GetHandleToSet("render_testing"));
	}


	void CoreDelegateImpl::Update(float i_elapsed_time)
	{
		g_camera_controller.Update();
	}

	namespace {
		void drawGrid(float size, float step)
		{
			auto p_render = SDK::Core::GetRenderer();
			auto& world = SDK::Core::GetApplication()->GetWorld();
			p_render->SetMatrix(SDK::MatrixMode::Projection, world.GetFrustum().GetProjectionMatrix());
			p_render->SetMatrix(SDK::MatrixMode::ModelView, world.GetCamera().GetModelViewMatrix());
			p_render->SetMatrixMode(SDK::MatrixMode::ModelView);
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
	}

	void CoreDelegateImpl::Draw()
	{
		auto p_lights = Core::GetRenderer()->GetLightsController();
		p_lights->DisableLighting();
		drawGrid(100, 4);
		p_lights->EnableLighting();
		static float x = 50;
		static float y = 50;
		IRect rect = Core::GetRenderer()->GetTargetRectangle();
		Core::GetRenderer()->SetMatrix(MatrixMode::Projection, Matrix4f::CreateOrtho(0, rect.Width(), 0, rect.Height()));
		std::wstring message = L"FPS: " + std::to_wstring(Core::GetApplication()->GetCurrentFPS());
		Render::g_font_manager.Render({ x, y }, 1.f, message);
	}

} // Game