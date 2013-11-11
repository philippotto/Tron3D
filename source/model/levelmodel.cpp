#include "levelmodel.h"

#include <math.h>

#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"


using namespace troen;


LevelModel::LevelModel()
{
	m_rigidBodies = std::make_shared<std::vector<btRigidBody>>();

	//

	// for now, we create just the ground (and some basic objects for testing); later, other gimmicks could be placed here

	// shapes
	btStaticPlaneShape *groundShape = new btStaticPlaneShape(btVector3(0, 0, 1), 0);
	// radius: 1 meter
	btBoxShape *fallingShape = new btBoxShape(btVector3(1, 0.5, 2));

	// rigids
	// TODO: convert to shared_ptr
	btDefaultMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));


	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(btScalar(0), groundMotionState, groundShape, btVector3(0, 0, 0));

	// the same btRigidBodyConstructionInfo can be used to instantiate 1000 rigidBodies
	btRigidBody groundRigidBody(groundRigidBodyCI);


	// set this to our logic representation
	// groundRigidBody.setUserPointer(...);

	m_rigidBodies->push_back(groundRigidBody);

	// TODO: convert to shared_ptr
	btDefaultMotionState *fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallingShape->calculateLocalInertia(mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo m_fallingRigidBodyCI(mass, fallMotionState, fallingShape, fallInertia);
	
	btRigidBody fallingRigidBody(m_fallingRigidBodyCI);
	fallingRigidBody.setLinearVelocity(btVector3(1, 0, 1));

	m_rigidBodies->push_back(fallingRigidBody);

}

std::shared_ptr<std::vector<btRigidBody>> LevelModel::getRigidBodies()
{
	return m_rigidBodies;
}