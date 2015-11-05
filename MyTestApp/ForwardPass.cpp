#include <ogl\gl_core_4_4.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include"MyTestApp.h"


void ForwardPass::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
	glUseProgram(*shader);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, 800, 600); // Use window dimensions (w, h) 
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ForwardPass::onPrep(const DirectionalLight &dLight)
{
	glm::mat4 textureSpaceOffset(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
		);
	//lights
	setUniform("dLColor", nsfw::UNIFORM::FLO3, &(dLight.color));
	setUniform("dLDirection", nsfw::UNIFORM::FLO3, &(dLight.direction));
	setUniform("lightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(textureSpaceOffset * dLight.getMatrix()));
}

void ForwardPass::draw(const Camera &c, const GameObject &go)
{
	//Camera
	setUniform("Projection", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getProjection()));
	setUniform("View", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getView()));
	//GameObject
	setUniform("Model", nsfw::UNIFORM::MAT4, glm::value_ptr(go.transform));
	setUniform("Diffuse", nsfw::UNIFORM::TEX2, &(go.diffuse), 0);
	//setUniform("Specular", nsfw::UNIFORM::TEX2, &(go.diffuse), 1);
	//setUniform("Normal", nsfw::UNIFORM::TEX2, &(go.diffuse), 2);

	nsfw::Asset<nsfw::ASSET::TEXTURE> shadowmap = "FINAL";

	setUniform("ShadowMap", nsfw::UNIFORM::TEX2, &shadowmap, 1);

	glBindVertexArray(*go.mesh);
	glDrawElements(GL_TRIANGLES, *go.tris, GL_UNSIGNED_INT, 0);
}

void ForwardPass::post()
{
	glDisable(GL_CULL_FACE);			  glDisable(GL_DEPTH_TEST); 
	glBindFramebuffer(GL_FRAMEBUFFER, 0); glUseProgram(0);
}