#include "stdafx.h"

#include "HardwareBufferManager.h"

#include "GlUitlities.h"

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

namespace SDK
{
	
	namespace Render
	{

		/*Render::VertexBufferHandle HardwareBufferManager::CreateVertexBuffer(uint i_num_verts, uint i_stride, BufferUsageFormat i_usage, const void* ip_initial_data /*= nullptr* /)
		{
			if (i_usage != BufferUsageFormat::Static)
				return VertexBufferHandle();

			auto cur_index = m_static_vertices.m_current_index;

			// if maximum we can hold
			if (cur_index == VertexBuffers::Size)
			{
				// if first buffer is not empty - we cannot hold one more allocation
				// TODO: user should be notified about error
				if (m_static_vertices.m_buffer[0].m_hardware_id != 0)
				{
					assert(false && "More static vertex buffers than can hold");
				}
				// if not - all is good and move further just flush index
				cur_index = 0;
			}

			auto handle = m_static_vertices.m_handlers[cur_index];
			auto& buffer = m_static_vertices.m_buffer[cur_index];
			auto& id = buffer.m_hardware_id;

			buffer.m_usage = i_usage;
			buffer.m_num_vertices = i_num_verts;

			// create buffer
			glGenBuffers(1, &id);
			CHECK_GL_ERRORS;

			glBindBuffer(GL_ARRAY_BUFFER, id);
			CHECK_GL_ERRORS;
			uint size = i_num_verts;
			// if stride == 0 - open GL decides that data is one-by-one
			if (i_stride != 0)
				size *= i_stride;
			glBufferData(GL_ARRAY_BUFFER, size, ip_initial_data, GL_STATIC_DRAW);
			CHECK_GL_ERRORS;

			m_static_vertices.m_current_index = cur_index + 1;
			return handle;
		}*/
		
		VertexBufferHandle HardwareBufferManager::CreateHardwareBuffer(uint i_buffer_size,
			BufferUsageFormat i_usage,
			const void* ip_initial_data /*= nullptr*/)
		{
			if (i_usage != BufferUsageFormat::Static)
				return VertexBufferHandle();

			auto cur_index = m_static_vertices.m_current_index;

			// if maximum we can hold
			if (cur_index == VertexBuffers::Size)
			{
				// if first buffer is not empty - we cannot hold one more allocation
				// TODO: user should be notified about error
				if (m_static_vertices.m_buffer[0].m_hardware_id != 0)
				{
					assert(false && "More static vertex buffers than can hold");
				}
				// if not - all is good and move further just flush index
				cur_index = 0;
			}

			auto handle = m_static_vertices.m_handlers[cur_index];
			auto& buffer = m_static_vertices.m_buffer[cur_index];
			auto& id = buffer.m_hardware_id;
			buffer.m_usage = i_usage;
			buffer.m_size_in_bytes = i_buffer_size;

			// create buffer
			glGenBuffers(1, &id);
			CHECK_GL_ERRORS;

			glBindBuffer(GL_ARRAY_BUFFER, id);
			CHECK_GL_ERRORS;
			glBufferData(GL_ARRAY_BUFFER, i_buffer_size, ip_initial_data, GL_STATIC_DRAW);
			CHECK_GL_ERRORS;

			m_static_vertices.m_current_index = cur_index + 1;
			
			return handle;
		}

		void HardwareBufferManager::DestroyBuffer(VertexBufferHandle i_handle)
		{
			auto cur_index = i_handle.index;
			assert(cur_index < VertexBuffers::Size);

			auto& buffer = m_static_vertices.m_buffer[cur_index].m_hardware_id;

			// destroy buffer
			if (buffer != 0)
			{
				glDeleteBuffers(1, &buffer);
				CHECK_GL_ERRORS;
				buffer = 0;
			}

			// increment generation after destroying
			++m_static_vertices.m_handlers[cur_index].generation;
		}

