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
		vector3(0.0f, 8.0f, 20.0f), //Position
		vector3(0.0f, 8.0f, -1.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	//PlaySound("Data\\Audio\\elementary-wave-11.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	m_uObjects = 0;

	m_uOctantID = 0;
	m_uOctantLevels = 3; 

	m_lastTime = static_cast <uint>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()); 
	m_lastOctreeUpdateTime = 0;

	tent = new MyEntity("Tent\\Tent.obj", "Tent", -1);
	
	sRoute = m_pSystem->m_pFolder->GetFolderData();
	sRoute += m_pSystem->m_pFolder->GetFolderAudio();

	m_soundBGM.openFromFile(sRoute + "elementary-wave-11.ogg");
	m_soundBGM.play();
	m_soundBGM.setLoop(true);

	//Balloon sound effect
	m_soundBuffer.loadFromFile(sRoute + "Balloon.wav");
	m_soundBalloon.setBuffer(m_soundBuffer);

	//Dart sound effect
	m_soundBuffer2.loadFromFile(sRoute + "whoosh.wav");
	m_soundDart.setBuffer(m_soundBuffer2);

	InitBalloonManager();
	m_Darts.push_back(new Dart(ZERO_V3));
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
	uint msPerBalloonSpawn,
	uint balloonsPerSpawn
)
{
	SafeDelete(m_pRoot);
	SafeDelete(m_BalloonMngr);

	m_BalloonMngr = new BalloonManager(
		m_soundBalloon,
		balloonRowCount, // number of rows
		balloonRowFrontCenter, // center of rows
		BalloonRowsForwardVector, // forward vector
		BalloonRowsRightVector, // right vector
		balloonRowSpacing, // spacing between rows
		balloonRowLength, // length of lines
		balloonMaxHeight, // maximum height a balloon can reach before despawning
		balloonMaxCount, // maximum number of balloons
		msPerBalloonSpawn, // ms per balloon spawn
		balloonsPerSpawn // number of balloons per spawn
	);

	// Reset octant levels, otherwise program will break
	m_uOctantLevels = 3;
	// Calculate the center of the octree based on the balloon manager parameters
	m_OctreeCenter = balloonRowFrontCenter +
		(BalloonRowsForwardVector * (((balloonRowCount - 1) * balloonRowSpacing) / 2.0f)) +
		(glm::cross(BalloonRowsForwardVector, BalloonRowsRightVector) * -1.0f * (balloonMaxHeight / 2.0f));
	// Calculate the necessary size of the octree based on the balloon manager parameters and add some padding in case balloons drift out
	m_OctreeHalfWidth = max(max(balloonRowLength / 2.0f, balloonMaxHeight / 2.0f), ((balloonRowCount - 1) * balloonRowSpacing) / 2.0f) + 5.0f;
}
void Application::Update(void)
{
	// Calculate how long since the last frame
	uint currentTime = static_cast <uint>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
	uint deltaMS = currentTime - m_lastTime;
	m_lastTime = currentTime;

	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	// Set the balloon count display
	m_uObjects = Balloon::BalloonCount;

	// Respawn the dart .5 seconds after it was thrown
	if ((m_Darts.size() < 1 || m_Darts.back()->m_bThrown) && currentTime - m_lastDartThrowTime > 500) {
		m_Darts.push_back(new Dart(ZERO_V3));
		m_lastDartThrowTime = currentTime;
	}

	// Have dart follow camera if it has not been thrown and handle flight if it has been
	if (m_Darts.size() > 0 && !m_Darts.back()->m_bThrown)
	{
		// If the mouse is held down, increase the dart force
		if (gui.m_bMousePressed[0] == true) {
			m_DartForce += deltaMS * .0005f;
			m_DartForce = min(m_DartForce, 0.4f);
		}
		m_Darts.back()->FollowCamera(m_pCameraMngr->GetPosition() + m_pCameraMngr->GetForward() * 1.5f + glm::cross(m_pCameraMngr->GetForward(), m_pCameraMngr->GetRightward())  * 0.75f);
	}

	for (std::vector<Dart*>::iterator it = m_Darts.begin(); it != m_Darts.end(); /* Iterating internally */) {
		if ((*it)->m_bThrown)
		{
			(*it)->HandleFlight(deltaMS);
		}

		// Respawn dart if below y = 0
		if ((*it)->GetPosition().y < -10.0f)
		{
			SafeDelete(*it);
			it = m_Darts.erase(it);
		}
		else {
			++it;
		}
	}

	// recreate the octree every 20 ms
	if (currentTime - m_lastOctreeUpdateTime > 20) {
		m_lastOctreeUpdateTime = currentTime;

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
	}

	if (m_BalloonMngr != nullptr)
		m_BalloonMngr->Update(deltaMS);

	tent->AddToRenderList();
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