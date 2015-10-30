#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>


struct Camera
{
	glm::mat4 m_transform;
	float m_near, m_far, aspect, fov;

	Camera() : m_near(1.f), m_far(100), aspect(800 / 600.f), fov(90) {}

	void update() {}

	void lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
	{
		m_transform = glm::inverse(glm::lookAt(pos, target, up));
	}

	glm::mat4 getView()		  const { return glm::inverse(m_transform); }
	glm::mat4 getProjection() const { return glm::perspective(fov, aspect, m_near, m_far); }
};