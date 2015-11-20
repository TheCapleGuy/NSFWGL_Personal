#pragma once
#include "nsfw.h"


class ParticleEmitter
{
	ParticleEmitter();
	~ParticleEmitter();
	struct Particle
	{
		glm::vec3 position, velocity;
		float lifeTime, lifeSpan;
		Particle() : lifeTime(1), lifeSpan(0) {};
		void update();
	};
public:

	Particle* mParticles; 
	unsigned mMaxParticles, mVAO[2], mVBO[2], mIBO;

	glm::vec3 mPosition;
	glm::vec4 mStartColor, mEndColor;

	float mLifeSpanMin, mLifeSpanMax;
	float mVelocityMin, mVelocityMax;
	float mStartSize, mEndSize;

	unsigned mActiveBuffer;

	void Init(
		unsigned aMaxParticles,
		float aLifeSpanMin, float aLifeSpanMax,
		float aVelocityMin, float aVelocityMax,
		float aStartSize,   float aEndSize,
		glm::vec4 const& aStartColor, glm::vec4 const& aEndColor);

	void emit();
	void update();
	void draw();
};