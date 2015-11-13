#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "ForwardPass.h"
#include "ShadowPass.h"
#include "Lights.h"
#include <nsfw.h>

class MyTestApp : public nsfw::Application
{
	Camera camera;
	GameObject obj;
	GameObject planes[4];
	DirectionalLight dLight;
	ForwardPass fp;
	ShadowPass sp;

public:
	void onInit();
	void onPlay();
	void onStep();
	void onTerm();
};