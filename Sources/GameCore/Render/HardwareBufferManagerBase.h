#ifndef	__GAMECORE_HARDWAREBUFFERMANAGERBASE_H__
#define __GAMECORE_HARDWAREBUFFERMANAGERBASE_H__

#include "RenderTypes.h"
#include "HardwareVertexBuffer.h"
#include "HardwareIndexBuffer.h"
#include "HardwareBuffer.h"

namespace SDK
{

	namespace Render
	{
		class HardwareBufferManagerBase
		{
		public:
			virtual ~HardwareBufferManagerBase(){}

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
			virtual VertexBufferHandle CreateHardwareBuffer(uint i_buffer_size, 
															BufferUsageFormat i_usage, 
															const void* ip_initial_data = nullptr) = 0;
			virtual void DestroyBuffer(VertexBufferHandle i_handle) = 0;
			// can simultaneously hold 4096  buffers with usage = Static
			virtual IndexBufferHandle CreateIndexBuffer(HardwareIndexBuffer::IndexType i_type, size_t i_num_indices, BufferUsageFormat i_usage, PrimitiveType i_primitive, const void* ip_initial_data = nullptr) = 0;
			virtual void DestroyBuffer(IndexBufferHandle i_handle) = 0;
			
			virtual VertexLayoutHandle CreateLayout(
				VertexBufferHandle i_source,
				uint i_ver_size,
				VertexSemantic i_semantic,
				ComponentType i_component,
				bool i_normalized,
				uint i_stride,
				uint i_offset) = 0;
			virtual void DestroyLayout(VertexLayoutHandle i_layout) = 0;
		};
	}

} // SDK

#endif