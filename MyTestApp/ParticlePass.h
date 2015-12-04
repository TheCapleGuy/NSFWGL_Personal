#pragma once

#include <render.h>

struct ParticleEmitter;
struct Camera;

class ParticlePass : public nsfw::RenderPass
{
public:
	void prep();

	void onPrep(const nsfw::ParticleEmitter *pEmit);

	void draw(const Camera &c);

	void post();

};