		IndexBufferHandle HardwareBufferManager::CreateIndexBuffer(HardwareIndexBuffer::IndexType i_type, size_t i_num_indices, BufferUsageFormat i_usage, PrimitiveType i_primitive, const void* ip_initial_data /*= nullptr*/)
		{
			if (i_usage != BufferUsageFormat::Static)
				return IndexBufferHandle();

			auto cur_index = m_static_indices.m_current_index;

			// if maximum we can hold
			if (cur_index == IndexBuffers::Size)
			{
				// if first buffer is not empty - we cannot hold one more allocation
				// TODO: user should be notified about error
				if (m_static_indices.m_buffer[0].m_hardware_id != 0)
				{
					assert(false && "More static vertex buffers than can hold");
				}
				// if not - all is good and move further just flush index
				cur_index = 0;
			}

			auto handle = m_static_indices.m_handlers[cur_index];
			auto& buffer = m_static_indices.m_buffer[cur_index];
			auto& id = buffer.m_hardware_id;			
			buffer.m_num_indices = i_num_indices;
			buffer.m_index_type = i_type;
			buffer.m_usage = i_usage;
			buffer.m_primitive = i_primitive;

			// create buffer
			glGenBuffers(1, &id);
			CHECK_GL_ERRORS;

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
			CHECK_GL_ERRORS;


			GLsizeiptr size = sizeof(unsigned int);
			if (i_type == HardwareIndexBuffer::IndexType::Byte)
				size = sizeof(char);
			else if (i_type == HardwareIndexBuffer::IndexType::Short)
				size = sizeof(short);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_num_indices * size, ip_initial_data, GL_STATIC_DRAW);
			CHECK_GL_ERRORS;

			m_static_indices.m_current_index = cur_index + 1;
			return handle;
		}

		void HardwareBufferManager::DestroyBuffer(IndexBufferHandle i_handle)
		{
			auto cur_index = i_handle.index;
			assert(cur_index < IndexBuffers::Size);

			auto& buffer = m_static_indices.m_buffer[cur_index].m_hardware_id;

			// destroy buffer
			if (buffer != 0)
			{
				glDeleteBuffers(1, &buffer);
				CHECK_GL_ERRORS;
				buffer = 0;
			}

			// increment generation after destroying
			++m_static_indices.m_handlers[cur_index].generation;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		HardwareVertexBuffer HardwareBufferManager::AccessVertexBuffer(VertexBufferHandle i_handle) const
		{
			if (i_handle.index < VertexBuffers::Size && m_static_vertices.m_buffer[i_handle.index].m_usage == BufferUsageFormat::Static)
				return m_static_vertices.m_buffer[i_handle.index];

			return HardwareVertexBuffer{ 0, BufferUsageFormat::Static, 0 };
		}

		HardwareIndexBuffer HardwareBufferManager::AccessIndexBuffer(IndexBufferHandle i_handle) const
		{
			if (i_handle.index < VertexBuffers::Size && m_static_indices.m_buffer[i_handle.index].m_usage == BufferUsageFormat::Static)
				return m_static_indices.m_buffer[i_handle.index];

			// set hardware id so rendere can check it
			HardwareIndexBuffer hib;
			hib.m_hardware_id = 0;
			return hib;
		}

		VertexLayout HardwareBufferManager::AccessLayout(VertexLayoutHandle i_handle) const
		{
			if (i_handle.index < VertexLayoutBuffers::Size)
				return m_static_elements.m_buffer[i_handle.index];

			VertexLayout vl;
			vl.m_vertex_size = 0;
			return vl;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		VertexLayoutHandle HardwareBufferManager::CreateLayout(
			VertexBufferHandle i_source,
			uint i_ver_size,
			VertexSemantic i_semantic,
			ComponentType i_component,
			bool i_normalized,
			uint i_stride,
			uint i_offset)
		{
			if (i_source.index >= VertexBuffers::Size || m_static_vertices.m_buffer[i_source.index].m_usage != BufferUsageFormat::Static)
				return VertexLayoutHandle{ 0, 0 };

			auto cur_index = m_static_elements.m_current_index;

			// if maximum we can hold
			if (cur_index == VertexLayoutBuffers::Size)
			{
				// if first buffer is not empty - we cannot hold one more allocation
				// TODO: user should be notified about error
				// TODO: definition of error - not as compare with enum value
				if (m_static_elements.m_buffer[0].m_vertex_size != 0)
				{
					assert(false && "More static elements than can hold");
				}
				// if not - all is good and move further just flush index
				cur_index = 0;
			}

			auto handle = m_static_elements.m_handlers[cur_index];
			auto& layout = m_static_elements.m_buffer[cur_index];

			layout.m_source = i_source.index;
			layout.m_vertex_size = i_ver_size;
			layout.m_semantic = i_semantic;
			layout.m_component = i_component;
			layout.m_normalized = i_normalized;
			layout.m_stride = i_stride;
			layout.m_offset = i_offset;

			m_static_elements.m_current_index = cur_index + 1;
			return handle;
		}

		void HardwareBufferManager::DestroyLayout(VertexLayoutHandle i_layout)
		{
			auto cur_index = i_layout.index;
			assert(cur_index < VertexLayoutBuffers::Size);

			auto& buffer = m_static_elements.m_buffer[cur_index];

			// destroy buffer
			buffer.m_vertex_size = 0;

			// increment generation after destroying
			++m_static_indices.m_handlers[cur_index].generation;
		}


	} // Render

} // SDK