#include "Dart.h"

int Dart::IdIterator = -1;


Dart::Dart(vector3 a_position)
{
	++IdIterator;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	std::string idPrefix = "Dart";
	m_uniqueID = idPrefix + std::to_string(IdIterator);

	m_pEntityMngr->AddEntity("Dart\\Dart.obj", m_uniqueID, -1);
	m_bThrown = false;

	m_v3Position = a_position;
	matrix4 m4Position = glm::translate(m_v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);
}

Dart::~Dart()
{
	m_pEntityMngr->RemoveEntity(m_pEntityMngr->GetEntityIndex(m_uniqueID));
	m_pMeshMngr = nullptr;
	m_pEntityMngr = nullptr;
}

void Dart::FollowCamera(vector3 a_position)
{
	m_v3Position = a_position;
	MoveTo(a_position);
}

void Dart::MoveTo(vector3 a_position)
{
	matrix4 m4Position = glm::translate(a_position);
	GetEntity()->SetModelMatrix(m4Position);
	
}

MyEntity * Dart::GetEntity()
{
	return m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex(m_uniqueID));
}

void Dart::Throw(vector3 a_v3Forward, float a_fThrowForce)
{
	if (!m_bThrown)
	{
		m_fThrowForce = a_fThrowForce;
		m_v3Velocity = a_v3Forward * m_fThrowForce;
		m_bThrown = true;
	}
}

void Dart::HandleFlight(uint a_deltaMS)
{
	float speedMultiplier = a_deltaMS / 10.0f;

	m_v3Velocity.y -= .0015f * speedMultiplier;

	m_v3Position = m_v3Position + m_v3Velocity * speedMultiplier;
	MoveTo(m_v3Position);
}

void Dart::Respawn()
{
		m_bThrown = false;
}

vector3 Dart::GetPosition()
{
	return m_v3Position;
}




