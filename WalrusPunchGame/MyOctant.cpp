#include "MyOctant.h"

MyOctant::MyOctant(uint a_level, uint a_subdivisionThreshold, vector3 a_halfWidth, vector3 a_center)
{
	level = a_level;
	subdivisionThreshold = a_subdivisionThreshold;
	halfWidth = a_halfWidth;
	center = a_center;
	maxVec = center + halfWidth;
	minVec = center - halfWidth;
	subdivided = false;

	mMeshMngr = MeshManager::GetInstance();
	mEntityMngr = MyEntityManager::GetInstance();
}

MyOctant::~MyOctant()
{
	for (uint i = 0; i < subdivisions.size(); i++) {
		SafeDelete(subdivisions[i]);
	}
	subdivisions.clear();
	entityIndexList.clear();
	mMeshMngr = nullptr;
	mEntityMngr = nullptr;
}

// Getters
uint MyOctant::GetLevel()
{
	return level;
}
uint MyOctant::GetSubdivisionThreshold()
{
	return subdivisionThreshold;
}
vector3 MyOctant::GetHalfWidth()
{
	return halfWidth;
}
vector3 MyOctant::GetCenter()
{
	return center;
}
bool MyOctant::GetSubdivided()
{
	return subdivided;
}
std::vector<MyOctant*> MyOctant::GetSubdivisions() 
{
	return subdivisions;
}
std::vector<uint> MyOctant::GetEntityIndexList()
{
	return entityIndexList;
}
uint MyOctant::GetEntityCount()
{
	return entityIndexList.size();
}
bool MyOctant::IsCollidingWithEntity(uint a_entityIndex)
{
	MyEntity* entity = mEntityMngr->GetEntity(a_entityIndex);
	MyRigidBody* entityRB = entity->GetRigidBody();
	vector3 entityMinG = entityRB->GetMinGlobal();
	vector3 entityMaxG = entityRB->GetMaxGlobal();

	bool colliding = true;

	if (maxVec.x < entityMinG.x) //this to the right of other
		colliding = false;
	if (minVec.x > entityMaxG.x) //this to the left of other
		colliding = false;

	if (maxVec.y < entityMinG.y) //this below of other
		colliding = false;
	if (minVec.y > entityMaxG.y) //this above of other
		colliding = false;

	if (maxVec.z < entityMinG.z) //this behind of other
		colliding = false;
	if (minVec.z > entityMaxG.z) //this in front of other
		colliding = false;

	return colliding;
}
void MyOctant::CheckCollision()
{
	// If the Octant is not a bottom level octant
	// Call this function on the child octants
	if (subdivisions.size() > 0) {
		// std::cout << "Lvl: " << level << " SD: true NumSubdivisions:" << subdivisions.size() << std::endl;
		// Iterate through subdivisions of current Octant
		for (uint i = 0; i < subdivisions.size(); i++) {
			subdivisions[i]->CheckCollision();
		}
	}
	// If the Octant is not subdivided,
	// Check collisions with a child entities in the entity list
	else {
		// std::cout << "Lvl: " << level << " SD: false NumEntities:" << entityIndexList.size() << std::endl;
		uint entityIndexListSize = entityIndexList.size();
		if (entityIndexListSize < 2) { return; }
		for (uint j = 0; j < entityIndexListSize - 1; j++) {
			for (uint k = j + 1; k < entityIndexListSize; k++) {
				MyEntity* entity1 = mEntityMngr->GetEntity(entityIndexList[j]);
				MyEntity* entity2 = mEntityMngr->GetEntity(entityIndexList[k]);
				entity1->IsColliding(entity2);
			}
		}
	}
	
}

// Setters
void MyOctant::SetSubdivided(bool a_subdivision)
{
	if (a_subdivision) {
		entityIndexList.clear();
	}
	subdivided = a_subdivision;
}
void MyOctant::AddSubdivision(MyOctant* a_oct)
{
	subdivisions.push_back(a_oct);
}
void MyOctant::AddEntityIndexToList(uint a_entityIndex) {
	entityIndexList.push_back(a_entityIndex);
}
void MyOctant::ShowWire(vector3 a_v3Color)
{
	matrix4 wireCubeMatrix = IDENTITY_M4;
	wireCubeMatrix = glm::translate(wireCubeMatrix, center);
	wireCubeMatrix = glm::scale(wireCubeMatrix, halfWidth * 2);
	mMeshMngr->AddWireCubeToRenderList(wireCubeMatrix, a_v3Color, RENDER_WIRE);

	for (uint i = 0; i < subdivisions.size(); i++) {
		subdivisions[i]->ShowWire(a_v3Color);
	}
}