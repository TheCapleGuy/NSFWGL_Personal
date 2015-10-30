#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "ForwardPass.h"
#include "CompositePass.h"
#include <nsfw.h>


class TestApp : public nsfw::Application
{
	Camera	    camera;
	GameObject  obj;
	ForwardPass fp;
	CompositePass cp;
public:
	void onInit();
	void onPlay();
	void onStep();
	void onTerm();
};
