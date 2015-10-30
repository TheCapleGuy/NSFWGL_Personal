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
	setUniform("Projection", nsfw::UNIFORM::MAT4, glm::value_ptr(glm::mat4(1)));
	setUniform("View", nsfw::UNIFORM::MAT4, glm::value_ptr(glm::mat4(1)));

	setUniform("Model", nsfw::UNIFORM::MAT4, glm::value_ptr(glm::mat4(1)));

	nsfw::Asset<nsfw::ASSET::TEXTURE> fin = "FINAL";
	nsfw::Asset<nsfw::ASSET::VAO> qm = "Quad";
	nsfw::Asset<nsfw::ASSET::SIZE> qt = "Quad";
	setUniform("Diffuse", nsfw::UNIFORM::TEX2, &fin);

	glBindVertexArray(*qm);
	glDrawElements(GL_TRIANGLES, *qt, GL_UNSIGNED_INT, 0);
}

void CompositePass::post()
{
	glUseProgram(0);
}
