#include "ParticleSystem.h"

ParticleEmitter::ParticleEmitter(FreeCamera* camera)
: m_particles(nullptr), m_maxParticles(0),
m_position(0, 0, 0), m_programID(0), m_programUpdate(0), m_lastDrawTime(0)
{
	m_vao[0] = 0;
	m_vao[1] = 0;
	m_vbo[0] = 0;
	m_vbo[1] = 0;

	m_programID = 0;
	m_programUpdate = 0;
	m_camera = camera;

	m_firstDead = 0;
}

ParticleEmitter::~ParticleEmitter()
{
	delete[] m_particles;

	glDeleteVertexArrays(2, m_vao);
	glDeleteBuffers(2, m_vbo);

	glDeleteProgram(m_programID);
	glDeleteProgram(m_programUpdate);
	//glDeleteBuffers(1, &m_ibo);
}

void ParticleEmitter::initialize(unsigned int maxParticles, float lifetimeMin, float lifetimeMax, float velocityMin, float velocityMax,
	float startSize, float endSize, const glm::vec4& startColour, const glm::vec4& endColour)
{
	// Store all variables passed in 
	m_startColour = startColour;
	m_endColour = endColour;
	m_startSize = startSize;
	m_endSize = endSize;
	m_velocityMin = velocityMin;
	m_velocityMax = velocityMax;
	m_lifespanMin = lifetimeMin;
	m_lifespanMax = lifetimeMax;
	m_maxParticles = maxParticles;

	// create particle array
	m_particles = new Particle[maxParticles];
	m_activeBuffer = 0;

	CreateBuffers();

	CreateUpdateShader();

	CreateDrawShader();

	/*
	// create the array of verticies fpr the particles
	// 4 verticies per particle for a quad
	// will be filled during the update
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	// create the index buffeer data for the particles
	// 6 indices per quad of 2 triangles
	// fill it now as it never changes

	unsigned int* indexData = new unsigned int[m_maxParticles * 6];

	for (unsigned int i = 0; i < m_maxParticles; i++)
	{
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;

		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	//Create a new VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Generate GL Buffers
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 * sizeof(ParticleVertex), m_vertexData, GL_DYNAMIC_DRAW);

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 * sizeof(unsigned int), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // colour
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;
	*/

}

/*void ParticleEmitter::Emit()
{
	// only emit if there is a dead particle to use
	if (m_firstDead >= m_maxParticles)
		return;

	// resurect the first dead particle
	Particle& particle = m_particles[m_firstDead++];

	// assign its starting position
	particle.position = m_position;


	// randomise its lifespan
	particle.lifetime = 0;
	particle.lifespan = (rand() / (float)RAND_MAX) * (m_lifespanMax - m_lifespanMin) + m_lifespanMin;

	// set starting size and colour
	particle.colour = m_startColour;
	particle.size = m_startSize;

	// randomise velocity direction and strength
	float velocity = (rand() / (float)RAND_MAX) * (m_velocityMax - m_velocityMin) + m_velocityMin;
	particle.velocity.x	= (rand() / (float)RAND_MAX) * 2 -1;
	particle.velocity.y	= (rand() / (float)RAND_MAX) * 2 -1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 -1;
	particle.velocity = glm::normalize(particle.velocity) * velocity;
}

/*void ParticleEmitter::Update(float deltaTime, const glm::mat4& cameraTransform)
{
	using glm::vec3;
	using glm::vec4;

	// spawn particles
	m_emitTimer += deltaTime;

	while (m_emitTimer > m_emitRate)
	{
		Emit();
		m_emitTimer -= m_emitRate;
	}

	/*
	unsigned int quad = 0;

	// update particles and turn live particles into billboard quads
	for (unsigned int i = 0; i < m_firstDead; i++)
	{
		Particle* particle = &m_particles[i];

		particle->lifetime += deltaTime;

		if (particle->lifetime >= particle->lifespan)
		{
			// Swap last alive with this one
			*particle = m_particles[m_firstDead - 1];
			m_firstDead--;
		}
		else
		{
			// Move Particle
			particle->position += particle->velocity * deltaTime;

			// Size Particle
			particle->size = glm::mix(m_startSize, m_endSize, particle->lifetime / particle->lifespan);

			// Colour Particle
			particle->colour = glm::mix(m_startColour, m_endColour, particle->lifetime / particle->lifespan);

			// make a quad the correct size and colour
			float halfSize = particle->size * 0.5f;

			m_vertexData[quad * 4 + 0].position = vec4(halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 0].colour = particle->colour;

			m_vertexData[quad * 4 + 1].position = vec4(-halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 1].colour = particle->colour;

			m_vertexData[quad * 4 + 2].position = vec4(-halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 2].colour = particle->colour;

			m_vertexData[quad * 4 + 3].position = vec4(halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 3].colour = particle->colour;

			// Create Billboard transform
			vec3 zAxis = glm::normalize(vec3(cameraTransform[3]) - particle->position);
			vec3 xAxis = glm::cross(vec3(cameraTransform[1]), zAxis);
			vec3 yAxis = glm::cross(zAxis, xAxis);
			glm::mat4 billboard(vec4(xAxis, 0), vec4(yAxis, 0), vec4(zAxis, 0), vec4(0, 0, 0, 1));

			m_vertexData[quad * 4 + 0].position = billboard * m_vertexData[quad * 4 + 0].position + vec4(particle->position, 0);
			m_vertexData[quad * 4 + 1].position = billboard * m_vertexData[quad * 4 + 1].position + vec4(particle->position, 0);
			m_vertexData[quad * 4 + 2].position = billboard * m_vertexData[quad * 4 + 2].position + vec4(particle->position, 0);
			m_vertexData[quad * 4 + 3].position = billboard * m_vertexData[quad * 4 + 3].position + vec4(particle->position, 0);

			++quad;
		}
	}
}*/

