#include "SkyBox.h"
#include <stb_image.h>

Skybox::Skybox(unsigned int programID, FreeCamera* camera)
{

	m_programID = programID;
	m_camera = camera;

	CreateBuffers();

	std::vector<const GLchar*> faces;

	faces.push_back("nightsky_right.jpg");
	faces.push_back("nightsky_left.jpg");
	faces.push_back("nightsky_top.jpg");
	faces.push_back("nightsky_top.jpg");
	faces.push_back("nightsky_front.jpg");
	faces.push_back("nightsky_back.jpg");

	faces.push_back("./data/alpine_right.jpg");
	faces.push_back("./data/alpine_left.jpg");
	faces.push_back("./data/alpine_top.jpg");
	faces.push_back("./data/alpine_top.jpg");
	faces.push_back("./data/alpine_back.jpg");
	faces.push_back("./data/alpine_front.jpg");

	LoadCubemap(faces);	
}

Skybox::~Skybox()
{

}

void Skybox::LoadCubemap(std::vector<const GLchar*> faces)
{
	int width, height;
	unsigned char* image;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	//glActiveTexture(GL_TEXTURE0);

	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = stbi_load(faces[i], &width, &height, 0, STBI_default);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::Draw()
{
	glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(m_programID);

	int iLoc = glGetUniformLocation(m_programID, "Projection");
	glUniformMatrix4fv(iLoc, 1, false, glm::value_ptr(m_camera->GetProjection()));


	iLoc = glGetUniformLocation(m_programID, "View");
	glUniformMatrix4fv(iLoc, 1, false, glm::value_ptr(m_camera->GetView()));


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glUniform1i(glGetUniformLocation(m_programID, "Skybox"), 0);

	glBindVertexArray(m_vao);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

}

void Skybox::CreateBuffers() 
{
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(GLfloat), skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindVertexArray(0);
}

//void Skybox::Draw(Camera* _camera) {
//	glDepthMask(GL_FALSE);
//	glUseProgram(m_program);
//	int loc = glGetUniformLocation(m_program, "projectionView");
//	glm::mat4 projectionView = _camera->GetProjection() * glm::mat4(glm::mat3(_camera->GetView()));
//	glUniformMatrix4fv(loc, 1, false, &projectionView[0][0]);
//	glBindVertexArray(m_vao);
//	glActiveTexture(GL_TEXTURE0);
//	glUniform1i(glGetUniformLocation(m_program, "skybox"), 0);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
//	glDrawArrays(GL_TRIANGLES, 0, 36);
//	glBindVertexArray(0);
//	glDepthMask(GL_TRUE);
//}
//void Skybox::LoadCubemap(std::vector<const char*> _faces) {
//	glGenTextures(1, &m_texture);
//	glActiveTexture(GL_TEXTURE0);
//	int width, height;
//	unsigned char* imageData;
//	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
//	for (GLuint i = 0; i < _faces.size(); i++) {
//		imageData = stbi_load(_faces[i], &width, &height, 0, STBI_default);
//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
//		stbi_image_free(imageData);
//	}
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//}
//void Skybox::CreateBuffers() {
//	GLfloat skyboxVertices[] = {
//		// Positions          
//		-1.0f, 1.0f, -1.0f,
//		-1.0f, -1.0f, -1.0f,
//		1.0f, -1.0f, -1.0f,
//		1.0f, -1.0f, -1.0f,
//		1.0f, 1.0f, -1.0f,
//		-1.0f, 1.0f, -1.0f,
//
//		-1.0f, -1.0f, 1.0f,
//		-1.0f, -1.0f, -1.0f,
//		-1.0f, 1.0f, -1.0f,
//		-1.0f, 1.0f, -1.0f,
//		-1.0f, 1.0f, 1.0f,
//		-1.0f, -1.0f, 1.0f,
//
//		1.0f, -1.0f, -1.0f,
//		1.0f, -1.0f, 1.0f,
//		1.0f, 1.0f, 1.0f,
//		1.0f, 1.0f, 1.0f,
//		1.0f, 1.0f, -1.0f,
//		1.0f, -1.0f, -1.0f,
//
//		-1.0f, -1.0f, 1.0f,
//		-1.0f, 1.0f, 1.0f,
//		1.0f, 1.0f, 1.0f,
//		1.0f, 1.0f, 1.0f,
//		1.0f, -1.0f, 1.0f,
//		-1.0f, -1.0f, 1.0f,
//
//		-1.0f, 1.0f, -1.0f,
//		1.0f, 1.0f, -1.0f,
//		1.0f, 1.0f, 1.0f,
//		1.0f, 1.0f, 1.0f,
//		-1.0f, 1.0f, 1.0f,
//		-1.0f, 1.0f, -1.0f,
//
//		-1.0f, -1.0f, -1.0f,
//		-1.0f, -1.0f, 1.0f,
//		1.0f, -1.0f, -1.0f,
//		1.0f, -1.0f, -1.0f,
//		-1.0f, -1.0f, 1.0f,
//		1.0f, -1.0f, 1.0f
//	};
//	glGenVertexArrays(1, &m_vao);
//	glGenBuffers(1, &m_vbo);
//	glBindVertexArray(m_vao);
//	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//	glBindVertexArray(0);
//}