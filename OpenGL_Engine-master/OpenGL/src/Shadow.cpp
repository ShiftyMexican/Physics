#include "Shadow.h"

Shadow::Shadow(unsigned int shadowProgram, unsigned int shadowGenProgram, FreeCamera* camera)
{
	m_shadowGenProgram = shadowGenProgram;
	m_programID = shadowProgram;
	m_fbx = new FBXFile();
	m_camera = camera;
}

Shadow::~Shadow()
{

}

void Shadow::Init()
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_fboDepth);
	glBindTexture(GL_TEXTURE_2D, m_fboDepth);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_fboDepth, 0);

	//glGenRenderbuffers(1, &m_fboDepth);
	//glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1240, 768);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);
	//GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };

	glDrawBuffer(GL_NONE);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_fboTexture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_lightDirection = glm::normalize(glm::vec3(1, 2.5f, 1));

	glm::mat4 lightProjection = glm::ortho < float > (-10, 10, -10, 10, -10, 10);

	glm::mat4 lightView = glm::lookAt(m_lightDirection, glm::vec3(0), glm::vec3(0, 1, 0));

	m_lightMatrix = lightProjection * lightView;

	ShadowMap();

	//float vertexData[] =
	//{
	//	-1, -1, -1, 1, 0, 0,
	//	1, -1, -1, 1, 1, 0,
	//	1, 1, -1, 1, 1, 1,
	//	-1, 1, -1, 1, 0, 1,
	//};
	//unsigned int indexData[] =
	//{
	//	0, 1, 2,
	//	0, 2, 3,
	//};
	////Create a new VAO
	//glGenVertexArrays(1, &m_vao);
	//glBindVertexArray(m_vao);
	//// Generate GL Buffers
	//glGenBuffers(1, &m_vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 6 * 4, vertexData, GL_STATIC_DRAW);
	//
	//glGenBuffers(1, &m_ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* 6, indexData, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* 6, 0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* 6, ((char*)0) + 16);
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Shadow::Draw()
{

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_shadowGenProgram);

	int loc = glGetUniformLocation(m_shadowGenProgram, "LightMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_lightMatrix[0][0]));

	//for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i)
	//{
	//	FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);
	//
	//	unsigned int* glData = (unsigned int*)mesh->m_userData;
	//
	//	glBindVertexArray(glData[0]);
	//	glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	//}

	SecondRender();


}
void Shadow::SecondRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1240, 768);
	glClear(GL_DEPTH_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	int loc = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_camera->GetProjectionView()[0][0]));

	glm:mat4 textureSpaceOffset(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	glm::mat4 lightMatrix = textureSpaceOffset * m_lightMatrix;

	loc = glGetUniformLocation(m_programID, "LightMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &lightMatrix[0][0]);

	loc = glGetUniformLocation(m_programID, "lightDir");
	glUniform3fv(loc, 1, &m_lightDirection[0]);

	loc = glGetUniformLocation(m_programID, "shadowMap");
	glUniform1i(loc, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fboDepth);

	for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);

		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
}


void Shadow::ShadowMap()
{
	glm::mat4 textureSpaceOffset(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
		);

	glm::mat4 lightMatrix = textureSpaceOffset * m_lightMatrix;
}


