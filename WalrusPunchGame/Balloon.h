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
		Blue,
		Green,
		Gold
	};
	Balloon(BalloonColor a_color = Red);
	Balloon(vector3 a_position, BalloonColor a_color, uint a_row);
	void Swap(Balloon& other);
	~Balloon();
	void MoveTo(vector3 position);
	MyEntity* GetEntity();
	vector3 GetPosition();
	float getLeftRightSpeed();
	bool getMovingLeft();
	vector3 getVelocity();
	void setVelocity(vector3 a_velocity);
	vector3 getBounce();
	void setBounce(vector3 a_bounceVelocity);
	void setMovingLeft(bool left);
	float getHorizontalSpeed();
	BalloonColor color;
private:
	static int IdIterator;
	MeshManager* m_pMeshMngr = nullptr;//Mesh Manager singleton
	MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager Singleton
	std::string m_uniqueID;
	float leftRightMaxSpeed;
	bool movingLeft;
	float horizontalSpeed;
	vector3 m_bounceVelocity;
	vector3 m_velocity;
};

#endif //__BALLOON_H_