void ParticleEmitter::Draw(float time, const glm::mat4& cameraTransform, const glm::mat4& projectionView)
{
	glUseProgram(m_programUpdate);

	 int location = glGetUniformLocation(m_programUpdate, "time");
	 glUniform1f(location, time);

	 float deltaTime = time - m_lastDrawTime; m_lastDrawTime = time;

	 location = glGetUniformLocation(m_programUpdate, "deltaTime");
	 glUniform1f(location, deltaTime);

	 location = glGetUniformLocation(m_programUpdate, "emitterPosition");
	 glUniform3fv(location, 1, &m_position[0]);

	 glEnable(GL_RASTERIZER_DISCARD);

	// Draw Particles
	glBindVertexArray(m_vao[m_activeBuffer]);

	unsigned int otherBuffer = (m_activeBuffer + 1) % 2;

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	glUseProgram(m_programID);

	location = glGetUniformLocation(m_programID, "projectionView");
	glUniformMatrix4fv(location, 1, false, &projectionView[0][0]);

	location = glGetUniformLocation(m_programID, "cameraTransform");
	glUniformMatrix4fv(location, 1, false, &cameraTransform[0][0]);

	glBindVertexArray(m_vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	m_activeBuffer = otherBuffer;
}

void ParticleEmitter::CreateBuffers()
{
	glGenVertexArrays(2, m_vao);
	glGenBuffers(2, m_vbo);

	// Find Buffer
	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(Particle), m_particles, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // velocity
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 28);

	// Second Buffer
	glBindVertexArray(m_vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(Particle), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // velocity
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 28);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ParticleEmitter::CreateDrawShader()
{
	unsigned int vs = LoadShader("Particle_Shader.vert", GL_VERTEX_SHADER);
	unsigned int fs = LoadShader("Particle_Shader.frag", GL_FRAGMENT_SHADER);
	unsigned int gs = LoadShader("Particle_Shader.geo", GL_GEOMETRY_SHADER);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vs);
	glAttachShader(m_programID, fs);
	glAttachShader(m_programID, gs);
	glLinkProgram(m_programID);

	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);

	glUseProgram(m_programID);

	int location = glGetUniformLocation(m_programID, "sizeStart");
	glUniform1f(location, m_startSize);

	location = glGetUniformLocation(m_programID, "sizeEnd");
	glUniform1f(location, m_endSize);

	location = glGetUniformLocation(m_programID, "colourStart");
	glUniform4fv(location, 1, &m_startColour[0]);

	location = glGetUniformLocation(m_programID, "colourEnd");
	glUniform4fv(location, 1, &m_endColour[0]);

}

void ParticleEmitter::CreateUpdateShader()
{

	unsigned int vs = LoadShader("Particle_Shader_Update.vert", GL_VERTEX_SHADER);

	m_programUpdate = glCreateProgram();

	glAttachShader(m_programUpdate, vs);
	// specify the data that we will stream back
	const char* varyings[] = { "position", "velocity", "lifetime", "lifespan" };

	glTransformFeedbackVaryings(m_programUpdate, 4, varyings, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(m_programUpdate);

	glDeleteShader(vs);

	glUseProgram(m_programUpdate);

	//bind lifetime minimum and maximum
	int location = glGetUniformLocation(m_programUpdate, "lifeMin");
	glUniform1f(location, m_lifespanMin);

	location = glGetUniformLocation(m_programUpdate, "lifeMax");
	glUniform1f(location, m_lifespanMax);

	location = glGetUniformLocation(m_programUpdate, "maxVelocity");
	glUniform1f(location, m_velocityMax);

}

unsigned int ParticleEmitter::LoadShader(const char* shaderName, unsigned int uiType)
{
	int success = GL_FALSE;

	unsigned int handle = glCreateShader(uiType);
	unsigned char* source = FileToBuffer(shaderName);

	glShaderSource(handle, 1, (const char**)&source, 0);
	glCompileShader(handle);

	//m_programID = glCreateProgram();
	//
	//glAttachShader(m_programID, handle);
	//glLinkProgram(m_programID);
	//
	//glGetProgramiv(m_programID, GL_LINK_STATUS, &success);

	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetShaderInfoLog(handle, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);

		delete[] infoLog;
		return 0;
	}

	delete[] source;

	return handle;
}

unsigned char* ParticleEmitter::FileToBuffer(const char* a_strName)
{
	// open file for text reading
	FILE* pFile = fopen(a_strName, "rb");
	if (pFile == nullptr)
	{
		printf_s("Error: Unable to open file '%s' for reading!\n", a_strName);
		return nullptr;
	}

	// get number of bytes in file
	fseek(pFile, 0, SEEK_END);
	unsigned int uiLength = (unsigned int)ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// allocate buffer and read file contents
	unsigned char* acBuffer = new unsigned char[uiLength + 1];
	memset(acBuffer, 0, uiLength + 1);
	fread(acBuffer, sizeof(unsigned char), uiLength, pFile);

	fclose(pFile);
	return acBuffer;
}
