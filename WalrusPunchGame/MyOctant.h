#pragma once
#include "Definitions.h"
#include "MyEntityManager.h"
using namespace Simplex;

class MyOctant
{
public:
	MyOctant(uint a_level = 1, uint a_subdivisionThreshold = 8, vector3 a_halfWidth = vector3(35.0f), vector3 a_center = vector3(0.0f));
	~MyOctant();
	uint GetLevel();
	uint GetSubdivisionThreshold();
	vector3 GetHalfWidth();
	vector3 GetCenter();
	bool GetSubdivided();
	std::vector<MyOctant*> GetSubdivisions();
	std::vector<uint> GetEntityIndexList();
	uint GetEntityCount();
	bool IsCollidingWithEntity(uint a_entityIndex);
	void CheckCollision();
	void SetSubdivided(bool a_subdivsion);
	void AddSubdivision(MyOctant* a_oct);
	void AddEntityIndexToList(uint a_entityIndex);
	void ShowWire(vector3 a_v3Color);
private:
	MeshManager* mMeshMngr = nullptr;//Mesh Manager singleton
	MyEntityManager* mEntityMngr = nullptr; //Entity Manager Singleton
	// Eventually use this ID to decide which octant to display
	uint level;
	uint subdivisionThreshold;
	vector3 halfWidth;
	vector3 center;
	vector3 maxVec;
	vector3 minVec;
	bool subdivided;
	std::vector<MyOctant*> subdivisions;
	std::vector<uint> entityIndexList;
};

