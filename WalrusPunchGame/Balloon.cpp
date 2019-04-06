#include "Balloon.h"

int Balloon::IdIterator = -1;

Balloon::Balloon(vector3 a_color)
{
	++IdIterator;

	color = a_color;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	std::string idPrefix = "balloon";
	m_uniqueID = idPrefix + std::to_string(IdIterator);
	m_pEntityMngr->AddEntity("Balloons\\IronMan.obj", m_uniqueID);

	vector3 v3Position = vector3(glm::sphericalRand(34.0f));
	matrix4 m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);
}


Balloon::~Balloon()
{
	m_pEntityMngr->RemoveEntity(m_pEntityMngr->GetEntityIndex(m_uniqueID));
	m_pMeshMngr = nullptr;
	m_pEntityMngr = nullptr;
}

void Balloon::MoveTo(vector3 position)
{
	matrix4 m4Position = glm::translate(position);
	GetEntity()->SetModelMatrix(m4Position);
}

// Getters
MyEntity* Balloon::GetEntity()
{
	return m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex(m_uniqueID));
}