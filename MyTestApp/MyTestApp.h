#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "ForwardPass.h"
//#include "CompositePass.h"
#include <nsfw.h>

class MyTestApp : public nsfw::Application
{
	Camera camera;
	GameObject obj;
	GameObject plane;

	ForwardPass fp;

public:
	void onInit();
	void onPlay();
	void onStep();
	void onTerm();
};