/*	
	Author: Jarrod Dowsey
	Description: Simple Applicatuion class
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Object.h"


struct GridVertex {

	glm::vec4 position;
	glm::vec4 colour;
};

struct Vertex {

	glm::vec4 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
	glm::vec4 tangent;
};

class Object;
class FreeCamera;
class Assessment1;
class Application
{
public:

	// Constructor
	Application();

	// Destructor
	~Application();

	void Run();

	void Update();

	void Draw();

	void StartUp();

	float deltaTime;

	unsigned int HandleShader(const char* vertexShader, const char* fragmentShader, const char* a_geometryShader);

	unsigned int LoadShader(const char* shaderName, unsigned int uiType);

	unsigned char* FileToBuffer(const char* a_strName);

	unsigned int CreateProgram(GLuint vs, GLuint fs, GLuint gs);

	unsigned int CreateProgram(GLuint vs);

	unsigned int ShaderHotLoad();

protected:
	// Window
	GLFWwindow* window;

	// Camera Object
	FreeCamera* myCamera;

	// Assessment 1
	Assessment1* m_assessment1;

	float m_currentTime;
	float m_previousTime;

};

#endif