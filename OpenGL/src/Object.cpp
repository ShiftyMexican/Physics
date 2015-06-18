#include "Object.h"
#include "Application.h"

#include <stb_image.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;


Object::Object(GLFWwindow* window, FreeCamera* _camera, unsigned int programID, const char* filename, const char* texture_name)
{
	m_filename = filename;
	m_textureName = texture_name;

	m_programID = programID;

	m_camera = _camera;

	m_vertexCount = 0;
	m_indexCount = 0;

	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	load_obj(m_vertices, m_normals, m_elements, m_texCoord);

	unsigned char* data = stbi_load(m_textureName, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
}

Object::~Object()
{

}

void Object::Update(float deltaTime)
{

	m_lightYPos = glm::vec3(10, 10, 10);

	m_lightYPos = glm::vec3((sin(glfwGetTime()), 1, cos(glfwGetTime())));

	m_lightYPos = glm::vec3((sin(glfwGetTime()), 1, cos(glfwGetTime())));

}

void Object::Draw(glm::vec3 _lightDir, glm::vec3 _lightColour)
{
	glUseProgram(m_programID);

	unsigned int uiProjViewLocation = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(uiProjViewLocation, 1, false, glm::value_ptr(m_camera->GetProjectionView()));
	
	//unsigned int uiLightPositionLocation = glGetUniformLocation(m_programID, "LightPos");
	//glUniform3f(uiLightPositionLocation, lightPos.x, lightPos.y, lightPos.z);

	unsigned int uiLightColourLocation = glGetUniformLocation(m_programID, "LightColour");
	glUniform3f(uiLightColourLocation, _lightColour.x, _lightColour.y, _lightColour.z);

	unsigned int uiCameraLocation = glGetUniformLocation(m_programID, "CameraPos");
	glUniform3fv(uiCameraLocation, 0, glm::value_ptr(m_camera->GetPosition())); // 0.0f, 1.0f, 0.0f); // glm::value_ptr(m_lightYPos));


	vec3 lightDir = glm::normalize(_lightDir);

	unsigned int uiLightDir = glGetUniformLocation(m_programID, "LightDir");
	glUniform3f(uiLightDir, lightDir.x, lightDir.y, lightDir.z);

	unsigned int uiSpecPow = glGetUniformLocation(m_programID, "SpecPow");
	glUniform1f(uiSpecPow, 125.0f);

	unsigned int loc = glGetUniformLocation(m_programID, "diffuse");
	glUniform1f(loc, 0);

	loc = glGetUniformLocation(m_programID, "normal");
	glUniform1f(loc, 1);



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

void Object::load_obj(std::vector<Vertex> &vertices, std::vector<glm::vec3> &normals, std::vector<unsigned int> &elements, std::vector<glm::vec2> &texCoord)
{
	//Create a new VAO
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray( m_VAO );
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)((sizeof(vec4)) + sizeof(glm::vec2)));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)((sizeof(vec4)) + sizeof(glm::vec4) + sizeof(float)));

	std::ifstream in(m_filename, std::ios::in);
	if(!in) { std::cerr << "cannot open" << m_filename << std::endl; exit(1); }

	std::string line;

	while (std::getline(in, line)) 
	{
		if(line.substr(0,2) == "v ")
		{
			std::istringstream s(line.substr(2));
			Vertex v; s >> v.position.x; s >> v.position.y; s >> v.position.z; v.position.w = 1.0f;
			//v.colour = glm::vec4(1, 1, 1, 1);
			vertices.push_back(v);
			m_vertexCount++;
		}

		else if (line.substr(0,2) == "f ")
		{
			char dummyChar;
			//int dummyInt;
			std::istringstream s(line.substr(2));
			unsigned int a,b,c;
			unsigned int aTex,bTex,cTex;
			unsigned int aNorm,bNorm,cNorm;

			s >> a; s >> dummyChar; s >> aTex; s >> dummyChar; s >> aNorm;
			s >> b; s >> dummyChar; s >> bTex; s >> dummyChar; s >> bNorm;
			s >> c; s >> dummyChar; s >> cTex; s >> dummyChar; s >> cNorm;
			a--; b--; c--;
			aTex--; bTex--; cTex--;
			aNorm--; bNorm--; cNorm--;

			elements.push_back(a); elements.push_back(b); elements.push_back(c);
			vertices[a].texCoord = texCoord[aTex];
			vertices[b].texCoord = texCoord[bTex];
			vertices[c].texCoord = texCoord[cTex];
			vertices[a].normal = normals[aNorm]; 
			vertices[b].normal = normals[bNorm];
			vertices[c].normal = normals[cNorm];

			m_indexCount  += 3;
		}
		else if (line.substr(0,3) == "vt ")
		{
			std::istringstream s(line.substr(3));
			glm::vec2 coord; s >> coord.x; s >> coord.y;
			coord.y *= -1.0f;
			texCoord.push_back(coord);

		}
		else if (line.substr(0, 3) == "vn ")
		{
			std::istringstream s(line.substr(3));
			glm::vec3 coord; s >> coord.x; s >> coord.y; s >> coord.z;
			normals.push_back(coord);
		}
		else if ( line[0] == '#') { /* Nothing goes here because ignoring this line */ }
		else { /* also ignore */ }
	}

	//normals.resize(vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));
	
	//for (unsigned int i = 0; i < elements.size(); i+=3)
	//{
	//	GLushort ia = elements[i];
	//	GLushort ib	= elements[i+1];
	//	GLushort ic	= elements[1+2];
	//
	//	glm::vec3 normal = glm::normalize(glm::cross(	
	//		glm::vec3(vertices[ib].position) - glm::vec3(vertices[ia].position),
	//		glm::vec3(vertices[ic].position) - glm::vec3(vertices[ia].position)));
	//
	//	normals[ia] = normals[ib] = normals[ic] = normal;
	//	//vertices[ia].colour = vertices[ib].colour = vertices[ic].colour = glm::vec4(normal, 1.0f);
	//}

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), &elements[0], GL_STATIC_DRAW);
}

vec3 Object::GetLightPos()
{
	return m_lightYPos;
}

unsigned int Object::GetTexture()
{
	return m_texture;
}