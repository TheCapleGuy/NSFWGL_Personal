#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() : 
	mParticles(nullptr), mMaxParticles(0),
	mPosition(0), mIBO(0)
{
	mVAO[0] = 0;
	mVAO[1] = 0;
	mVBO[0] = 0;
	mVBO[1] = 0;
}

ParticleEmitter::~ParticleEmitter() 
{
	delete[] mParticles;

	glDeleteVertexArrays(2, mVAO);
	glDeleteBuffers(2, mVBO); 
	glDeleteBuffers(1, &mIBO);
}

void ParticleEmitter::Init(
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

	glGenVertexArrays(2, mVAO);
	glGenBuffers(2, mVBO);

	//    first buffer
	glBindVertexArray(mVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(Particle), mParticles, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);               //position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 12); //velocity
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 24); //lifetime
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 28); //lifespan
	//    first buffer

	//    second buffer
	glBindVertexArray(mVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(Particle), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);               //position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 12); //velocity
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 24); //lifetime
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 28); //lifespan
	//   second buffer

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleEmitter::Particle::update()
{

}

void ParticleEmitter::emit()
{

}

void ParticleEmitter::update()
{
	emit();
}

void ParticleEmitter::draw()
{

}


