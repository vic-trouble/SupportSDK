#ifndef	__GAMECORE_GLUTILITIES_H__
#define __GAMECORE_GLUTILITIES_H__
#include "../../GameCoreAPI.h"
#include <GL/glew.h>
// Макрос проверки OpenGL на ошибки
#ifdef _DEBUG
#	define CHECK_GL_ERRORS CheckGlErrors(__FILE__, __LINE__)
#else
#	define CHECK_GL_ERRORS
#endif

GAMECORE_EXPORT void CheckGlErrors(const char *filename, int line);

void makeGlMatrix(GLfloat gl_matrix[16], const SDK::Matrix4f& m);

__forceinline GLboolean Convert(bool i_value)
{
	return i_value ? GL_TRUE : GL_FALSE;
}

#endif