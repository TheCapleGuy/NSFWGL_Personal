#pragma once

#include <render.h>

struct ParticleEmitter;
struct Camera;

class ParticlePass : public nsfw::RenderPass
{
public:
	//nsfw::Asset<nsfw::ASSET::SHADER>  shaderUpdate = "ParticleUpdate";
	//nsfw::Asset<nsfw::ASSET::SHADER>  shaderDraw   = "ParticleDraw";
	void prep();

	void onPrep();

	void draw(const Camera &c, nsfw::ParticleEmitter *pEmit);

	void post();

};