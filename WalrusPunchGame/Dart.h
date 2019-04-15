#pragma once
#include "Definitions.h"
#include "MyEntityManager.h"
using namespace Simplex;

class Dart
{
public:

	Dart(vector3 a_position);
	~Dart();
	void FollowCamera(vector3 a_position);
	void MoveTo(vector3 a_position);
	MyEntity* GetEntity();
	bool m_bThrown;
	void Throw(vector3 a_v3Forward);
	float m_fSpeed;
	void HandleFlight();
	void Respawn();
	vector3 GetPosition();
	float m_fThrowForce;


private:

	MeshManager* m_pMeshMngr = nullptr;
	MyEntityManager* m_pEntityMngr = nullptr;
	vector3 m_v3Position;
	vector3 m_v3Forward;
};

