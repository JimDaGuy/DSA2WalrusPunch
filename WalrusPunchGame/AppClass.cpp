#include "AppClass.h"
using namespace Simplex;
using namespace std;

void Application::InitVariables(void)
{
	// Seed random numbers for the program
	srand(static_cast <unsigned> (time(0)));

	m_sProgrammer = "Walrus Punch";

	//Hide the cursor
	m_pWindow->setMouseCursorVisible(false);

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 3.0f, 8.0f), //Position
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

	InitBalloonManager();
	m_Dart = new Dart(vector3(0));

	m_uOctantID = 0;
	m_uOctantLevels = 3;
}
void Application::InitBalloonManager
(
	uint balloonRowCount,
	vector3 balloonRowFrontCenter,
	vector3 BalloonRowsForwardVector,
	vector3 BalloonRowsRightVector,
	float balloonRowSpacing,
	float balloonRowLength,
	float balloonMaxHeight,
	uint balloonMaxCount,
	uint msPerBalloonSpawn
)
{
	SafeDelete(m_BalloonMngr);

	m_BalloonMngr = new BalloonManager(
		balloonRowCount, // number of rows
		balloonRowFrontCenter, // center of rows
		BalloonRowsForwardVector, // forward vector
		BalloonRowsRightVector, // right vector
		balloonRowSpacing, // spacing between rows
		balloonRowLength, // length of lines
		balloonMaxHeight, // maximum height a balloon can reach before despawning
		balloonMaxCount, // maximum number of balloons
		msPerBalloonSpawn // ms per balloon spawn
	);

	// Reset octant levels, otherwise program will break
	m_uOctantLevels = 3;
	// Calculate the center of the octree based on the balloon manager parameters
	m_OctreeCenter = balloonRowFrontCenter +
		(BalloonRowsForwardVector * (((balloonRowCount - 1) * balloonRowSpacing) / 2.0f)) +
		(glm::cross(BalloonRowsForwardVector, BalloonRowsRightVector) * -1.0f * (balloonMaxHeight / 2.0f));
	// Calculate the necessary size of the octree based on the balloon manager parameters
	m_OctreeHalfWidth = max(max(balloonRowLength / 2.0f, balloonMaxHeight / 2.0f), ((balloonRowCount - 1) * balloonRowSpacing) / 2.0f);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	// Have dart follow camera if it has not been thrown and handle flight if it has been
	if (!m_Dart->m_bThrown)
	{
		m_Dart->FollowCamera(m_pCameraMngr->GetPosition() + m_pCameraMngr->GetForward() * 1.5f);
	}
	else
	{
		m_Dart->HandleFlight();
	}

	// Respawn dart if below y = 0
	if (m_Dart->GetPosition().y < 0)
	{
		m_Dart->Respawn();
	}

	// Recreate the octree as the balloons have moved
	SafeDelete(m_pRoot);
	if (m_uOctantLevels > 0)
		m_pRoot = new MyOctree(m_uOctantLevels, m_OctreeHalfWidth, m_OctreeCenter);
	
	

	//Update Entity Manager
	// If octree exists, use the octree's collision check
	// Otherwise use the entity manager's collision check
	m_pEntityMngr->ClearCollisions();

	if (m_pRoot == nullptr) {
		m_pEntityMngr->Update();
	}
	else {
		// Check collisions
		m_pRoot->CheckCollisions();
	}

	if (m_BalloonMngr != nullptr)
		m_BalloonMngr->Update();

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

	if (m_BalloonMngr != nullptr)
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