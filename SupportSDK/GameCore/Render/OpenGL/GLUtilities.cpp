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
	}
}