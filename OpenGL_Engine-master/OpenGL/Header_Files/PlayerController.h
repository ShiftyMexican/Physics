/*
	Author: Jarrod Dowsey	
	Description: Player controller class
*/

#ifndef	PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include "MyControllerHitReport.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class PlayerController
{
public:
	PlayerController(PxScene* _scene, PxMaterial* _material, GLFWwindow* window);
	~PlayerController();

	void Update(float deltaTime);
	void Draw();

private:

	MyControllerHitReport *m_myHitReport;
	PxControllerManager* gCharacterManager;
	PxController* gPlayerController;

	float m_characterYVelocity;
	float m_characterRotation;
	float m_playerGravity;

	PxVec3 m_startPosition;

	GLFWwindow* m_window;
};

#endif