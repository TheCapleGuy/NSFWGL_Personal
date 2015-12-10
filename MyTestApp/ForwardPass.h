#pragma once
#include <render.h>
struct Camera;
struct GameObject;
struct DirectionalLight;

class ForwardPass : public nsfw::RenderPass
{
public:
	void prep();

	void onPrep(const DirectionalLight &dLight);

	void draw(const Camera &c, const GameObject &go);

	void draw(const Camera &c, nsfw::ParticleEmitter& emitter);

	void post();

};