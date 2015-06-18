/*
	Author: Jarrod Dowsey
	Description: Render Targets Class
*/

#ifndef RENDER_TARGET_H
#define  RENDER_TARGET_H

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "MyAntTweakBar.h"

class RenderTarget
{

public:

	RenderTarget(unsigned int programID);

	~RenderTarget();

	void Init();

	void Draw();

	void SetAsActiveRenderTarget();

	void ClearActiveRenderTarget();


protected:

	unsigned int m_fbo;
	unsigned int m_fboTexture;
	unsigned int m_fboDepth;

	unsigned int m_vao;
	unsigned int m_ibo;
	unsigned int m_vbo;

	unsigned int m_programID;

private:

};

#endif