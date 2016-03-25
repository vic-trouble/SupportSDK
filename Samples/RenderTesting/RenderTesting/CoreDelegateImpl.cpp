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

namespace Game
{
	EntityHandle entity_handler;
	void CoreDelegateImpl::LoadModel()
	{
		//E:\Git_Projects\SupportSDK\Samples\Resources\Models\Box.obj
		//loaded_mesh = Render::g_mesh_system.Load("Resources\\Models\\Box.obj", Render::BufferUsageFormat::Static, Render::BufferUsageFormat::Static);
		auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
		loaded_mesh = p_load_manager->GetHandleToResource<Render::Mesh>("SimpleBox");
		
		auto mesh_handler = Render::g_mesh_system.CreateInstance(loaded_mesh);
		auto trans_handler = g_transforms_system.CreateInstance();
		auto p_transform = g_transforms_system.GetInstance(trans_handler);

		entity_handler = g_entity_manager.CreateEntity();
		g_entity_manager.AddComponent<Render::MeshComponent>(entity_handler, mesh_handler);
		g_entity_manager.AddComponent<Transform>(entity_handler, trans_handler);

		// test getting of entity and component
		auto entity = g_entity_manager.GetEntity(entity_handler);
		
		Render::MaterialHandle material_handle = p_load_manager->GetHandleToResource<Render::Material>("Sample_material");
		auto p_material = Render::g_material_mgr.AccessMaterial(material_handle);

		Render::g_mesh_system.AddMaterialTo(mesh_handler, material_handle);
	}

	void CoreDelegateImpl::CreateMesh()
	{	
		auto p_renderer = Core::GetRenderer();
		auto p_mgr = p_renderer->GetHardwareBufferMgr();

	}

	void CoreDelegateImpl::OnCreate()
	{
		InputSystem::Instance().AddSubscriber(&m_input_subs);
		auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
		p_load_manager->LoadResourceSet("..\\..\\Resources\\ResourceSets\\render_testing.res");

		LoadModel();

		CreateMesh();

		auto& world = Core::GetApplication()->GetWorld();
		auto& camera = world.GetCamera();

		camera.LookAt({ 0,-5,-15 }, { -10,-2,0 });

		world.GetFrustum().SetFOV(60 * Math::DEG2RAD);
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

	void CoreDelegateImpl::Draw()
	{
	}

} // Game