/*
	Author: Jarrod Dowsey
	Description: Skybox class, makes a skybox for the world
*/

#ifndef	SKYBOX_H
#define SKYBOX_H

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "FreeCamera.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Skybox
{
public:
	Skybox(unsigned int programID, FreeCamera* camera);

	~Skybox();

	void LoadCubemap(std::vector<const GLchar*> faces);

	void Draw();

	void CreateBuffers();

private:

	unsigned int m_programID;

	FreeCamera* m_camera;

	unsigned int m_texture;

	unsigned int m_vao;
	unsigned int m_vbo;

	GLuint m_cubemapTexture;

};

#endif