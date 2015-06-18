/*
	Author: Jarrod Dowsey
	Description: Basic Particle System
*/

#ifndef	PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

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

using glm::vec3;
using glm::vec4;

struct Particle
{
	Particle() : lifetime(1), lifespan(0) {}


	vec3 position;
	vec3 velocity;
	float lifetime;
	float lifespan;

};

struct ParticleVertex
{
	vec4 position;
	vec4 colour;
};

class ParticleEmitter
{
public:
	ParticleEmitter(FreeCamera* camera);

	virtual ~ParticleEmitter();

	void initialize(unsigned int maxParticles, float lifetimeMin, float lifetimeMax, float velocityMin, float velocityMax, 
					float startSize, float endSize, const glm::vec4& startColour, const glm::vec4& endColour);

	//void Emit();

	//void Update(float deltaTime, const glm::mat4& cameraTransform);

	void Draw(float time, const glm::mat4& cameraTransform, const glm::mat4& projectionView);

	void CreateDrawShader();

	unsigned int LoadShader(const char* shaderName, unsigned int uiType);

	unsigned char* FileToBuffer(const char* a_strName);

	float m_lifespanMin;
	float m_lifespanMax;
	vec3 m_position;

protected:

	void CreateBuffers();
	void CreateUpdateShader();

	FreeCamera* m_camera;

	Particle* m_particles;

	unsigned int m_maxParticles;



	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	vec4 m_startColour;
	vec4 m_endColour;

	unsigned int m_activeBuffer;
	unsigned int m_vao[2];
	unsigned int m_vbo[2];

	float m_lastDrawTime;

	unsigned int m_programID;
	unsigned int m_programUpdate;

	unsigned int m_firstDead;

};

#endif