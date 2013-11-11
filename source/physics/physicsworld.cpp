#include "physicsworld.h"

#include "BulletDynamics/btBulletDynamicsCommon.h"
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"

#include <iostream>

using namespace troen::physics;

PhysicsWorld::PhysicsWorld(){};

PhysicsWorld::~PhysicsWorld(){


	m_world->removeRigidBody(m_fallingRigidBody);
	delete m_fallingRigidBody->getMotionState();
	delete m_fallingRigidBody;

	m_world->removeRigidBody(m_groundRigidBody);
	delete m_groundRigidBody->getMotionState();
	delete m_groundRigidBody;


	delete m_fallingShape;
	delete m_groundShape;


	// clean the world
	delete m_world;
	delete m_solver;
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_broadphase;

};


void PhysicsWorld::initialize() {


	initializeWorld();
	createLevel();
	addFence();
	stepSimulation();

}


void PhysicsWorld::initializeWorld() {
	// can be used to used to filter manually potential collision partners
	m_broadphase = new btDbvtBroadphase();

	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	// potential bottleneck
	m_solver = new btSequentialImpulseConstraintSolver;

	m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	// TODO: adjust gravity
	m_world->setGravity(btVector3(0, -0, 0));
}


void PhysicsWorld::createLevel() {
	// for now, we create just the ground (and some basic objects for testing); later, other gimmicks could be placed here

	// shapes
	m_groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	// radius: 1 meter
	m_fallingShape = new btBoxShape(btVector3(1, 0.5, 2));

	// rigids
	btDefaultMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, m_groundShape, btVector3(0, 0, 0));

	// the same btRigidBodyConstructionInfo can be used to instantiate 1000 rigidBodies
	m_groundRigidBody = new btRigidBody(groundRigidBodyCI);


	// set this to our logic representation
	// m_groundRigidBody.setUserPointer(...);

	m_world->addRigidBody(m_groundRigidBody);


	btDefaultMotionState *fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	m_fallingShape->calculateLocalInertia(mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo m_fallingRigidBodyCI(mass, fallMotionState, m_fallingShape, fallInertia);
	m_fallingRigidBody = new btRigidBody(m_fallingRigidBodyCI);
	m_fallingRigidBody->setLinearVelocity(btVector3(1, 0, 1));
	
	m_world->addRigidBody(m_fallingRigidBody);
}



void PhysicsWorld::stepSimulation() {

	// TODO: remove the loop and call this method in every (n-th ?) frame (and provide the elapsed time as a parameter)


	float elapsedTime = 1 / 60.f;

	for (int i = 0; i < 300; i++)
	{

		// TODO: mind the following constraint:
		// timeStep < maxSubSteps * fixedTimeStep
		// where the parameters are given as follows: stepSimulation(timeStep, maxSubSteps, fixedTimeStep)
		
		m_world->stepSimulation(elapsedTime, 10);
		
		// holds position (center of object) and orientation
		btTransform trans;
		m_fallingRigidBody->getMotionState()->getWorldTransform(trans);
		std::cout << "box Y: " << trans.getOrigin().getY()
				  << " X: " << trans.getOrigin().getX()
				  << " Z: " << trans.getOrigin().getZ()
				  << std::endl;

		checkForCollisions();

	}

}


// respectively: updateFence ?
void PhysicsWorld::addFence() {
	/*
	we want to have a concave shape (which will be updated every frame), so we got the following options:
	- btBvhTriangleMeshShape with btTriangleMesh or btTriangleIndexVertexArray as input (no difference in performance)
			"it is recommend to enable useQuantizedAabbCompression for better memory usage"
	- create independent boxes (respectively glue them together in a compoundShape)


	first approach will be: btBvhTriangleMeshShape with btTriangleMesh
	*/


	
	btTriangleMesh fenceMesh;
	/*
	the mesh will look this (I should study art)
	1 4                5 ...

	0		             2 3 ...
	*/
	
	int fenceHeight = 1;
	int fenceStep = 1;

	// TODO change initial value of X
	int currentX = 0;

	// mind the manipulation of i within the loop
	for (int i = 0; i < 10; ++i)
	{

		currentX += fenceStep;

		fenceMesh.addTriangle(
			btVector3(currentX, 0, 0),
			btVector3(currentX, fenceHeight, 0),
			btVector3(currentX + fenceStep, 0, 0),
			false
		);

		i++;

		currentX += fenceStep;

		fenceMesh.addTriangle(
			btVector3(currentX, 0, 0),
			btVector3(currentX - fenceStep, fenceHeight, 0),
			btVector3(currentX, fenceHeight, 0),
			false
		);
	}


	btBvhTriangleMeshShape *fenceShape = new btBvhTriangleMeshShape(&fenceMesh, false);


	btTransform trans;
	trans.setIdentity();      
	trans.setOrigin(btVector3(0, 7, 0));      
	btDefaultMotionState* motionState = new btDefaultMotionState(trans);
	


	// we set localInertia to a zero vector
	// maybe, the mass has to be 0 instead of 1 ?
	btRigidBody* fenceBody = new btRigidBody(1, motionState, fenceShape, btVector3(0, 0, 0));
	

	// do we have to adjust these parameters ?
	fenceBody->setContactProcessingThreshold(BT_LARGE_FLOAT);
	fenceBody->setCcdMotionThreshold(.5);
	fenceBody->setCcdSweptSphereRadius(0);
	
	m_world->addRigidBody(fenceBody);


}

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



void PhysicsWorld::addBike() {

}


void PhysicsWorld::addItemBox() {

}

// ...




