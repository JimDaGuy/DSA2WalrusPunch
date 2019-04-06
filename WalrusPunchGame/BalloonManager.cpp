#include "BalloonManager.h"

BalloonManager::BalloonManager
(
	uint a_lines,
	vector3 a_lineCenter,
	vector3 a_forwardVec,
	vector3 a_rightwardVec,
	float a_spaceBetweenLines,
	float a_lineLength,
	float a_maxHeight,
	uint a_balloonMax,
	uint a_msPerBalloonSpawn
)
{
	// Set values
	lines = a_lines;
	lineCenter = a_lineCenter;
	forwardVec = a_forwardVec;
	rightwardVec = a_rightwardVec;
	spaceBetweenLines = a_spaceBetweenLines;
	lineLength = a_lineLength;
	maxHeight = a_maxHeight;
	balloonMax = a_balloonMax;
	msPerBalloonSpawn = a_msPerBalloonSpawn;
	// msSinceLastBalloonSpawn = 0;

	lastBalloonSpawn = static_cast <uint>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

	// Get references to the entity and mesh managers
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
}

BalloonManager::~BalloonManager()
{
	m_pMeshMngr = nullptr;
	m_pEntityMngr = nullptr;
	delete[] balloonList;
}

void BalloonManager::Update()
{
	// Iterate through list of balloons
	for (uint i = 0; i < balloonCount; i++) {
		BalloonPointer current = balloonList[i];
		vector3 currentPos = current->GetPosition();

		// Move the balloon upward
		currentPos.y += .02;

		// Move the balloon left and right


		// If any of the balloons are higher than the maxHeight,
		if (current->GetPosition().y > lineCenter.y + maxHeight) {
			// Delete the balloon and decrement the loop
			DestroyBalloon(i);
			i--;
			continue;
		}

		current->MoveTo(currentPos);
	}

	// Increment the msSinceLastBalloonSpawn
	uint currentTime = static_cast <uint>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

	// If msSinceLastBalloonSpawn is greater than the timer
	// And if the current number of balloons is less than the max
	if (currentTime >= msPerBalloonSpawn + lastBalloonSpawn && balloonCount < balloonMax) {
		// Generate position for new balloon
		uint lineNum = rand() % lines; // Random line num

		// Borrowed this line for generating floats within a range from a stack overflow post
		// https://stackoverflow.com/questions/686353/random-float-number-generation
		float linePosFloat = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / lineLength)) - (lineLength / 2.0f); // Random float along line

		// Start at first line center
		vector3 balloonPosition = lineCenter;
		// Push pos to the correct line
		vector3 forwardOffset = glm::normalize(forwardVec) * spaceBetweenLines * lineNum;
		balloonPosition += forwardOffset;
		// Push pos to correct spot on the line
		vector3 rightwardOffset = glm::normalize(rightwardVec) * linePosFloat;
		balloonPosition += rightwardOffset;

		// Create balloon
		CreateBalloon(Balloon::BalloonColor::Blue, balloonPosition);
		// Reset timer
		lastBalloonSpawn = currentTime;
	}
}

void BalloonManager::CheckCollisions()
{
}

void BalloonManager::DisplayLines(vector3 a_color)
{
	// Draw each line
	for (uint i = 0; i < lines; i++) {
		vector3 currLineCenter = lineCenter + glm::normalize(forwardVec) * spaceBetweenLines * i;
		matrix4 centerMatrix = IDENTITY_M4;
		centerMatrix = glm::translate(centerMatrix, currLineCenter);
		m_pMeshMngr->AddLineToRenderList(centerMatrix, glm::normalize(rightwardVec) * (-lineLength / 2), glm::normalize(rightwardVec) * (lineLength / 2), a_color, a_color);
	}
}

void BalloonManager::CreateBalloon(Balloon::BalloonColor a_color, vector3 a_position)
{
	BalloonPointer bTemp = new Balloon(a_position, a_color);
	MyEntity* bTempEntity = bTemp->GetEntity();

	if (bTempEntity->IsInitialized()) {
		BalloonPointer* bTempArray = new BalloonPointer[balloonCount + 1];

		uint uCount = 0;
		for (uint i = 0; i < balloonCount; ++i) {
			bTempArray[uCount] = balloonList[i];
			++uCount;
		}
		bTempArray[uCount] = bTemp;

		if (balloonList) {
			delete[] balloonList;
		}

		balloonList = bTempArray;
		balloonCount++;
	}
}

void BalloonManager::DestroyBalloon(uint a_balloonIndex)
{
	if (balloonCount <= 0)
		return;

	if (a_balloonIndex >= balloonCount)
		a_balloonIndex = balloonCount - 1;

	if (a_balloonIndex != balloonCount - 1)
		std::swap(balloonList[a_balloonIndex], balloonList[balloonCount - 1]);

	BalloonPointer* bTempArray = new BalloonPointer[balloonCount - 1];

	for (uint i = 0; i < balloonCount - 1; ++i) {
		bTempArray[i] = balloonList[i];
	}

	if (balloonList) {
		delete[] balloonList;
	}

	balloonList = bTempArray;
	--balloonCount;
}