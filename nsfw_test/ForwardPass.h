#pragma once


#include <render.h>

struct Camera;
struct GameObject;


class ForwardPass : public nsfw::RenderPass
{
public:
	void prep();

	void draw(const Camera &c, const GameObject &go);

	void post();

};
