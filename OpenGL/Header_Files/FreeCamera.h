/*
	Author: Jarrod Dowsey
	Description: Free Camera Class
*/
#include <Camera.h>

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>


#ifndef	FREECAMERA_H
#define FREECAMERA_H

class FreeCamera : public Camera
{
public:

	FreeCamera(GLFWwindow* _window) : Camera(m_window)
	{
		m_window = _window;
		m_bViewButtonClicked = false;
		m_flySpeed = 10.0f;
	}

	~FreeCamera();

	virtual void Update(float deltaTime);

	void SetFlySpeed(float speed);
	float GetFlySpeed() const { return m_flySpeed; }

protected:

	void HandleKeyboardInput(float dt);
	void HanldeMouseInput(float dt);

	void CalculateRotation(double xOffset, double yOffset);

	GLFWwindow* m_window;
	float m_flySpeed;

	bool m_bViewButtonClicked;

	double m_dCursorX, m_dCursorY;

	float m_deltaTime;
};

#endif