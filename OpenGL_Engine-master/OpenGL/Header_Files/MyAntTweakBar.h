/* 
	Author: Jarrod Dowsey
	Description: GUI Bar
*/

#ifndef	MYANTTWEAKBAR_H
#define MYANTTWEAKBAR_H

#include "AntTweakBar.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "GroundSurface.h"

class MyAntTeakBar
{
public:

	MyAntTeakBar(GLFWwindow* window);

	~MyAntTeakBar();

	void Update(float deltaTime);

	void Draw();

	static void OnMouseButton(GLFWwindow*, int b, int a, int m)
	{
		TwEventMouseButtonGLFW(b, a);
	}

	static void OnMousePosition(GLFWwindow*, double x, double y)
	{
		TwEventMousePosGLFW((int)x, (int)y);
	}

	static void OnMouseScroll(GLFWwindow*, double x, double y)
	{
		TwEventMouseWheelGLFW((int)y);
	}

	static void OnKey(GLFWwindow*, int k, int s, int a, int m)
	{
		TwEventKeyGLFW(k, a);
	}

	static void OnChar(GLFWwindow*, unsigned int c)
	{
		TwEventCharGLFW(c, GLFW_PRESS);
	}

	static void OnWindowResize(GLFWwindow*, int w, int h)
	{
		TwWindowSize(w, h);
		glViewport(0, 0, w, h);
	}

	glm::vec4 GetClearColour();

	float GetPerlinSeed();

	void SetPerlinNoise(float value);
	float GetLightDirX();
	float GetLightDirZ();
	void SetLightDirX(float value);
	void SetLightDirZ(float value);

	float GetAmp();
	void SetAmp(float value);

	float GetIceLevel() { return m_iceLevel; }
	void SetIceLevel(float value) { m_iceLevel = value;  }

	glm::vec3 GetLightColour();

	

protected:

	TwBar* m_bar;
	glm::vec4 m_clearColour;
	glm::vec3 m_light;
	glm::vec3 m_lightColour;

	float m_perlinNoiseSeed;

	float m_lightDir_x;
	float m_lightDir_z;

	float m_amplitude;
	float m_iceLevel;
	

private:
};

#endif