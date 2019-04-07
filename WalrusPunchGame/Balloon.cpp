#include "Balloon.h"

int Balloon::IdIterator = -1;

Balloon::Balloon(vector3 a_position, BalloonColor a_color)
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
		m_pEntityMngr->AddEntity("Balloons\\Red.obj", m_uniqueID);
		break;
	case Blue:
		m_pEntityMngr->AddEntity("Balloons\\Blue.obj", m_uniqueID);
		break;
	default:
		m_pEntityMngr->AddEntity("Balloons\\Red.obj", m_uniqueID);
	}

	vector3 v3Position = a_position;
	matrix4 m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);

	// Keeps track of how far left and right of original position the balloon is
	horizontalOffset = 0;
	// Change for how far left and right balloons float
	leftRightDistance = 1.0f;
	// Set to random (t/f) for more movement variety
	movingLeft = false;
	// Speed balloons move back and forth
	horizontalSpeed = .01f;
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

float Balloon::getHorizontalOffset()
{
	return horizontalOffset;
}

void Balloon::setHorizontalOffset(float offset)
{
	horizontalOffset += offset;
}

float Balloon::getLeftRightDistance()
{
	return leftRightDistance;
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
