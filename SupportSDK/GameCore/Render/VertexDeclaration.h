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
			static VertexLayout Elements[];
		};

		struct VertexDeclaration
		{
			VertexBufferHandle		vertices;
			VertexLayout			temp_element;
			const VertexLayout*	elements;
			uint					decl_size;

			template <typename T>
			static inline uint ElementsSize()
			{
				return DeclHelper<T>::GetElementsCount();
			}

			template <typename T>
			static inline VertexLayout* Elements()
			{
				return DeclHelper<T>::Elements;
			}
		};
		//static_assert(std::is_pod<VertexDeclaration>::value == true, "Draw must be a POD.");
		struct Batch
		{
			constexpr static int MAX_STREAMS_COUNT = 5;
			VertexBufferHandle		vertices;		
			VertexLayoutHandle		element;
			IndexBufferHandle		indices;
		};

	} // Render

} // SDK

#endif