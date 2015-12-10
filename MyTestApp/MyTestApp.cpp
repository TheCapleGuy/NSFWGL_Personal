#include "MyTestApp.h"

void MyTestApp::onInit()
{
	auto &a = nsfw::Assets::instance();
	a.loadShader("Forward", "../resources/shaders/basic.vert",
							"../resources/shaders/basic.frag");

	a.loadShader("Shadow", "../resources/shaders/shadow.vert", 
						   "../resources/shaders/shadow.frag");

	a.loadShader("ParticleDraw", "../resources/shaders/gpuParticle.vert", 
								 "../resources/shaders/gpuParticle.geom",
								 "../resources/shaders/gpuParticle.frag");

	a.loadShader("ParticleUpdate", "../resources/shaders/gpuParticleUpdate.vert");

	a.loadFBX("", "../resources/fbx/soulspear/soulspear.fbx");

	a.loadTexture("RockDiffuse", "../resources/textures/rock_diffuse.tga");
	a.loadTexture("RockNormal", "../resources/textures/rock_normal.tga");
	a.loadTexture("RockSpecular", "../resources/textures/rock_specular.tga");

	const char *renderTargetNames[] = { "FINAL" };
	unsigned renderTargetDepths[] = { GL_DEPTH_COMPONENT };

	a.makeFBO("Shadow", 1024, 1024, 1, renderTargetNames, renderTargetDepths);

	parEmit = new nsfw::ParticleEmitter();
	parEmit->Init(100000, .1f, 5.0f, 5.f, 20.f, 1.f, .1f, glm::vec4(1,0,0,1), glm::vec4(1,1,0,1));

	a.makeVAO("ParticleVAO", parEmit);

	//light values on initialization
	dLight.color     = glm::vec3(.8f, .6f, .4f);
	dLight.direction = glm::normalize(glm::vec3(0.f, 5.5f, 5.7f)); 
}

void MyTestApp::onPlay()
{
	camera.lookAt(glm::vec3(0, 2.5, 3.0), //offest top right and forward
				  glm::vec3(0, 0, 0),  // origin
				  glm::vec3(0, 1, 0)); // up

	obj.transform = glm::mat4(1);
	obj.diffuse   = "soulspear_diffuse.tga"; //Default built-in
	obj.mesh      = "SoulSpear_Low:SoulSpear_Low1";    //Default built-in
	obj.tris = "SoulSpear_Low:SoulSpear_Low1";    //Default built-in

	for (int i = 0; i < 4; i++)
	{
		float x, y;
		if (i == 0)
			x = -1, y = 1;
		if (i == 1)
			x = 1, y = 1;
		if (i == 2)
			x = -1, y = -1;
		if (i == 3)
			x = 1, y = -1;
		//setting plane values to set it on "floor"
		planes[i].transform = glm::mat4(1);
		planes[i].transform = glm::rotate(-90.f, glm::vec3(1, 0, 0)) * // rotating planes to parallel "floor"
							  glm::scale(glm::vec3(4, 4, 1)) *         // scaling to better size
							  glm::translate(x , y, 0.f);			   //translate to coresponding location	
		
		planes[i].diffuse = "RockDiffuse";
		planes[i].normal = "RockNormal";
		planes[i].Specular = "RockSpecular";
		planes[i].mesh = "Quad";
		planes[i].tris = "Quad";
	}


	fp.shader = "Forward";
	fp.fbo    = "Screen";

	sp.shader = "Shadow";
	sp.fbo    = "Shadow";

	//pp.shader = "Particle";
	pp.fbo    = "ParticleVAO";
	//fp.fbo    = "Screen"; //Default built-in

	//cp.shader = "Basic";
}

void MyTestApp::onStep()
{
	camera.update();

	//moving light for effect with shadow;
	float time = nsfw::Window::instance().getTime();
	float wave = .5f * sin(time);
	//std::cout << wave << std::endl;
	dLight.direction.x = wave;

	sp.prep();
	sp.draw(dLight, obj);
	for (int i = 0; i < 4; i++)
	{
		sp.draw(dLight, planes[i]);
	}

	sp.post();

	fp.prep();
	fp.onPrep(dLight);
	fp.draw(camera, obj);
	for (int i = 0; i < 4; i++)
	{
		fp.draw(camera, planes[i]);
	}


	//pp.prep();
	pp.draw(camera, parEmit);
	//pp.post();
	fp.post();


	//cp.prep();
	//cp.draw();
	//cp.post();
}

void MyTestApp::onTerm()
{
	delete parEmit;
}
