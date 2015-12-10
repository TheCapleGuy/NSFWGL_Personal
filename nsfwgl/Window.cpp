#include <ogl\gl_core_4_4.h>
#include <glfw\glfw3.h>
#include "nsfw.h"
void APIENTRY oglErrorDefaultCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	// if 'GL_DEBUG_OUTPUT_SYNCHRONOUS' is enabled, you can place a
	// breakpoint here and the callstack should reflect the problem location!
	std::cerr << message << std::endl;
}

void nsfw::Window::init(unsigned width, unsigned height)
{
	this->height = height;
	this->width = width;
	bool success = glfwInit();
	if (!success)
	{
		std::cout << "error inititalizing GLFW\n"; 
		return;
	}
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	window = glfwCreateWindow(width, height, "NSFWGL", nullptr, nullptr);

	if(window == nullptr)
	{
		std::cout << "Error creating window\n";
		return;
	}

	glfwMakeContextCurrent(window);

	
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		std::cout << "Error loading ogl\n";
		delete window;
		return;
	}
#ifdef _DEBUG
	if (glDebugMessageCallback)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(oglErrorDefaultCallback, nullptr);

		GLuint unusedIDs = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIDs, true);
	}
	else
	{
		assert(false && "Failed to subscribe to glDebugMessageCallback.");
	}
#endif
}

void nsfw::Window::step()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void nsfw::Window::term()
{
	glfwDestroyWindow(window);
	glfwTerminate();
	//TODO();
}

float nsfw::Window::getTime() const
{
	return glfwGetTime();
}

bool nsfw::Window::getKey(unsigned k) const
{
	return glfwGetKey(window, k);
}

bool nsfw::Window::getShouldClose() const
{
	return glfwWindowShouldClose(window) == true || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

unsigned nsfw::Window::getWidth() const
{
	return width;
}

unsigned nsfw::Window::getHeight() const
{
	return height;
}

glm::mat4 nsfw::Window::getTexelAdjustmentMatrix() const
{
	TODO_D("Implemented, not tested.");

	glm::vec3 texelSize = 1.0f/glm::vec3(width,height,0);
	glm::vec3 halfTexel = texelSize * 0.5f; // bottom left

	// scale up to the appropriate size, then shift it to line up w/center of texels
	return glm::translate(halfTexel) * glm::scale(glm::vec3(1, 1, 1) - texelSize);	
}