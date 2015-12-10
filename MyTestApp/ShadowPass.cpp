#include <ogl\gl_core_4_4.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Lights.h"
#include "ShadowPass.h"
#include "GameObject.h"

void ShadowPass::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
	glUseProgram(*shader);

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, 1024, 1024); // Use window dimensions (w, h) 
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_DEPTH_BUFFER_BIT);
}

	//lights


void ShadowPass::draw(const DirectionalLight &dLight, const GameObject &go)
{
	//setUniform("dLColor", nsfw::UNIFORM::FLO3, &(dLight.color));
	//setUniform("dLDirection", nsfw::UNIFORM::FLO3, &(dLight.direction));
	setUniform("LightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(dLight.getMatrix()));

	//Camera
	//setUniform("Projection", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getProjection()));
	//setUniform("View", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getView()));
	//GameObject
	setUniform("Model", nsfw::UNIFORM::MAT4, glm::value_ptr(go.transform));
	//setUniform("Diffuse", nsfw::UNIFORM::TEX2, &(go.diffuse), 0);
	//setUniform("Specular", nsfw::UNIFORM::TEX2, &(go.diffuse), 1);
	//setUniform("Normal", nsfw::UNIFORM::TEX2, &(go.diffuse), 2);
	//setUniform("ShadowMap", nsfw::UNIFORM::TEX2, &(go.diffuse), 3);

	glBindVertexArray(*go.mesh);
	glDrawElements(GL_TRIANGLES, *go.tris, GL_UNSIGNED_INT, 0);
}

void ShadowPass::post()
{
	glDisable(GL_CULL_FACE);			  
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	glUseProgram(0);
}