#include <ogl\gl_core_4_4.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include"MyTestApp.h"
#include "ParticleEmitter.h"

void ParticlePass::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
	//glUseProgram(*shader);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, 800, 600); // Use window dimensions (w, h) 
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ParticlePass::onPrep()
{

}

void ParticlePass::draw(const Camera & c, nsfw::ParticleEmitter *pEmit)
{
	shader = "ParticleUpdate";
	glUseProgram(*shader);

	float dTime = nsfw::Window::instance().getTime();
	setUniform("time", nsfw::UNIFORM::FLO1, &pEmit->time);
	setUniform("deltaTime", nsfw::UNIFORM::FLO1, &dTime);
	setUniform("lifeMin", nsfw::UNIFORM::FLO1, &pEmit->mLifeSpanMin);
	setUniform("lifeMax", nsfw::UNIFORM::FLO1, &pEmit->mLifeSpanMax);
	setUniform("sizeStart", nsfw::UNIFORM::FLO1, &pEmit->mStartSize);
	setUniform("sizeEnd", nsfw::UNIFORM::FLO1, &pEmit->mEndSize);
	setUniform("colorStart", nsfw::UNIFORM::FLO3, &pEmit->mStartColor);
	setUniform("colorEnd", nsfw::UNIFORM::FLO3, &pEmit->mEndColor);
	setUniform("emitterPosition", nsfw::UNIFORM::FLO3, &pEmit->mPosition);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindVertexArray(*pEmit->vao[pEmit->mActiveBuffer]);

	unsigned otherBuffer = (pEmit->mActiveBuffer + 1) % 2;

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, *pEmit->vbo[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, pEmit->mMaxParticles);

	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	shader = "ParticleDraw";
	glUseProgram(*shader);

	setUniform("projectionView",nsfw::UNIFORM::MAT4, glm::value_ptr(c.getView()));
	setUniform("cameraTransform", nsfw::UNIFORM::MAT4, glm::value_ptr(c.m_transform));
	
	glBindVertexArray(*pEmit->vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, pEmit->mMaxParticles);

	pEmit->mActiveBuffer = otherBuffer;
}

void ParticlePass::post()
{
	glDisable(GL_CULL_FACE);			  glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); glUseProgram(0);
}
