#include "Balloon.h"

int Balloon::IdIterator = -1;

Balloon::Balloon(vector3 a_position, BalloonColor a_color, uint a_row)
{
	++IdIterator;

	color = a_color;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	std::string idPrefix = "balloon";
	m_uniqueID = idPrefix + std::to_string(IdIterator);

	switch (a_color)
	{
	case Red:
		m_pEntityMngr->AddEntity("Balloons\\Red.obj", m_uniqueID, a_row);
		break;
	case Blue:
		m_pEntityMngr->AddEntity("Balloons\\Blue.obj", m_uniqueID, a_row);
		break;
	case Green:
		m_pEntityMngr->AddEntity("Balloons\\Green.obj", m_uniqueID, a_row);
		break;
	case Gold:
		m_pEntityMngr->AddEntity("Balloons\\Gold.obj", m_uniqueID, a_row);
		break;
	default:
		m_pEntityMngr->AddEntity("Balloons\\Red.obj", m_uniqueID, a_row);
	}

	vector3 v3Position = a_position;
	matrix4 m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);

	// Change how far left and right balloons float
	leftRightMaxSpeed = 0.02f;
	// Set to random (t/f) for more movement variety
	movingLeft = false;
	// Speed balloons move back and forth
	horizontalSpeed = 1.0f;
	m_velocity = vector3(0.0f, 0.02f, 0.0f);
	m_bounceVelocity = ZERO_V3;
}

void Balloon::Swap(Balloon & other)
{
	std::swap(m_uniqueID, other.m_uniqueID);
	std::swap(color, other.color);
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

vector3 Balloon::GetPosition()
{
	return GetEntity()->GetRigidBody()->GetCenterGlobal();
}

float Balloon::getLeftRightSpeed()
{
	return leftRightMaxSpeed;
}

bool Balloon::getMovingLeft()
{
	return movingLeft;
}

void Balloon::setMovingLeft(bool left)
{
	movingLeft = left;
}

float Balloon::getHorizontalSpeed()
{
	return horizontalSpeed;
}

vector3 Balloon::getVelocity()
{
	return m_velocity;
}

void Balloon::setVelocity(vector3 a_velocity)
{
	m_velocity = a_velocity;
}

vector3 Balloon::getBounce()
{
	return m_bounceVelocity;
}

void Balloon::setBounce(vector3 a_bounceVelocity)
{
	m_bounceVelocity = a_bounceVelocity;
}