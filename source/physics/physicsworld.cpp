#include "physicsworld.h"

#include "BulletDynamics/btBulletDynamicsCommon.h"
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"

#include <iostream>

PhysicsWorld::PhysicsWorld(){};
PhysicsWorld::~PhysicsWorld(){};


void PhysicsWorld::checkForCollisions() {
	btDispatcher *dispatcher = m_world->getDispatcher();
	int numManifolds = dispatcher->getNumManifolds();
	

	std::cout << "numManifolds " << numManifolds << std::endl;
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold =  dispatcher->getManifoldByIndexInternal(i);

		// unnecessary (?) and not working
		// btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		// btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		std::cout << "numContacts " << numContacts << std::endl;
		for (int j=0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			
			
			// access our logic representation
			// contactManifold->getBody0()->getUserPointer();


			if (pt.getDistance() < 0.f)
			{
				// probably, we should "break" here for performance

				// additional infos
				/*const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;*/
				
				std::cout << "collision detected" << std::endl;
			}
		}
	}
}

void PhysicsWorld::testBullet() {
	// can be used to used to filter manually potential collision partners
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// potential bottleneck
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;


	// the world
	m_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	m_world->setGravity(btVector3(0, -0, 0));



	// the real stuff

	// shapes
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	// radius: 1 meter
	btCollisionShape* fallingShape = new btBoxShape(btVector3(1, 0.5, 2));

	// rigids
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));

	// the same btRigidBodyConstructionInfo can be used to instantiate 1000 rigidBodies
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);


	// set this to our logic representation
	// groundRigidBody.setUserPointer();

	m_world->addRigidBody(groundRigidBody);


	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallingShape->calculateLocalInertia(mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo fallingRigidBodyCI(mass, fallMotionState, fallingShape, fallInertia);
	btRigidBody* fallingRigidBody = new btRigidBody(fallingRigidBodyCI);
	fallingRigidBody->setLinearVelocity(btVector3(1, 0, 1));
	
	m_world->addRigidBody(fallingRigidBody);

	

	for (int i = 0; i < 3000; i++) {
		m_world->stepSimulation(1 / 60.f, 10);
		
		// holds position (center of object) and orientation
		btTransform trans;
		fallingRigidBody->getMotionState()->getWorldTransform(trans);
		checkForCollisions();
		std::cout << "box Y: " << trans.getOrigin().getY()
				  << " X: " << trans.getOrigin().getX()
				  << " Z: " << trans.getOrigin().getZ()
				  << std::endl;
	}

	m_world->removeRigidBody(fallingRigidBody);
	delete fallingRigidBody->getMotionState();
	delete fallingRigidBody;

	m_world->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;


	delete fallingShape;
	delete groundShape;


	// clean the world
	delete m_world;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;

}


