#include <ogl/gl_core_4_4.h>
#include <glm\glm.hpp>	

#include "ForwardPass.h"

#include "GameObject.h"
#include "Camera.h"

void ForwardPass::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
	glUseProgram(*shader);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, 800, 600);
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

};


void ForwardPass::draw(const Camera &c, const GameObject &go)
{
	setUniform("Projection", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getProjection()));
	setUniform("View", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getView()));

	setUniform("Model", nsfw::UNIFORM::MAT4, glm::value_ptr(go.transform));
	setUniform("Diffuse", nsfw::UNIFORM::TEX2, &(go.diffuse));

	glBindVertexArray(*go.mesh);
	glDrawElements(GL_TRIANGLES, *go.tris, GL_UNSIGNED_INT, 0);
}

void ForwardPass::post() 
{ 
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE); 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 0, 0);
	glUseProgram(0);
};