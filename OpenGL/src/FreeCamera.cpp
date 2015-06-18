#include "FreeCamera.h"
#include "Camera.h"

void FreeCamera::Update(float deltaTime)
{
	HandleKeyboardInput(deltaTime);
	HanldeMouseInput(deltaTime);

	//Gizmos::draw(m_projectionTransform * m_viewTransform);
}

void FreeCamera::SetFlySpeed(float speed)
{
	m_flySpeed = speed;
}

void FreeCamera::HandleKeyboardInput(float dt)
{
	glm::vec3 vRight = m_worldTransform[0].xyz;
	glm::vec3 vUp = m_worldTransform[1].xyz;
	glm::vec3 vForward = m_worldTransform[2].xyz;

	glm::vec3 moveDir(0.0f);

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		moveDir -= vForward;
	}

	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		moveDir += vForward;
	}

	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		moveDir -= vRight;
	}

	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		moveDir += vRight;
	}

	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		moveDir += glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		moveDir -= glm::vec3(0.0f, 1.0f, 0.0f);
	}

	float fLength = glm::length(moveDir);
	if (fLength > 0.01f)
	{
		moveDir = ((float)dt * m_flySpeed) * glm::normalize(moveDir);
		SetPosition(GetPosition() + moveDir);
	}


}

void FreeCamera::HanldeMouseInput(float dt)
{
	m_deltaTime = dt;

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		if (m_bViewButtonClicked == false)
		{
			int width, height;
			glfwGetFramebufferSize(m_window, &width, &height);


			m_dCursorX = width / 2.0;
			m_dCursorY = height / 2.0;
			glfwSetCursorPos(m_window, m_dCursorX, m_dCursorY);
			m_bViewButtonClicked = true;
		}
		else
		{
			double mouseX, mouseY;
			glfwGetCursorPos(m_window, &mouseX, &mouseY);

			double xOffset = mouseX - m_dCursorX;
			double yOffset = mouseY - m_dCursorY;

			m_dCursorX = mouseX;
			m_dCursorY = mouseY;

			CalculateRotation(xOffset, yOffset);

			std::cout << xOffset << " " << yOffset << std::endl;
		}
	}
	else
	{
		m_bViewButtonClicked = false;
	}
}

void FreeCamera::CalculateRotation(double xOffset, double yOffset)
{
	if (xOffset != 0)
	{
		glm::mat4 rot = glm::rotate((float)(m_deltaTime * -xOffset) * 0.3f, glm::vec3(0, 1, 0));
		SetTransform(GetTransform() * rot);
	}

	if (yOffset != 0)
	{
		glm::mat4 rot = glm::rotate((float)(m_deltaTime * -yOffset) * 0.3f, glm::vec3(1, 0, 0));

		SetTransform(GetTransform() * rot);
	}

	glm::mat4 oldTrans = GetTransform();

	glm::vec3 worldUp = glm::vec3(0, 1, 0);

	glm::mat4 trans;

	trans[0] = glm::vec4(glm::normalize(glm::cross(worldUp, oldTrans[2].xyz())), 0);

	trans[1] = glm::vec4(glm::normalize(glm::cross(oldTrans[2].xyz(), trans[0].xyz())), 0);

	trans[2] = glm::normalize(oldTrans[2]);

	trans[3] = oldTrans[3];

	SetTransform(trans);
}
