/*
	Author: Jarrod Dowsey
	Description: Shadow Class
*/

#ifndef SHADOW_H
#define SHADOW_H

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "FBXFile.h"
#include "FreeCamera.h"

class Shadow
{

public:

	Shadow(unsigned int shadowProgram, unsigned int shadowGenProgram, FreeCamera* camera);

	~Shadow();

	void Init();

	void Draw();

	void SecondRender();

	void ShadowMap();

	void SetAsActiveRenderTarget();

	void ClearActiveRenderTarget();


protected:

	FBXFile* m_fbx;
	FreeCamera* m_camera;

	unsigned int m_fbo;
	unsigned int m_fboTexture;
	unsigned int m_fboDepth;

	glm::vec3 m_lightDirection;
	glm::mat4 m_lightMatrix;

	unsigned int m_vao;
	unsigned int m_ibo;
	unsigned int m_vbo;

	unsigned int m_programID;
	unsigned int m_shadowGenProgram;

private:

};

#endif