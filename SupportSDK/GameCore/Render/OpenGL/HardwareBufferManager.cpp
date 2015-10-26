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

		Render::VertexBufferHandle HardwareBufferManager::CreateVertexBuffer(uint i_num_verts, uint i_stride, BufferUsageFormat i_usage, const void* ip_initial_data /*= nullptr*/)
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
			buffer.m_stride = i_stride;

			// create buffer
			glGenBuffers(1, &id);
			CHECK_GL_ERRORS;

			glBindBuffer(GL_ARRAY_BUFFER, id);
			CHECK_GL_ERRORS;

			glBufferData(GL_ARRAY_BUFFER, i_num_verts * i_stride, ip_initial_data, GL_STATIC_DRAW);
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

		IndexBufferHandle HardwareBufferManager::CreateIndexBuffer(HardwareIndexBuffer::IndexType i_type, size_t i_num_indices, BufferUsageFormat i_usage, const void* ip_initial_data /*= nullptr*/)
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

			return HardwareVertexBuffer();
		}

		HardwareIndexBuffer HardwareBufferManager::AccessIndexBuffer(IndexBufferHandle i_handle) const
		{
			if (i_handle.index < VertexBuffers::Size && m_static_indices.m_buffer[i_handle.index].m_usage == BufferUsageFormat::Static)
				return m_static_indices.m_buffer[i_handle.index];

			return HardwareIndexBuffer();
		}

		VertexElement HardwareBufferManager::AccessElement(ElementBufferHandle i_handle) const
		{
			if (i_handle.index < ElementBuffers::Size && m_static_indices.m_buffer[i_handle.index].m_usage == BufferUsageFormat::Static)
				return m_static_elements.m_buffer[i_handle.index];

			return VertexElement();
		}

		/////////////////////////////////////////////////////////////////////////////////////

		ElementBufferHandle HardwareBufferManager::CreateElement(uint i_ver_size, VertexSemantic i_semantic, PrimitiveType i_primitive, ComponentType i_component, bool i_normalized)
		{
			auto cur_index = m_static_elements.m_current_index;

			// if maximum we can hold
			if (cur_index == ElementBuffers::Size)
			{
				// if first buffer is not empty - we cannot hold one more allocation
				// TODO: user should be notified about error
				// TODO: definition of error - not as compare with enum value
				if (m_static_elements.m_buffer[0].m_semantic != VertexSemantic::Count)
				{
					assert(false && "More static elements than can hold");
				}
				// if not - all is good and move further just flush index
				cur_index = 0;
			}

			auto handle = m_static_elements.m_handlers[cur_index];
			auto& buffer = m_static_elements.m_buffer[cur_index];

			buffer.m_vertex_size = i_ver_size;
			buffer.m_semantic = i_semantic;
			buffer.m_primitive = i_primitive;
			buffer.m_component = i_component;
			buffer.m_normalized = i_normalized;
			buffer.m_stride = 0;
			buffer.m_offset = 0;

			// create element
			
			m_static_elements.m_current_index = cur_index + 1;
			return handle;
		}

		void HardwareBufferManager::DestroyBuffer(ElementBufferHandle i_handle)
		{
			auto cur_index = i_handle.index;
			assert(cur_index < IndexBuffers::Size);

			auto& buffer = m_static_elements.m_buffer[cur_index];

			// destroy buffer
			buffer.m_semantic = VertexSemantic::Count;

			// increment generation after destroying
			++m_static_indices.m_handlers[cur_index].generation;
		}

	} // Render

} // SDK