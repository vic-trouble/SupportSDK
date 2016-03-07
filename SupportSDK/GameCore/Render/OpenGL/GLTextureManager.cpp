#include "stdafx.h"

#include "GLTextureManager.h"

#include "Core.h"
#include "Render/IRenderer.h"
#include "Resources/ResourceManager.h"

#include "FileSystem/FileSystem.h"

#include <SOIL.h>

#include <GL/glew.h>

namespace SDK
{
	namespace Resources
	{
		namespace Serialization
		{

			template <>
			struct Definition <Render::Texture>
			{
				typedef std::string InfoType;
			};

			template <>
			struct LoaderImpl < Render::Texture >
			{
				static std::pair<LoadResult, Render::Texture> Load(std::istream& io_stream, std::string i_path)
				{					
					int width, height;				
					unsigned char* image = SOIL_load_image(i_path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
					if (image == nullptr)
						return std::make_pair(LoadResult::Failure, Render::Texture());

					GLuint texture;
					glGenTextures(1, &texture);
					glBindTexture(GL_TEXTURE_2D, texture);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
					glGenerateMipmap(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, 0);

					SOIL_free_image_data(image);
					
					Render::Texture t;
					t.hardware_id = texture;
					t.type = Render::TextureType::TwoD;
					t.image_format = Render::ImageFormat::RGBA;

					return std::make_pair(LoadResult::Success, t);
				}

				static int CreateNewHandle()
				{
					Render::GLTextureManager* p_mgr = static_cast<Render::GLTextureManager*>(Core::GetRenderer()->GetTextureManager());
					Render::TextureHandle handle = p_mgr->m_textures.Create();

					return handle.index;
				}

				static void RemoveHandle(int i_handle)
				{
					Render::GLTextureManager* p_mgr = static_cast<Render::GLTextureManager*>(Core::GetRenderer()->GetTextureManager());
					p_mgr->m_textures.m_elements[i_handle].second = Render::Texture();
					p_mgr->m_textures.m_elements[i_handle].first.index = -1;
					++p_mgr->m_textures.m_elements[i_handle].first.generation;
				}

				static void UnloadResource(int i_handle)
				{
					Render::GLTextureManager* p_mgr = static_cast<Render::GLTextureManager*>(Core::GetRenderer()->GetTextureManager());
					p_mgr->m_textures.m_elements[i_handle].second = Render::Texture();
					p_mgr->m_textures.m_elements[i_handle].first.index = -1;
					++p_mgr->m_textures.m_elements[i_handle].first.generation;
					
				}

				static void Register(int i_handle, Render::Texture i_texture)
				{
					Render::GLTextureManager* p_mgr = static_cast<Render::GLTextureManager*>(Core::GetRenderer()->GetTextureManager());
					p_mgr->m_textures.m_elements[i_handle].second = i_texture;
				}
			};

		} // Serialization
	} // Resources

	namespace Render
	{
		GLTextureManager::~GLTextureManager()
		{
		}

		TextureHandle GLTextureManager::Load(const std::string & i_file_name)
		{
			auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
			const auto app_path = FS::GetApplicationPath();
			auto path = app_path;
			path.append("\\").append(i_file_name);
			int index = p_load_manager->Load<Texture>(i_file_name, path);
			// resource is already loaded
			if (index != -1)
			{
				assert(index < static_cast<int>(TexturesArray::Size));
				return m_textures.m_elements[index].first;
			}
			
			return TextureHandle::InvalidHandle();
		}
	} // Render
} // SDK