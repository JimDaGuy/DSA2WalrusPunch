#include "MyOctree.h"

MyOctree::MyOctree(uint a_octreeLevels)
{
	octreeLevels = a_octreeLevels;
	// Initial octant will be 70x70x70
	octreeHalfWidth = vector3(35.0f);
	// Centered at 0,0,0
	octreeCenter = vector3(0.0f);
	// Arbitrary int for threshold of subdivision
	subdivisionThreshold = 8;

	mMeshMngr = MeshManager::GetInstance();
	mEntityMngr = MyEntityManager::GetInstance();

	// Create initial/largest/main Octant
	root = new MyOctant(1, subdivisionThreshold, octreeHalfWidth, octreeCenter);
	// Set root octant's entity list to the list of all entities
	uint entityCount = mEntityMngr->GetEntityCount();
	for (uint i = 0; i < entityCount; i++) {
		root->AddEntityIndexToList(i);
	}

	// Subdivide root octant however many times neccesary
	CreateSubdivisions(root);
}

MyOctree::~MyOctree()
{
	SafeDelete(root);
	mMeshMngr = nullptr;
	mEntityMngr = nullptr;
}

void MyOctree::CreateSubdivisions(MyOctant* a_parent)
{
	// Recursive function to subdivide octants as neccesary
	uint currentLevel = a_parent->GetLevel();

	// Return if octree has been subdivided enough
	if (currentLevel >= octreeLevels)
		return;
	
	// Return if the octant has too few entities to subdivide further
	if (a_parent->GetEntityCount() < a_parent->GetSubdivisionThreshold())
		return;

	// Return if the octant has already been subdivided
	if (a_parent->GetSubdivided())
		return;

	vector3 parentHalfWidth = a_parent->GetHalfWidth();
	vector3 parentCenter = a_parent->GetCenter();
	std::vector<uint> parentEntityIndexList = a_parent->GetEntityIndexList();
	uint parentEntityCount = a_parent->GetEntityCount();

	// Create octant, set center, half width, subdivision level, parent, entity list children
	vector3 oct1Center = parentCenter;
	oct1Center.x += parentHalfWidth.x / 2;
	oct1Center.y += parentHalfWidth.y / 2;
	oct1Center.z += parentHalfWidth.z / 2;
	MyOctant* oct1 = new MyOctant(currentLevel + 1, a_parent->GetSubdivisionThreshold(), parentHalfWidth / 2, oct1Center);
	// Add child octant to the parent's list
	a_parent->AddSubdivision(oct1);

	// Octant #2
	vector3 oct2Center = parentCenter;
	oct2Center.x -= parentHalfWidth.x / 2;
	oct2Center.y += parentHalfWidth.y / 2;
	oct2Center.z += parentHalfWidth.z / 2;
	MyOctant* oct2 = new MyOctant(currentLevel + 1, a_parent->GetSubdivisionThreshold(), parentHalfWidth / 2, oct2Center);
	a_parent->AddSubdivision(oct2);

	// Octant #3
	vector3 oct3Center = parentCenter;
	oct3Center.x -= parentHalfWidth.x / 2;
	oct3Center.y -= parentHalfWidth.y / 2;
	oct3Center.z += parentHalfWidth.z / 2;
	MyOctant* oct3 = new MyOctant(currentLevel + 1, a_parent->GetSubdivisionThreshold(), parentHalfWidth / 2, oct3Center);
	a_parent->AddSubdivision(oct3);

	// Octant #4
	vector3 oct4Center = parentCenter;
	oct4Center.x += parentHalfWidth.x / 2;
	oct4Center.y -= parentHalfWidth.y / 2;
	oct4Center.z += parentHalfWidth.z / 2;
	MyOctant* oct4 = new MyOctant(currentLevel + 1, a_parent->GetSubdivisionThreshold(), parentHalfWidth / 2, oct4Center);
	a_parent->AddSubdivision(oct4);

	// Octant #5
	vector3 oct5Center = parentCenter;
	oct5Center.x += parentHalfWidth.x / 2;
	oct5Center.y += parentHalfWidth.y / 2;
	oct5Center.z -= parentHalfWidth.z / 2;
	MyOctant* oct5 = new MyOctant(currentLevel + 1, a_parent->GetSubdivisionThreshold(), parentHalfWidth / 2, oct5Center);
	a_parent->AddSubdivision(oct5);

	// Octant #6
	vector3 oct6Center = parentCenter;
	oct6Center.x -= parentHalfWidth.x / 2;
	oct6Center.y += parentHalfWidth.y / 2;
	oct6Center.z -= parentHalfWidth.z / 2;
	MyOctant* oct6 = new MyOctant(currentLevel + 1, a_parent->GetSubdivisionThreshold(), parentHalfWidth / 2, oct6Center);
	a_parent->AddSubdivision(oct6);

	// Octant #7
	vector3 oct7Center = parentCenter;
	oct7Center.x -= parentHalfWidth.x / 2;
	oct7Center.y -= parentHalfWidth.y / 2;
	oct7Center.z -= parentHalfWidth.z / 2;
	MyOctant* oct7 = new MyOctant(currentLevel + 1, a_parent->GetSubdivisionThreshold(), parentHalfWidth / 2, oct7Center);
	a_parent->AddSubdivision(oct7);

	// Octant #8
	vector3 oct8Center = parentCenter;
	oct8Center.x += parentHalfWidth.x / 2;
	oct8Center.y -= parentHalfWidth.y / 2;
	oct8Center.z -= parentHalfWidth.z / 2;
	MyOctant* oct8 = new MyOctant(currentLevel + 1, a_parent->GetSubdivisionThreshold(), parentHalfWidth / 2, oct8Center);
	a_parent->AddSubdivision(oct8);

	// Check which entities from the parent's entityList are colliding with the current octant, add those to the entity list
	// Check collision with every entity in the entity list
	// Append each item found to be colliding to the entityList for the child octant
	for (uint i = 0; i < parentEntityCount; i++) {
		// Grab entity from the entity list
		uint currentEntityIndex = parentEntityIndexList[i];

		// Check if entity is colliding with the octant
		bool oct1EntColliding = oct1->IsCollidingWithEntity(currentEntityIndex);
		bool oct2EntColliding = oct2->IsCollidingWithEntity(currentEntityIndex);
		bool oct3EntColliding = oct3->IsCollidingWithEntity(currentEntityIndex);
		bool oct4EntColliding = oct4->IsCollidingWithEntity(currentEntityIndex);
		bool oct5EntColliding = oct5->IsCollidingWithEntity(currentEntityIndex);
		bool oct6EntColliding = oct6->IsCollidingWithEntity(currentEntityIndex);
		bool oct7EntColliding = oct7->IsCollidingWithEntity(currentEntityIndex);
		bool oct8EntColliding = oct8->IsCollidingWithEntity(currentEntityIndex);

		// If entity is colliding with the octant, add it to the Octant's entity list
		if (oct1EntColliding) {
			oct1->AddEntityIndexToList(currentEntityIndex);
		}
		if (oct2EntColliding) {
			oct2->AddEntityIndexToList(currentEntityIndex);
		}
		if (oct3EntColliding) {
			oct3->AddEntityIndexToList(currentEntityIndex);
		}
		if (oct4EntColliding) {
			oct4->AddEntityIndexToList(currentEntityIndex);
		}
		if (oct5EntColliding) {
			oct5->AddEntityIndexToList(currentEntityIndex);
		}
		if (oct6EntColliding) {
			oct6->AddEntityIndexToList(currentEntityIndex);
		}
		if (oct7EntColliding) {
			oct7->AddEntityIndexToList(currentEntityIndex);
		}
		if (oct8EntColliding) {
			oct8->AddEntityIndexToList(currentEntityIndex);
		}
	}

	// Create subdivisions for the new octants if neccesary
	CreateSubdivisions(oct1);
	CreateSubdivisions(oct2);
	CreateSubdivisions(oct3);
	CreateSubdivisions(oct4);
	CreateSubdivisions(oct5);
	CreateSubdivisions(oct6);
	CreateSubdivisions(oct7);
	CreateSubdivisions(oct8);

	// Set parent node subdivided property
	a_parent->SetSubdivided(true);
}

void MyOctree::Display(vector3 a_v3Color)
{
	root->ShowWire(a_v3Color);
}

void MyOctree::CheckCollisions()
{
	root->CheckCollision();
}