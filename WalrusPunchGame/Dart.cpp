#include "Dart.h"

Dart::Dart(vector3 a_position)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_pEntityMngr->AddEntity("Dart\\Dart.obj", "Dart", 0);
	m_bThrown = false;

	m_v3Position = a_position;
	matrix4 m4Position = glm::translate(m_v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);
}

Dart::~Dart()
{


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
	return m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Dart"));
}

void Dart::Throw(vector3 a_v3Forward)
{
	if (!m_bThrown)
	{
		m_fThrowForce = .2f;
		m_v3Forward = a_v3Forward;
		m_bThrown = true;
	}
}

void Dart::HandleFlight()
{
	if (m_fThrowForce > 0)
	{
		m_fThrowForce -= .001f;
	}
	m_v3Position = m_v3Position + m_v3Forward * m_fThrowForce;
	m_v3Position = m_v3Position - vector3(0.0f, .05f, 0.0f);
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





