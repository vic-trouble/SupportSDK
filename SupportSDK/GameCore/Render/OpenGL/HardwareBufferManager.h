#ifndef	__GAMECORE_HARDWAREBUFFERMANAGER_H__
#define __GAMECORE_HARDWAREBUFFERMANAGER_H__

#include "Render/HardwareBufferManagerBase.h"

namespace SDK
{
	
	namespace Render
	{	
		typedef GenericBuffersArray<VertexBufferHandle, HardwareVertexBuffer, 4096> VertexBuffers;
		typedef GenericBuffersArray<IndexBufferHandle, HardwareIndexBuffer, 4096> IndexBuffers;
		typedef GenericBuffersArray<ElementBufferHandle, VertexElement, 4096> ElementBuffers;

		class HardwareBufferManager : public HardwareBufferManagerBase
		{
		private:
			// for static data
			VertexBuffers	m_static_vertices;
			IndexBuffers	m_static_indices;
			ElementBuffers	m_static_elements;

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
			virtual VertexBufferHandle CreateVertexBuffer(uint i_num_verts, uint i_stride, BufferUsageFormat i_usage, const void* ip_initial_data = nullptr) override;
			virtual void DestroyBuffer(VertexBufferHandle i_handle) override;
			// can simultaneously hold 4096  buffers with usage = Static
			virtual IndexBufferHandle CreateIndexBuffer(HardwareIndexBuffer::IndexType i_type, size_t i_num_indices, BufferUsageFormat i_usage, const void* ip_initial_data = nullptr) override;
			virtual void DestroyBuffer(IndexBufferHandle i_handle) override;

			// for elements we can provide compose allocation strategy - FreList with malloc
			virtual ElementBufferHandle CreateElement(uint i_ver_size, VertexSemantic i_semantic, PrimitiveType i_primitive, ComponentType i_component, bool i_normalized) override;
			virtual void DestroyBuffer(ElementBufferHandle i_handle) override;

			////////////////////////////////////////////////////////////////////////////////////
			// Part that available to render backent - OpenGL, DirectX, other

			// TODO: should return some struct or ? - not all information about ver./ind. buffer
			HardwareVertexBuffer AccessVertexBuffer(VertexBufferHandle i_handle) const;
			HardwareIndexBuffer AccessIndexBuffer(IndexBufferHandle i_handle) const;
			VertexElement AccessElement(ElementBufferHandle i_handle) const;

			// use for other buffers - colors/uvs/deepth...
			/*typedef int BufHandle;
			VertexBufferHandle CreateBuffer(const VertexDeclaration* ip_declaration, uint i_count, const void* ip_data, BufferUsageFormat i_usage);
			void DestroyBuffer(VertexBufferHandle i_handle, BufHandle);*/
		};

	} // Render

} // SDK

#endif