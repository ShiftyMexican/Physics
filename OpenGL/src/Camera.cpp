#include "Camera.h"
#include "Application.h"

Camera::Camera(GLFWwindow* _window)
{
	m_window = _window;

	m_viewTransform = glm::lookAt(vec3(1000,1000,1000), vec3(1000), vec3(0,1,0));
	m_projectionTransform = glm::perspective(glm::pi<float>() * 0.25f, 16/9.f, 0.1f, 1000.f);

	m_worldTransform[3] = glm::vec4(10,10,10,1);
}

Camera::~Camera()
{

}

void Camera::SetTransform(glm::mat4 transform)
{
	m_worldTransform = transform;
	UpdateProjectionViewTransform();
}

const glm::mat4 Camera::GetTransform() const
{
	return m_worldTransform;
}

void Camera::SetPosition(glm::vec3 position)
{
	m_worldTransform[3] = glm::vec4(position, 1);
	UpdateProjectionViewTransform();
}

void Camera::LookAt(glm::vec3 lookAt, glm::vec3 up)
{
	glm::vec4 vLocation = m_worldTransform[3];
	m_worldTransform = glm::inverse(glm::lookAt(vLocation.xyz(), lookAt, up));
	UpdateProjectionViewTransform();
}

void Camera::LookAt(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up)
{
	//glm::vec3 vUp(m_worldTransform[1].xyz);
	//Use vUp if rotating and looking down breaks everything
	m_worldTransform = glm::inverse(glm::lookAt(position, lookAt, up));
	UpdateProjectionViewTransform();
}

void Camera::SetupPerspective(float fieldOfView, float aspectRatio)
{
	m_projectionTransform = glm::perspective(fieldOfView, aspectRatio, 0.1f, 100000.0f);
	m_bPerspectiveSet = true;

	UpdateProjectionViewTransform();

}

void Camera::UpdateProjectionViewTransform()
{
	m_viewTransform = glm::inverse(m_worldTransform);
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}


void Camera::Update(float deltaTime)
{

	//m_viewTransform = glm::lookAt(m_worldTransform[3].xyz(), vec3(0), vec3(0,1,0));
	//m_projectionTransform = glm::perspective(glm::pi<float>() * 0.25f, 16/9.f, 0.1f, 10000.f);
	//m_worldTransform = glm::inverse(m_viewTransform);
	//
	//
	////Gizmos::draw(m_projectionTransform * m_viewTransform);
}

mat4 Camera::GetWorldTransform()
{
	return m_worldTransform;
}
	