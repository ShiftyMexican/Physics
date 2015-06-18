#include "MyAntTweakBar.h"

MyAntTeakBar::MyAntTeakBar(GLFWwindow* window)
{
	m_clearColour = glm::vec4(0);
	m_perlinNoiseSeed = 0;
	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1240, 768);

	m_bar = TwNewBar("my bar");

	glfwSetMouseButtonCallback(window, OnMouseButton);
	glfwSetCursorPosCallback(window, OnMousePosition);
	glfwSetScrollCallback(window, OnMouseScroll);
	glfwSetKeyCallback(window, OnKey);
	glfwSetCharCallback(window, OnChar);
	glfwSetWindowSizeCallback(window, OnWindowResize);

	//TwAddVarRW(m_bar, "clear colour", TW_TYPE_COLOR4F, &m_clearColour[0], "");
	TwAddVarRW(m_bar, "X Light Direction", TW_TYPE_FLOAT, &m_lightDir_x, "group=Light");
	TwAddVarRW(m_bar, "Z Light Direction", TW_TYPE_FLOAT, &m_lightDir_z, "group=Light");
	TwAddVarRW(m_bar, "Light Colour", TW_TYPE_COLOR3F, &m_lightColour[0], "group=Light");
	TwAddVarRW(m_bar, "Perlin Noise Seed", TW_TYPE_FLOAT, &m_perlinNoiseSeed, "group=Ground");
	TwAddVarRW(m_bar, "Amplitude", TW_TYPE_FLOAT, &m_amplitude, "group=Ground");
	TwAddVarRW(m_bar, "Ice Level", TW_TYPE_FLOAT, &m_iceLevel, "group=Ground");

	m_lightColour = glm::vec3(1, 1, 1);

	m_amplitude = 0.8f;
	m_iceLevel = 0.0f;
}

MyAntTeakBar::~MyAntTeakBar()
{
	TwDeleteAllBars();
	TwTerminate();
}

void MyAntTeakBar::Update(float deltaTime)
{
	
}

void MyAntTeakBar::Draw()
{
	glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a);
	TwDraw();
}

glm::vec4 MyAntTeakBar::GetClearColour()
{
	return m_clearColour;
}

float MyAntTeakBar::GetPerlinSeed()
{
	return m_perlinNoiseSeed;
}

void MyAntTeakBar::SetPerlinNoise(float value)
{
	m_perlinNoiseSeed = value;
}

float MyAntTeakBar::GetLightDirX()
{
	return m_lightDir_x;
}

void MyAntTeakBar::SetLightDirX(float value)
{
	m_lightDir_x = value;
}

float MyAntTeakBar::GetLightDirZ()
{
	return m_lightDir_z;
}

void MyAntTeakBar::SetLightDirZ(float value)
{
	m_lightDir_z = value;
}

glm::vec3 MyAntTeakBar::GetLightColour()
{
	return m_lightColour;
}

float MyAntTeakBar::GetAmp()
{ 
	return m_amplitude;
}

void MyAntTeakBar::SetAmp(float value)
{
	m_amplitude = value;
}




