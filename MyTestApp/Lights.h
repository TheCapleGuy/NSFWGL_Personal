#pragma once
#include <glm/ext.hpp>

struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 color;


	glm::mat4 getView() const 
	{
		return glm::lookAt(glm::vec3(direction), glm::vec3(0.f), glm::vec3(0.f,1.f,0.f));
	}

	glm::mat4 getProjection() const
	{
		return glm::ortho<float>(-10, 10,-10, 10, -10, 10);
	}

	glm::mat4 getMatrix() const
	{
		auto gp = getProjection();
		auto gv = getView();

		return getProjection() * getView();
	}
};