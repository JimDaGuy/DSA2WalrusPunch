#pragma once
#include <chrono>

#include "Definitions.h"
#include "Balloon.h"
using namespace Simplex;
using namespace std::chrono;

class BalloonManager
{
typedef Balloon* BalloonPointer; //Balloon Pointer
BalloonPointer* balloonList = nullptr;
uint balloonCount = 0;
public:
	BalloonManager
	(
		sf::Sound a_soundBalloon,
		uint a_lines = 4,
		vector3 a_lineCenter = vector3(0.0f),
		vector3 a_forwardVec = -AXIS_Z,
		vector3 a_rightwardVec = AXIS_X,
		float a_spaceBetweenLines = 3.0f,
		float a_lineLength = 15.0f,
		float a_maxHeight = 7.5f,
		uint a_balloonMax = 10,
		uint a_msPerBalloonSpawn = 500,
		uint a_balloonsPerSpawn = 1
	);
	void Update(uint a_deltaMS);
	void DisplayLines(vector3 a_color);
	void CreateBalloon(Balloon::BalloonColor a_color, vector3 a_position, uint a_row);
	void DestroyBalloon(uint a_balloonIndex);
	uint WeightedRandom();
	~BalloonManager();
	uint score = 0;
	sf::Sound soundBalloon; //sound effect
private:
	// Number of lines to be generated
	uint lines;
	// Center position of first line
	vector3 lineCenter;
	// Forward vector of lines
	vector3 forwardVec;
	// Rightward vector of lines
	vector3 rightwardVec;
	// Space between lines
	float spaceBetweenLines;
	// Length of lines (From end to end)
	float lineLength;
	// Max height a balloon can reach before it is despawned
	float maxHeight;
	// Max balloons allowed at a given time
	uint balloonMax;
	// Milliseconds between each balloon spawn (if the limit has not been hit)
	uint msPerBalloonSpawn;
	// Timestamp is ms from last balloon spawn
	uint balloonsPerSpawn;
	// Timestamp is ms from last balloon spawn
	uint lastBalloonSpawn;
	// Weights for random gen, highest to lowest
	std::vector<uint> weights = { 7, 4, 3, 1 };

	typedef MyRigidBody* PRigidBody; //Entity Pointer
	MeshManager* m_pMeshMngr = nullptr;//Mesh Manager singleton
	MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager Singleton
};

