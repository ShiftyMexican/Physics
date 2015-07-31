#include "PlayerController.h"
#include "MyControllerHitReport.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Utilities.h"

using namespace physx;

PlayerController::PlayerController(PxScene* _scene, PxMaterial* _material, GLFWwindow* window)
{
	m_myHitReport = new MyControllerHitReport();

	gCharacterManager = PxCreateControllerManager(*_scene);

	//describe our controller...
	PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 0, 0);
	desc.material = _material;
	desc.reportCallback = m_myHitReport; //connect it to our collision detection routine
	desc.density = 10;

	//create the layer controller
	gPlayerController = gCharacterManager->createController(desc);
	gPlayerController->setPosition(PxExtendedVec3(0, 0, 20));

	//set up some variables to control our player with
	m_characterYVelocity = 0; //initialize character velocity
	m_characterRotation = 0; //and rotation
	m_playerGravity = -0.5f; //set up the player gravity
	m_myHitReport->ClearPlayerContactNormal(); //initialize the contact normal (what we are in contact with)

	m_window = window;
	
}

PlayerController::~PlayerController()
{

}

void PlayerController::Update(float deltaTime)
{
	bool onGround; //set to true if we are on the ground
	float movementSpeed = 10.0f; //forward and back movement speed
	float rotationSpeed = 1.0f; //turn speed
	//check if we have a contact normal. if y is greater than .3 we assume this issolid ground. This is a rather primitive way to do this.Can you do better ?
	if (m_myHitReport->GetPlayerContactNormal().y > 0.3f)
	{
		m_characterYVelocity = -0.1f;
		onGround = true;
	}
	else
	{
		m_characterYVelocity += m_playerGravity * deltaTime;
		onGround = false;
	}
	m_myHitReport->ClearPlayerContactNormal();
	const PxVec3 up(0, 1, 0);

	//scan the keys and set up our intended velocity based on a global transform
	PxVec3 velocity(0, m_characterYVelocity, 0);
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		velocity.z -= movementSpeed * deltaTime;
	}

	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		velocity.z += movementSpeed * deltaTime;
	}

	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		velocity.x -= movementSpeed * deltaTime;
	}

	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		velocity.x += movementSpeed * deltaTime;
	}

	//To do.. add code to control z movement and jumping
	float minDistance = 0.001f;
	PxControllerFilters filter;

	//move the controller
	gPlayerController->move(velocity, minDistance, deltaTime, filter);
}

void PlayerController::Draw() 
{
	PxU32 nShapes = gPlayerController->getActor()->getNbShapes();
	PxShape** shapes = new PxShape*[nShapes];
	gPlayerController->getActor()->getShapes(shapes, nShapes);

	while (nShapes--) 
	{
		Utilities::AddWidget(shapes[nShapes], gPlayerController->getActor());
	}
}