/*
	Author: Jarrod Dowsey
	Description: FBX Object
*/

#ifndef	FBX_OBJECT_H
#define FBX_OBJECT_H

#include "FBXFile.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "FreeCamera.h"

class FBXObject
{
public:

	FBXObject(const char* filename);
	FBXObject(GLFWwindow* window, unsigned int programID, FreeCamera* _camera, const char* filename);

	~FBXObject();

	void Update(float deltaTime);

	void Draw(glm::vec3 _lightDir, glm::vec3 _lightColour);

	void CreateOpenGLBuffers(FBXFile* fbx);

	void CleanupOpenGLBuffers(FBXFile* fbx);

	vec3 GetLightPos();

	unsigned int GetTexture();

protected:

	FBXFile* m_fbx;
	FreeCamera* m_camera;

	vec3 m_lightYPos;

	unsigned int m_programID;

	unsigned int m_texture;

	float m_timer;

private:

};

#endif