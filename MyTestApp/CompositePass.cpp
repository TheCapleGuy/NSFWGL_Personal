#include <ogl/gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "CompositePass.h"

void CompositePass::prep()
{
	glUseProgram(*shader);
	glViewport(0, 0, 800, 600);
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void CompositePass::draw()
{

}

void CompositePass::post()
{
	glUseProgram(0);
}