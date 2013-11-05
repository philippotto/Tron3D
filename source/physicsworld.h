#pragma once

#include "forwarddeclarations.h"

class PhysicsWorld
{
	

public:
	PhysicsWorld();

	virtual ~PhysicsWorld();

	void testBullet();
	void checkForCollisions();
	btDiscreteDynamicsWorld *m_world;
};