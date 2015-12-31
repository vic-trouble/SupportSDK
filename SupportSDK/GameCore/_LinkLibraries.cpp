#include "stdafx.h"

#if defined(TARGET_PLATFORM_WIN32)
#include <Math/_Link.h>
#include <Utilities/_Link.h>
#include <Patterns/_Link.h>

#pragma comment(lib, "opengl32.lib")
#if defined(_DEBUG)
# pragma comment(lib, "glew32d.lib")
#else
# pragma comment(lib, "glew32.lib")
#endif
#endif