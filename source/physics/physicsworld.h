#pragma once

// TODO
// why do i need the " ../" , really shouldn't be necessary
#include "../forwarddeclarations.h"

// TODO
// add namespace

class PhysicsWorld
{
	

public:
	PhysicsWorld();

	virtual ~PhysicsWorld();

	void testBullet();
	void checkForCollisions();
	btDiscreteDynamicsWorld *m_world;
};