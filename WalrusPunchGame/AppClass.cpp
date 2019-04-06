#include "AppClass.h"
using namespace Simplex;
using namespace std;
void Application::InitVariables(void)
{
	// Seed random numbers for the program
	srand(static_cast <unsigned> (time(0)));

	m_sProgrammer = "James DiGrazia jtd2401@rit.edu";

	//Hide the cursor
	m_pWindow->setMouseCursorVisible(false);

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 0.0f, 0.0f), //Position
		vector3(0.0f, 0.0f, -1.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

#ifdef DEBUG
	uint uInstances = 900;
#else
	uint uInstances = 1849;
#endif
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	m_uObjects = nSquare * nSquare;
	/*
	uint uIndex = -1;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			uIndex++;
			m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
			vector3 v3Position = vector3(glm::sphericalRand(34.0f));
			matrix4 m4Position = glm::translate(v3Position);
			m_pEntityMngr->SetModelMatrix(m4Position);
		}
	}
	*/

	// Balloon creation
	/*
	Balloon* b1 = new Balloon(Balloon::BalloonColor::Red);
	vector3 v3Position = vector3(0.0f, 0.0f, -5.0f);
	b1->MoveTo(v3Position);

	Balloon* b2 = new Balloon(Balloon::BalloonColor::Blue);
	v3Position = vector3(2.0f, 0.0f, -5.0f);
	b2->MoveTo(v3Position);
	*/

	m_BalloonMngr = new BalloonManager(
		4,
		vector3(0.0f, -1.0f, -7.5f),
		-AXIS_Z,
		AXIS_X,
		3.0f,
		15.0f,
		7.5f,
		10,
		500 // Change this to an actual time later
	);

	m_uOctantLevels = 3;
	m_uOctantID = 0;
	m_pRoot = new MyOctree(m_uOctantLevels);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	//Update Entity Manager
	// If octree exists, use the octree's collision check
	// Otherwise use the entity manager's collision check
	m_pEntityMngr->ClearCollisions();

	m_BalloonMngr->Update();

	if (m_pRoot == nullptr) {
		m_pEntityMngr->Update();
	}
	else {
		// Check collisions
		m_pRoot->CheckCollisions();
	}

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//display octree
	if (m_pRoot != nullptr && m_uOctantID == 1) {
		m_pRoot->Display(C_ORANGE);
	}

	m_BalloonMngr->DisplayLines(C_BLUE);

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui,
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}