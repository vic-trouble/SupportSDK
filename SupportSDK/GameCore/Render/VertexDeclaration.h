#ifndef __GAMECORE_VERTEXDECLARATION_H__
#define __GAMECORE_VERTEXDECLARATION_H__

#include "GameCore/Render/RenderTypes.h"

namespace SDK
{
	
	namespace Render
	{

		template <typename DeclType>
		struct DeclHelper
		{
			static uint ElementsSize;
			static VertexElement Elements[];
		};

		struct VertexDeclaration
		{
			VertexBufferHandle		vertices;
			VertexElement			temp_element;
			const VertexElement*	elements;
			uint					decl_size;

			template <typename T>
			static inline uint ElementsSize()
			{
				return DeclHelper<T>::GetElementsCount();
			}

			template <typename T>
			static inline VertexElement* Elements()
			{
				return DeclHelper<T>::Elements;
			}
		};
		//static_assert(std::is_pod<VertexDeclaration>::value == true, "Draw must be a POD.");
		struct Batch
		{
			VertexBufferHandle		vertices;
			ElementBufferHandle		element;
			IndexBufferHandle		indices;
		};


		//////////////////////////////////////////////////////////
		// Common declarations

		struct PosStaticBuffer
		{
			VertexBufferHandle handle;
		};


		template <>
		uint DeclHelper<PosStaticBuffer>::ElementsSize = 1;

		template <>
		VertexElement DeclHelper<PosStaticBuffer>::Elements[] =
		{
			{ 3, VertexSemantic::Position, ComponentType::Float, false, PrimitiveType::Points, sizeof(PosStaticBuffer), offsetof(PosStaticBuffer, handle) }
		};		

	} // Render

} // SDK

#endif