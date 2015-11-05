#pragma once
#include <render.h>
struct Camera;
struct GameObject;
struct DirectionalLight;

class ShadowPass : public nsfw::RenderPass
{
public:
	void prep();

	void draw(const DirectionalLight &dLight, const GameObject &go);

	void post();

};