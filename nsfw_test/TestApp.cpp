#include "TestApp.h"
#include <ogl/gl_core_4_4.h>
#include <glm\glm.hpp>	

void TestApp::onInit()
{
	auto &A = nsfw::Assets::instance();
	A.loadShader("Basic", "../resources/shaders/basic.vert",
												 "../resources/shaders/basic.frag");
	
	A.loadTexture("RockDiffuse", "../resources/textures/rock_diffuse.tga");
	A.loadTexture("RockDisplacement", "../resources/textures/rock_displacement.tga");
	A.loadTexture("RockNormal", "../resources/textures/rock_normal.tga");
	A.loadTexture("RockSpecular", "../resources/textures/rock_specular.tga");

	A.loadFBX("", "../resources/fbx/soulspear/soulspear.fbx");

	const char *renderTargetNames[] = { "FINAL" };
	unsigned renderTargetDepths[] = { GL_RGBA };
	A.makeFBO("FINAL", 800, 600, 1, renderTargetNames, renderTargetDepths);
}

void TestApp::onPlay()
{
	camera.lookAt(glm::vec3(2,2,2), //offest top right and forward
				  glm::vec3(0, 0, 0),  // origin
				  glm::vec3(0, 1, 0)); // up
	
	obj.transform = glm::mat4(1);
	obj.diffuse   = "soulspear_diffuse.tga"; //Default built-in
	obj.mesh      = "SoulSpear_Low:SoulSpear_Low1";    //Default built-in
	obj.tris      = "SoulSpear_Low:SoulSpear_Low1";    //Default built-in

	fp.shader = "Basic";
	fp.fbo = "FINAL";
	//fp.fbo    = "Screen"; //Default built-in

	cp.shader = "Basic";
}

void TestApp::onStep()
{
	float time = nsfw::Window::instance().getTime();
	obj.transform = glm::rotate(time * 10, glm::vec3(0, 1, 0));

	fp.prep();
	fp.draw(camera, obj);
	fp.post();

	cp.prep();
	cp.draw();
	cp.post();
}

void TestApp::onTerm()
{
}
