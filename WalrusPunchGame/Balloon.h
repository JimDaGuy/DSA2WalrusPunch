#pragma once
#ifndef __BALLOON_H_
#define __BALLOON_H_
#include "Definitions.h"
#include "MyEntityManager.h"
using namespace Simplex;

class Balloon
{
public:
	Balloon(vector3 a_color = C_RED);
	~Balloon();
	void MoveTo(vector3 position);
private:
	static int IdIterator;
	MyEntity* GetEntity();
	MeshManager* m_pMeshMngr = nullptr;//Mesh Manager singleton
	MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager Singleton
	std::string m_uniqueID;
	vector3 color;
};

#endif //__BALLOON_H_