//#include "ParticleEmitter.h"
#include "nsfw.h"

nsfw::ParticleEmitter::ParticleEmitter() :
	mParticles(nullptr), mMaxParticles(0),
	mPosition(0), time(0)
{

}

nsfw::ParticleEmitter::~ParticleEmitter()
{
	delete[] mParticles;
	delete[] mVertexData;
}

void nsfw::ParticleEmitter::Init(
	unsigned aMaxParticles, 
	float aLifeSpanMin, float aLifeSpanMax, 
	float aVelocityMin, float aVelocityMax, 
	float aStartSize, float aEndSize, 
	glm::vec4 const & aStartColor, glm::vec4 const & aEndColor)
{
	mMaxParticles = aMaxParticles;
	mLifeSpanMin  = aLifeSpanMin;
	mLifeSpanMax  = aLifeSpanMax;
	mVelocityMin  = aVelocityMin;
	mVelocityMax  = aVelocityMax;
	mStartSize    = aStartSize;
	mEndSize      = aEndSize;
	mStartColor   = aStartColor;
	mEndColor     = aEndColor;

	mParticles = new Particle[mMaxParticles];
}

void nsfw::ParticleEmitter::Particle::update()
{

}

void nsfw::ParticleEmitter::emit()
{

}

void nsfw::ParticleEmitter::update()
{
	float dTime = nsfw::Window::instance().getTime();
	time += dTime;
	emit();
}

void nsfw::ParticleEmitter::draw()
{

}


