#include "stdafx.h"

#include "GlUitlities.h"

#include <sstream>

#include <GL/glew.h>

void CheckGlErrors(const char *filename, int line) {
	std::stringstream ss;
	bool has_error = false;

	for (GLenum error = glGetError(); error != GL_NO_ERROR; error = glGetError()) {
		ss << "GL error 0x" << std::hex << error << std::endl;
		has_error = true;
		assert(false);
	}
}

void makeGlMatrix(float gl_matrix[16], const SDK::Matrix4f& m)
{
	size_t x = 0;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			gl_matrix[x] = m[j][i];
			x++;
		}
	}
}