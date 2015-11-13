#include "Camera.h"
#include "Window.h"

void Camera::update()
{
	{
		auto &w = nsfw::Window::instance();
		auto const &time = w.getTime();
		//move camera right
		if (w.getKey('D'))
			m_transform[3] += m_transform[0] * (camSpeed * time);
		//move camera left
		if (w.getKey('A'))
			m_transform[3] -= m_transform[0] * (camSpeed * time);
		//move camera up
		if (w.getKey('W'))
			m_transform[3] -= m_transform[2] * (camSpeed * time);
		//move camera down
		if (w.getKey('S'))
			m_transform[3] += m_transform[2] * (camSpeed * time);
	}

	//lookAt(pos, target, up);
}