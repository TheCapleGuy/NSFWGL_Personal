#include "MyTestApp.h"

void MyTestApp::onInit()
{
	auto &a = nsfw::Assets::instance();
	a.loadShader("Forward", "../resources/shaders/basic.vert",
							"../resources/shaders/basic.frag");

	a.loadShader("Shadow", "../resources/shaders/shadow.vert", "../resources/shaders/shadow.vert");

	a.loadFBX("", "../resources/fbx/soulspear/soulspear.fbx");
	a.loadTexture("RockDiffuse", "../resources/textures/rock_diffuse.tga");

	const char *renderTargetNames[] = { "FINAL" };
	unsigned renderTargetDepths[] = { GL_DEPTH_COMPONENT };

	a.makeFBO("Shadow", 1024, 1024, 1, renderTargetNames, renderTargetDepths);

	//setting plane values to set it on "floor"
	plane.transform = glm::mat4(1);
	plane.transform = glm::scale(10, 10, 10);
	plane.transform = glm::rotate(-90.f, glm::vec3(1, 0, 0));


	//light values on initialization
	dLight.color     = glm::vec3(.8f, .6f, .4f);
	dLight.direction = glm::vec3(15.f, 25.f, 0.f); 
}

void MyTestApp::onPlay()
{
	camera.lookAt(glm::vec3(2, 2, 2), //offest top right and forward
		glm::vec3(0, 0, 0),  // origin
		glm::vec3(0, 1, 0)); // up

	obj.transform = glm::mat4(1);
	obj.diffuse   = "soulspear_diffuse.tga"; //Default built-in
	obj.mesh      = "SoulSpear_Low:SoulSpear_Low1";    //Default built-in
	obj.tris      = "SoulSpear_Low:SoulSpear_Low1";    //Default built-in

	
	//plane.transform = glm::rotate(90.f, glm::vec3(0, 1, 0));
	plane.diffuse = "RockDiffuse";
	plane.mesh    = "Quad";
	plane.tris    = "Quad";

	fp.shader     = "Forward";
	fp.fbo        = "Screen";

	sp.shader = "Shadow";
	sp.fbo = "Shadow";
	//fp.fbo    = "Screen"; //Default built-in

	//cp.shader = "Basic";
}

void MyTestApp::onStep()
{
	//moving camera for effect with shadow;
	float time = nsfw::Window::instance().getTime();
	dLight.direction.x = 30 * sin(time * 2.f) + 10;
	
	sp.prep();
	sp.draw(dLight, obj);
	sp.draw(dLight, plane);
	sp.post();

	fp.prep();
	fp.onPrep(dLight);
	fp.draw(camera, obj);
	fp.draw(camera, plane);
	fp.post();

	//cp.prep();
	//cp.draw();
	//cp.post();
}

void MyTestApp::onTerm()
{

}
