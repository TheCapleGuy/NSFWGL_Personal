#include "MyTestApp.h"

void MyTestApp::onInit()
{
	auto &a = nsfw::Assets::instance();
	a.loadShader("Forward", "../resources/shaders/basic.vert",
							"../resources/shaders/basic.frag");

	a.loadFBX("", "../resources/fbx/soulspear/soulspear.fbx");
	a.loadTexture("RockDiffuse", "../resources/textures/rock_diffuse.tga");

	const char *renderTargetNames[] = { "FINAL" };
	unsigned renderTargetDepths[] = { GL_RGBA };
	//A.makeFBO("FINAL", 800, 600, 1, renderTargetNames, renderTargetDepths);
}

void MyTestApp::onPlay()
{
	camera.lookAt(glm::vec3(2, 2, 2), //offest top right and forward
		glm::vec3(0, 0, 0),  // origin
		glm::vec3(0, 1, 0)); // up

	obj.transform = glm::mat4(1);
	obj.diffuse = "soulspear_diffuse.tga"; //Default built-in
	obj.mesh = "SoulSpear_Low:SoulSpear_Low1";    //Default built-in
	obj.tris = "SoulSpear_Low:SoulSpear_Low1";    //Default built-in

	plane.transform = glm::mat4(1);
	//plane.transform = glm::rotate(90.f, glm::vec3(0, 1, 0));
	plane.diffuse = "RockDiffuse";
	plane.mesh = "Quad";
	plane.tris = "Quad";

	fp.shader = "Forward";
	fp.fbo = "Screen";
	//fp.fbo    = "Screen"; //Default built-in

	//cp.shader = "Basic";
}

void MyTestApp::onStep()
{
	float time = nsfw::Window::instance().getTime();
	plane.transform = glm::rotate(-time * 10, glm::vec3(1, 0, 0));

	fp.prep();
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
