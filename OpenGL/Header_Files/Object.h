/*
	Author: Jarrod Dowsey
	Description: .obj file loader
*/

#ifndef	OBJECT_H
#define OBJECT_H

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
#include "Application.h"

struct Vertex;

class Object
{
public:

	Object(GLFWwindow* window, FreeCamera* _camera, unsigned int programID, const char* filename, const char* obj_name);

	~Object();

	void Update(float deltaTime);

	void Draw(glm::vec3 _lightDir, glm::vec3 _lightColour);

	void load_obj(std::vector<Vertex> &vertices, std::vector<glm::vec3> &normals, std::vector<unsigned int> &elements, std::vector<glm::vec2> &texCoord);

	unsigned int GetTexture();

	vec3 GetLightPos();

protected:

	unsigned int m_VAO;
	unsigned int m_IBO;
	unsigned int m_VBO;

	unsigned int m_indexCount;
	unsigned int m_vertexCount;

	const char* m_filename;
	const char* m_textureName;

	unsigned int m_texture;

	vec3 m_lightYPos;

	// For obj file
	std::vector<Vertex> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<unsigned int> m_elements;
	std::vector<glm::vec2> m_texCoord;
	

private:

	FreeCamera* m_camera;
	unsigned int m_programID;

};

#endif