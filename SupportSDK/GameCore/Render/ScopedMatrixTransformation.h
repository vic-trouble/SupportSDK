#ifndef	__GAMECORE_SCOPEDMATRIXTRANSFORMATION_H__
#define __GAMECORE_SCOPEDMATRIXTRANSFORMATION_H__

#include "IRenderer.h"
#include <Utilities/CustomScopedObject.h>

namespace SDK
{
	namespace Render
	{
		struct ScopedMatrix {};

		template <>
		struct ScopedHelper <ScopedMatrix, IRenderer*>
		{
			static bool Push(IRenderer* ip_renderer)
			{
				ip_renderer->PushMatrix();
				return true;
			}

			static void Pop(bool, IRenderer* ip_renderer)
			{
				ip_renderer->PopMatrix();
			}
		};

		using ScopedMatrixTransformation = CustomScopedObject<ScopedMatrix, IRenderer*>;

	} // Render
} // SDK

#endif
