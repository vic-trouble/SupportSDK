#ifndef	__GAMECORE_RENDERTYPES_H__
#define __GAMECORE_RENDERTYPES_H__

#include "GameCore/GenericHandle.h"

namespace SDK
{
	
	namespace Render
	{

		enum class BufferUsageFormat
		{
			/*
				Static buffer which the application rarely modifies once created. Modifying
				the contents of this buffer will involve a performance hit.
			*/
			Static = 1,
			/*
				Indicates the application would like to modify this buffer with the CPU
					fairly often. 
				Buffers created with this flag will typically end up in AGP memory rather 
					than video memory.
			*/
			Dynamic = 2,

			/*
				Indicates the application will never read the contents of the buffer back, 
			    it will only ever write data. Locking a buffer with this flag will ALWAYS 
			    return a pointer to new, blank memory rather than the memory associated 
			    with the contents of the buffer; this avoids DMA stalls because you can 
			    write to a new memory area while the previous one is being used. 
			*/
			WriteOnly = 4,
			/*
				Indicates that the application will be refilling the contents
                of the buffer regularly (not just updating, but generating the
                contents from scratch), and therefore does not mind if the contents 
                of the buffer are lost somehow and need to be recreated. This
                allows and additional level of optimisation on the buffer.
                This option only really makes sense when combined with 
                HBU_DYNAMIC_WRITE_ONLY.
			*/
			Discardable = 8,

			///	Combination of Static and WriteOnly
			StaticWriteOnly = 5,

			/*
				Combination of Dynamic and WriteOnly. If you use
				this, strongly consider using DynamicWriteOnly
				instead if you update the entire contents of the buffer very
				regularly.
			*/
			DynamicWriteOnly = 6,
			/// Combination of Dynamic, WriteOnly and Discardable
			DynamicWriteOnlyDiscardable = 14
		};

		enum class PrimitiveType
		{
			Points,
			LineStrip,
			LineLoop,
			Lines,
			TriangleStrip,
			TriangleFan,
			Triangles,
			QuadStrip,
			Quads,
			Polygon
		};

		// component type for openGL - glVertexAttribPointer
		enum class ComponentType
		{
			Byte,
			UByte,
			Shot,
			UShort,
			Int,
			UInt,
			HalfFloat,
			Float,
			Double,
			Fixed // ?
		};

		enum class VertexSemantic : char
		{
			Position = 1,
			BlendWeights = 2,
			BlendIndices = 3,
			Normal = 4,
			Diffuse = 5,
			Specular = 6,
			TextureCoordinates = 7,
			Binomial = 8,
			Tangent = 9,

			Count
		};

		struct VertexLayout
		{
			/// The source vertex buffer, as bound to an index using VertexBufferBinding
			//unsigned short	m_source;
			// 1, 2, 3 or 4
			uint			m_vertex_size;
			/// The meaning of the element
			VertexSemantic	m_semantic;
			/// The type of element
			ComponentType	m_component;
			bool			m_normalized;
			PrimitiveType	m_primitive;
			uint			m_stride;
			/// Offset of field in input data
			uint			m_offset;
		};
		static_assert(std::is_pod<VertexLayout>::value == true, "VertexLayout must be a POD.");
		///////////////////////////////////////////////////////////////
		
		struct VertexHandleTag {};
		typedef GenericHandle<12, 20, VertexHandleTag> VertexBufferHandle;
		struct IndexHandleTag {};
		typedef GenericHandle<12, 20, IndexHandleTag> IndexBufferHandle;
		struct VerLayoutHandleTag {};
		typedef GenericHandle<12, 20, VerLayoutHandleTag> VertexLayoutHandle;

		struct RenderLayaerTag{};
		typedef GenericHandle<12, 20, RenderLayaerTag> RenderLayerHandle;
	} // Render

} // SDK

#endif