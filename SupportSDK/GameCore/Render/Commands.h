#ifndef	__GAMECORE_COMMANDS_H__
#define __GAMECORE_COMMANDS_H__

#include "Render/RenderTypes.h"

namespace SDK
{
	namespace Render
	{
		namespace Commands
		{

			struct Draw
			{
				static CommandExecutor EXECUTOR_FUNCTION;

				uint vertexCount;
				uint startVertex;

				VertexBufferHandle		vertices;
				VertexLayoutHandle		layout;
				IndexBufferHandle		indices;
			};
			static_assert(std::is_pod<Draw>::value == true, "Draw must be a POD.");

		} // Commands
	} // Render
} // SDK

#endif
