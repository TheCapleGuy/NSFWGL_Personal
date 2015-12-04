#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "ForwardPass.h"
#include "ShadowPass.h"
#include "ParticlePass.h"
#include "Lights.h"
#include <nsfw.h>

class MyTestApp : public nsfw::Application
{
	Camera camera;
	GameObject obj;
	GameObject planes[4];
	nsfw::ParticleEmitter *parEmit;
	DirectionalLight dLight;
	ForwardPass fp;
	ShadowPass sp;
	ParticlePass pp;

public:
	void onInit();
	void onPlay();
	void onStep();
	void onTerm();
};