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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

		entity_handler = g_entity_manager.CreateEntity();
		g_entity_manager.AddComponent<Render::MeshComponent>(entity_handler, mesh_handler);
		g_entity_manager.AddComponent<Transform>(entity_handler, trans_handler);

		// test getting of entity and component
		auto entity = g_entity_manager.GetEntity(entity_handler);
		
		auto material_handle_int = p_load_manager->GetHandleToResource("SimpleBox");
		Render::MaterialHandle material_handle = { material_handle_int.index, material_handle_int.generation };
		auto p_material = Render::g_material_mgr.AccessMaterial(material_handle);

		Render::g_mesh_system.AddMaterialTo(mesh_handler, material_handle);
	}

	void CoreDelegateImpl::CreateMesh()
	{	
		auto p_renderer = Core::GetRenderer();
		auto p_mgr = p_renderer->GetHardwareBufferMgr();

	}

	namespace
	{
		using namespace std;
		struct Vertex {
			// Position
			Vector3 Position;
			// Normal
			Vector3 Normal;
			// TexCoords
			Vector2 TexCoords;
		};
		// Data to fill
		vector<Vertex> vertices;
		vector<GLuint> indices;
		/*  Render data  */
		GLuint VAO, VBO, EBO;

		/*  Functions    */
		// Initializes all the buffer objects/arrays
		void setupMesh()
		{
			// Create buffers/arrays
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			// Load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// A great thing about structs is that their memory layout is sequential for all its items.
			// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
			// again translates to 3/2 floats which translates to a byte array.
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

			// Set the vertex attribute pointers
			// Vertex Positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
			// Vertex Normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
			// Vertex Texture Coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

			glBindVertexArray(0);
		}

		void processMesh(aiMesh* mesh, const aiScene* scene)
		{
			// Walk through each of the mesh's vertices
			for (GLuint i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				Vector3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
								  // Positions
				vector[0] = mesh->mVertices[i].x;
				vector[1] = mesh->mVertices[i].y;
				vector[2] = mesh->mVertices[i].z;
				vertex.Position = vector;
				// Normals
				vector[0] = mesh->mNormals[i].x;
				vector[1] = mesh->mNormals[i].y;
				vector[2] = mesh->mNormals[i].z;
				vertex.Normal = vector;
				// Texture Coordinates
				if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
				{
					Vector2 vec;
					// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vec[0] = mesh->mTextureCoords[0][i].x;
					vec[1] = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = vec;
				}
				else
					vertex.TexCoords = Vector2{ 0.0f, 0.0f };
				vertices.push_back(vertex);
			}
			// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (GLuint i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				// Retrieve all indices of the face and store them in the indices vector
				for (GLuint j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}
		}

		void loadModel(std::string path)
		{
			// Read file via ASSIMP
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
			// Check for errors
			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
				return;
			}

			// Process ASSIMP's root node recursively
			processMesh(scene->mMeshes[0], scene);
			setupMesh();			
		}

		void DrawX()
		{
			glPushMatrix();
			glTranslatef(0, 0, 0);

			CHECK_GL_ERRORS;
			auto p_renderer = Core::GetRenderer();
			auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
			auto tex = p_load_manager->GetHandleToResource("box_diffuse");
			auto shader = p_load_manager->GetHandleToResource("SimpleShader");
			Render::ShaderHandler sh{ shader.index, shader.generation };
			const Render::Shader* p_shader = Render::g_shader_system.Access(sh);
			{
				p_renderer->GetLightsController()->EnableLighting();
				// set up light colors (ambient, diffuse, specular)
				GLfloat lightKa[] = { .0f, .0f, .0f, 1.0f };      // ambient light
				GLfloat lightKd[] = { .9f, .9f, .9f, 1.0f };      // diffuse light
				GLfloat lightKs[] = { 1, 1, 1, 1 };               // specular light
				int light_num = GL_LIGHT0;
				glLightfv(light_num, GL_AMBIENT, lightKa);
				glLightfv(light_num, GL_DIFFUSE, lightKd);
				glLightfv(light_num, GL_SPECULAR, lightKs);
				// position the light in eye space
				float lightPos[4] = { 0, 0, 1, 0 };               // directional light
				glLightfv(light_num, GL_POSITION, lightPos);

				glEnable(light_num);
				glUseProgram(p_shader->GetId());
				Render::g_shader_system.SetKnownUniforms(sh);
				CHECK_GL_ERRORS;
				p_renderer->GetTextureManager()->Bind(0, { tex.index, tex.generation });
				CHECK_GL_ERRORS;
				int i = 0;
				glUniform1i(p_shader->GetUniform("diffuse_tex").location, 0);
				CHECK_GL_ERRORS;
			}
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glPopMatrix();
		}

	} // namespace

	void CoreDelegateImpl::OnCreate()
	{
		InputSystem::Instance().AddSubscriber(&m_input_subs);
		auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
		p_load_manager->LoadResourceSet("..\\..\\Resources\\ResourceSets\\render_testing.res");

		//LoadModel();

		CreateMesh();

		auto& world = Core::GetApplication()->GetWorld();
		auto& camera = world.GetCamera();

		camera.LookAt({ 0,-4,-7 }, { -10,-2,0 });

		world.GetFrustum().SetFOV(60 * Math::DEG2RAD);

		loadModel("E:\\Git_Projects\\SupportSDK\\Samples\\Resources\\Models\\box.obj");
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
		DrawX();
	}

} // Game