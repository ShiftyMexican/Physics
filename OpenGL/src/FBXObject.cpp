#include "FBXObject.h"


FBXObject::FBXObject(const char* filename)
{

}

FBXObject::FBXObject(GLFWwindow* window, unsigned int programID, FreeCamera* _camera, const char* filename)
{
	m_programID = programID;
	m_camera = _camera;

	m_fbx = new FBXFile();
	m_fbx->load(filename);
	m_fbx->initialiseOpenGLTextures();
	CreateOpenGLBuffers(m_fbx);

	m_timer = 0.0f;

	m_lightYPos = vec3(10);
	m_fbx->getAnimationByIndex(0)->m_endFrame = 85;
}

FBXObject::~FBXObject()
{
	CleanupOpenGLBuffers(m_fbx);
}

void FBXObject::Update(float deltaTime)
{
	FBXSkeleton* skeleton = m_fbx->getSkeletonByIndex(0);
	FBXAnimation* animation = m_fbx->getAnimationByIndex(0);

	m_timer += deltaTime;

	skeleton->evaluate(animation, m_timer);
	
	for (unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index)
	{
		skeleton->m_nodes[bone_index]->updateGlobalTransform();
	}
}

void FBXObject::Draw(glm::vec3 _lightDir, glm::vec3 _lightColour)
{
	glUseProgram(m_programID);

	unsigned int uiProjViewLocation = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(uiProjViewLocation, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjectionView()));

 	FBXSkeleton* skeleton = m_fbx->getSkeletonByIndex(0);
	skeleton->updateBones();
	
	int bones_location = glGetUniformLocation(m_programID, "bones");
	glUniformMatrix4fv(bones_location, skeleton->m_boneCount, GL_FALSE, (float*)skeleton->m_bones);

	for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->m_material->textures[FBXMaterial::DiffuseTexture]->handle);

		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glBindVertexArray(glData[0]);

		// diffuse 
		// ambient colour uniform
		// diffuse colour uniform

		unsigned int diffuse = glGetUniformLocation(m_programID, "diffuse");
		glUniform1i(diffuse, 0);

		unsigned int normal = glGetUniformLocation(m_programID, "normal");
		glUniform1i(normal, 1);

		unsigned int uiLightPositionLocation = glGetUniformLocation(m_programID, "LightPos");
		glUniform3fv(uiLightPositionLocation, 1, glm::value_ptr(m_lightYPos));

		vec3 lightDir = glm::normalize(_lightDir);
		unsigned int uiLightDirectionLocation = glGetUniformLocation(m_programID, "LightDir");
		glUniform3f(uiLightDirectionLocation, lightDir.x, lightDir.y, lightDir.z);

		unsigned int uiLightColourLocation = glGetUniformLocation(m_programID, "LightColour");
		glUniform3f(uiLightColourLocation, _lightColour.x, _lightColour.y, _lightColour.z);

		unsigned int uiCameraLocation = glGetUniformLocation(m_programID, "CameraPos");
		glUniform3fv(uiCameraLocation, 0, glm::value_ptr(m_camera->GetPosition())); // 0.0f, 1.0f, 0.0f); // glm::value_ptr(m_lightYPos));

		unsigned int uiSpecPow = glGetUniformLocation(m_programID, "SpecPow");
		glUniform1f(uiSpecPow, 125.0f);

		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

	
}

void FBXObject::CreateOpenGLBuffers(FBXFile* fbx)
{
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);

		// Storage for the openGL data in 3 unsigned int
		unsigned int* glData = new unsigned int[3];

		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);

		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);

		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);

		glBufferData(GL_ARRAY_BUFFER, mesh->m_vertices.size() * sizeof(FBXVertex), mesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size() * sizeof(unsigned int), mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // position
		glEnableVertexAttribArray(1); // normals
		glEnableVertexAttribArray(2); // tangents
		glEnableVertexAttribArray(3); // texcoords
		glEnableVertexAttribArray(4); // weights
		glEnableVertexAttribArray(5); // indices 

		// Position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::PositionOffset);

		// Normals
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), (void*)FBXVertex::NormalOffset);

		// tangents
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), (void*)FBXVertex::TangentOffset);

		// tex coords
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::TexCoord1Offset);

		// Wieghts
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::WeightsOffset);

		// indices
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::IndicesOffset);

		glBindVertexArray(0);

		mesh->m_userData = glData;
	}
}

void FBXObject::CleanupOpenGLBuffers(FBXFile* fbx)
{
	// Clean up the vertex data attactched to each mesh
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);

		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);

		delete[] glData;
	}
}

vec3 FBXObject::GetLightPos()
{
	return m_lightYPos;
}

unsigned int FBXObject::GetTexture()
{
	return m_texture;
}