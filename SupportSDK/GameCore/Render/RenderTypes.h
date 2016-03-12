#ifndef	__GAMECORE_RENDERTYPES_H__
#define __GAMECORE_RENDERTYPES_H__

#include "../GameCoreAPI.h"

#include "GameCore/GenericHandle.h"
#include "Light.h"

namespace SDK
{
	
	namespace Render
	{

		enum class ProjectionType
		{
			Orthographic,
			Perspective
		};

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
			Dynamic = 0,
			Position,
			Color,
			BlendWeights,
			BlendIndices,
			Normal,
			Diffuse,
			Specular,
			Ambient,
			TextureCoordinates,
			Binomial,
			Tangent,

			Count
		};

		enum class ShaderVariableType : char
		{
			Float,
			FloatVec2,
			FloatVec3,
			FloatVec4,
			Double,
			DoubleVec2,
			DoubleVec3,
			DoubleVec4,
			Int,
			IntVec2,
			IntVec3,
			IntVec4,
			UInt,
			UIntVec2,
			UIntVec3,
			UIntVec4,
			Bool,
			FloatMat2,
			FloatMat3,
			FloatMat4,
			FloatMat2x3,
			FloatMat2x4,
			FloatMat3x2,
			FloatMat3x4,
			FloatMat4x2,
			FloatMat4x3,
			Sampler2D,

			Undefined
		};

		enum class UniformType
		{
			ProjectionMatrix,
			ModelviewMatrix,
			Dynamic
		};		

		struct VertexLayout
		{
			/// The source vertex buffer, as bound to an index using VertexBufferBinding
			InternalHandle	m_source;
			// 1, 2, 3 or 4
			uint			m_vertex_size;
			/// The meaning of the element
			VertexSemantic	m_semantic;
			/// The type of element
			ComponentType	m_component;
			bool			m_normalized;
			uint			m_stride;
			/// Offset of field in input data
			uint			m_offset;

			//VertexLayout(uint i_ver_size, )
		};
		static_assert(std::is_pod<VertexLayout>::value == true, "VertexLayout must be a POD.");
		///////////////////////////////////////////////////////////////
		
		struct VertexHandleTag {};
		typedef GenericHandle<int, 12, 20, VertexHandleTag> VertexBufferHandle;
		struct IndexHandleTag {};
		typedef GenericHandle<int, 12, 20, IndexHandleTag> IndexBufferHandle;
		struct VerLayoutHandleTag {};
		typedef GenericHandle<int, 12, 20, VerLayoutHandleTag> VertexLayoutHandle;
		// Meshes
		struct MeshHandlerTag{};
		typedef GenericHandle<int, 12, 20, MeshHandlerTag> MeshHandler;
		struct MeshComponentHandlerTag {};
		typedef GenericHandle<int, 12, 20, MeshComponentHandlerTag> MeshComponentHandler;
		// Shaders
		struct ShaderHandlerTag {};
		typedef GenericHandle<int, 12, 20, ShaderHandlerTag> ShaderHandler;
		// Lights
		using LightHandle = int;
		constexpr LightHandle INVALID_LIGHT_HANDLE = -1;
		// Material
		struct MaterialTag {};
		typedef GenericHandle<int, 12, 20, MaterialTag> MaterialHandle;
		// Commands
		typedef void(*CommandExecutor)(const void*);

#define IMPLEMENT_COMMAND_IMPL(ProcessorFunc, CompletionFunc) \
		static constexpr CommandExecutor EXECUTOR_FUNCTION = &ProcessorFunc; \
		static constexpr CommandExecutor COMPLETION_FUNCTION = &CompletionFunc;		

#define IMPLEMENT_COMMAND_WITH_COMPLETION_EXPORT(ProcessorFunc, CompletionFunc) \
		GAMECORE_EXPORT static void ProcessorFunc(const void*); \
		GAMECORE_EXPORT static void CompletionFunc(const void*); \
		GAMECORE_EXPORT void SetDefaultValues(); \
		IMPLEMENT_COMMAND_IMPL(ProcessorFunc, CompletionFunc)

#define IMPLEMENT_COMMAND_EXPORT(ProcessorFunc) \
		GAMECORE_EXPORT static void ProcessorFunc(const void*); \
		static void CompletionStub(const void*){} \
		GAMECORE_EXPORT void SetDefaultValues(); \
		IMPLEMENT_COMMAND_IMPL(ProcessorFunc, CompletionStub)

		// For use in library|exe only
#define IMPLEMENT_COMMAND_WITH_COMPLETION(ProcessorFunc, CompletionFunc) \
		static void ProcessorFunc(const void*); \
		static void CompletionFunc(const void*); \
		void SetDefaultValues(); \
		IMPLEMENT_COMMAND_IMPL(ProcessorFunc, CompletionFunc)

#define IMPLEMENT_COMMAND(ProcessorFunc) \
		static void ProcessorFunc(const void*); \
		static void CompletionStub(const void*){} \
		void SetDefaultValues(); \
		IMPLEMENT_COMMAND_IMPL(ProcessorFunc, CompletionStub)

	} // Render

} // SDK

#endif