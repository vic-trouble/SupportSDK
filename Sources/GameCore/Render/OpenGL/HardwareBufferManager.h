#ifndef	__GAMECORE_HARDWAREBUFFERMANAGER_H__
#define __GAMECORE_HARDWAREBUFFERMANAGER_H__

#include "Render/HardwareBufferManagerBase.h"

namespace SDK
{
	
	namespace Render
	{	
		typedef GenericBuffersArray<VertexBufferHandle, HardwareVertexBuffer, 4096> VertexBuffers;
		typedef GenericBuffersArray<IndexBufferHandle, HardwareIndexBuffer, 4096> IndexBuffers;
		typedef GenericBuffersArray<VertexLayoutHandle, VertexLayout, 4096> VertexLayoutBuffers;

		class HardwareBufferManager : public HardwareBufferManagerBase
		{
		private:
			// for static data
			VertexBuffers	m_static_vertices;
			IndexBuffers	m_static_indices;
			VertexLayoutBuffers	m_static_elements;

		public:
			// TODO: allocate configurable number objects
			/* TODO: shadow_buffer - 
				  If set to @c true, this buffer will be 'shadowed' by one stored in 
					system memory rather than GPU or AGP memory. You should set this flag if you intend 
					to read data back from the index buffer, because reading data from a buffer
					in the GPU or AGP memory is very expensive, and is in fact impossible if you
					specify HBU_WRITE_ONLY for the main buffer. If you use this option, all 
					reads and writes will be done to the shadow buffer, and the shadow buffer will
					be synchronised with the real buffer at an appropriate time.
			*/
			// can simultaneously hold 4096  buffers with usage = Static
			//virtual VertexBufferHandle CreateVertexBuffer(uint i_num_verts, uint i_stride, BufferUsageFormat i_usage, const void* ip_initial_data = nullptr) override;
			virtual VertexBufferHandle CreateHardwareBuffer(uint i_buffer_size,
															BufferUsageFormat i_usage,
															const void* ip_initial_data = nullptr);
			virtual void DestroyBuffer(VertexBufferHandle i_handle) override;
			// can simultaneously hold 4096  buffers with usage = Static
			virtual IndexBufferHandle CreateIndexBuffer(HardwareIndexBuffer::IndexType i_type, size_t i_num_indices, BufferUsageFormat i_usage, PrimitiveType i_primitive, const void* ip_initial_data = nullptr) override;
			virtual void DestroyBuffer(IndexBufferHandle i_handle) override;

			virtual VertexLayoutHandle CreateLayout(
				VertexBufferHandle i_source,
				uint i_ver_size,
				VertexSemantic i_semantic,
				ComponentType i_component,
				bool i_normalized,
				uint i_stride,
				uint i_offset) override;
			virtual void DestroyLayout(VertexLayoutHandle i_layout) override;

			////////////////////////////////////////////////////////////////////////////////////
			// Part that available to render backent - OpenGL, DirectX, other

			// TODO: should return some struct or ? - not all information about ver./ind. buffer
			HardwareVertexBuffer AccessVertexBuffer(VertexBufferHandle i_handle) const;
			HardwareIndexBuffer AccessIndexBuffer(IndexBufferHandle i_handle) const;
			VertexLayout AccessLayout(VertexLayoutHandle i_handle) const;

			// use for other buffers - colors/uvs/deepth...
			/*typedef int BufHandle;
			VertexBufferHandle CreateBuffer(const VertexDeclaration* ip_declaration, uint i_count, const void* ip_data, BufferUsageFormat i_usage);
			void DestroyBuffer(VertexBufferHandle i_handle, BufHandle);*/
		};

	} // Render

} // SDK

#endif