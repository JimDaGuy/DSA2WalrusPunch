#pragma once
#ifndef __BALLOON_H_
#define __BALLOON_H_
#include "Definitions.h"
#include "MyEntityManager.h"
using namespace Simplex;

class Balloon
{
public:
	enum BalloonColor
	{
		Red,
		Blue
	};
	Balloon(BalloonColor a_color = Red);
	~Balloon();
	void MoveTo(vector3 position);
private:
	static int IdIterator;
	MyEntity* GetEntity();
	MeshManager* m_pMeshMngr = nullptr;//Mesh Manager singleton
	MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager Singleton
	std::string m_uniqueID;
	BalloonColor color;
};

#endif //__BALLOON_H_