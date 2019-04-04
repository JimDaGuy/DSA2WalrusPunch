#pragma once

#include "Definitions.h"
#include "MyEntityManager.h"
#include "MyOctant.h"
using namespace Simplex;

class MyOctree
{
public:
	MyOctree(uint a_octreeLevels = 1);
	~MyOctree();
	void Display(vector3 a_v3Color);
	void CheckCollisions();
private:
	void CreateSubdivisions(MyOctant* a_parent);
	MeshManager* mMeshMngr = nullptr;//Mesh Manager singleton
	MyEntityManager* mEntityMngr = nullptr; //Entity Manager Singleton
	uint octreeLevels;
	uint subdivisionThreshold;
	vector3 octreeHalfWidth;
	vector3 octreeCenter;
	MyOctant* root;
};

