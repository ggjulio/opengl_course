#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function, const char *file, int line)
{
	GLenum error;
	bool hasError = false;
	
	while((error = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "[openGL Error "<< error <<"] " << file << ":" << line << " --> " << function << std::endl;
		hasError = true;
	}
	return !hasError;
}
