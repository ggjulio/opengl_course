#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <unistd.h>
#include <math.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

std::string vertexShader = 
	"#version 330 core\n"
	"\n"
	"layout(location = 0) in vec4 position;\n"
	"\n"
	"void main()\n"
	"{\n"
	"  gl_Position = position;\n"
	"}\n"
;

std::string fragmentShader = 
	"#version 330 core\n"
	"\n"
	"layout(location = 0) out vec4 color;\n"
	"\n"
	"uniform vec4 u_color;\n"
	"\n"
	"void main()\n"
	"{\n"
	"  color = u_color;\n"
	"}\n"
;

unsigned int compileShader(GLuint type, const std::string &source)
{
	GLuint id = glCreateShader(type);
	const char * src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char *)alloca(length);
		glGetShaderInfoLog(id, length, &length, message);
		std::cerr << "failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")  << " shader !" << std::endl
			<< message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

static GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR: glewInit()" << std::endl;

	std::cout << "gl version: " << glGetString(GL_VERSION) << std::endl;

	GLfloat triangleVertices[] =
	{
		-0.5,  0.5, 0,
		 0.5,  0.5, 0,
		-0.5, -0.5, 0,
		0.5,  -0.5, 0,
	};

	GLuint indices[]{
		0,1,2,
		2,3,1
	};

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	VertexBuffer vb(triangleVertices, 2 * 6 * sizeof(GLfloat));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (const void *)0));

	IndexBuffer ib(indices, 6);

	GLuint program = createShader(vertexShader, fragmentShader);
	GLCall(glUseProgram(program));

	int location = glGetUniformLocation(program, "u_color");
	if (location == -1)
		std::cout << __FILE__ <<" "<< __LINE__ << ": uniform location not found"<< std::endl;

	// unbind everything just to test...
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	float red = 1.0f;
	float inc = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		GLCall(glUseProgram(program));
		GLCall(glBindVertexArray(vao));
		ib.Bind();

		GLCall(glUniform4f(location, red, 0.3f, 0.0f, 1.0f));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		if (red >= 1.0f)
			inc = -0.05;
		else if (red <= .0f)
			inc = 0.05f;
		red += inc;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	GLCall(glDeleteProgram(program));
	glfwTerminate();
	return 0;
}
