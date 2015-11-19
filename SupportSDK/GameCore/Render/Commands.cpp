#include "stdafx.h"

#include "Commands.h"

#include "Core.h"
#include "IRenderer.h"

namespace SDK
{
	namespace Render
	{
		namespace Commands
		{
			///////////////////////////////////////////////////////////////////////////
			// Declarations
			void DrawFunction(const void*);
			CommandExecutor Draw::EXECUTOR_FUNCTION = &DrawFunction;


			///////////////////////////////////////////////////////////////////////////
			// Implementations

			void DrawFunction(const void* ip_data)
			{
				auto p_renderer = Core::GetRenderer();
				const Draw* cmd = reinterpret_cast<const Draw*>(ip_data);

				Batch b;
				b.vertices = cmd->vertices;
				b.element = cmd->layout;
				b.indices = cmd->indices;

				p_renderer->Draw(b);
			}
		} // Commands
	} // Render
} // SDK