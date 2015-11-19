#pragma once
#include "nsfw.h"


class ParticleEmitter
{
	struct Particle
	{
		glm::vec3 position, velocity;
		float lifeTime, lifeSpan;
		Particle() : lifeTime(1), lifeSpan(0) {};
		void update();
	};

public:
	void emite();
	void update();
};