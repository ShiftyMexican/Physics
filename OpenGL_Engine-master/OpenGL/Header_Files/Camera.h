/*
	Author: Jarrod Dowsey
	Description: Basic Camera class
*/

#ifndef	CAMERA_H
#define CAMERA_H

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Application;
class Camera
{
public:

	Camera(GLFWwindow* _window);

	~Camera();

	void Update(float deltaTime);

	mat4 GetWorldTransform();

	void SetTransform(glm::mat4 transform);
	const glm::mat4 GetTransform() const;

	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition() const { return m_worldTransform[3].xyz(); }

	void LookAt(glm::vec3 lookAt, glm::vec3 up);
	void LookAt(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up);

	void SetupPerspective(float fieldOfView, float aspectRatio);

	const glm::mat4& GetProjection() const { return m_projectionTransform; }
	const glm::mat4& GetView() const { return m_viewTransform; }
	const glm::mat4& GetProjectionView() const { return m_projectionViewTransform; }

	bool GetPerspectiveSet() const { return m_bPerspectiveSet;  }
	
protected:

	void UpdateProjectionViewTransform();

	Application* myApp;
	GLFWwindow* m_window;

	//mat4 m_projection;
	//mat4 m_view;

	mat4 m_worldTransform;
	mat4 m_viewTransform;
	mat4 m_projectionTransform;
	mat4 m_projectionViewTransform;

	bool m_bPerspectiveSet;

	float x;
	float y;
	float z;
};

#endif