#include "levelmodel.h"
//bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"

using namespace troen;

LevelModel::LevelModel()
{
	m_rigidBodies = std::make_shared<std::vector<btRigidBody>>();

	//
	// shapes
	btStaticPlaneShape *groundShape = new btStaticPlaneShape(btVector3(0, 0, 1), 0);

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


	addWalls();
	
}


void LevelModel::addWalls()
{
	// walls

	// TODO: get data from levelView ? or the other way round?
	// in levelView: wallRight = new osg::Box(osg::Vec3(levelSize / 2, 0, 50), 1, levelSize, 100);
	// center: levelSize/2, 0, 50
	// width: 1
	// length: levelSize
	// height: 100


	btVector3 planeNormal(-1, 0, 0);
	// planeConstant = 0 ? 

	// TODO grab the value from origin

	int levelSize = getLevelSize();
	

	btStaticPlaneShape *wallShape1 = new btStaticPlaneShape(planeNormal, 0);
	// TODO: convert to shared_ptr
	btDefaultMotionState *wallMotionState1 = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(levelSize / 2, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI1(btScalar(0), wallMotionState1, wallShape1, btVector3(0, 0, 0));
	btRigidBody wallRigidBody1(wallRigidBodyCI1);


	btStaticPlaneShape *wallShape2 = new btStaticPlaneShape(-planeNormal, 0);
	// TODO: convert to shared_ptr
	btDefaultMotionState *wallMotionState2 = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(- levelSize / 2, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI2(btScalar(0), wallMotionState2, wallShape2, btVector3(0, 0, 0));
	btRigidBody wallRigidBody2(wallRigidBodyCI2);


	planeNormal.setX(0); planeNormal.setY(-1);


	btStaticPlaneShape *wallShape3 = new btStaticPlaneShape(planeNormal, 0);
	// TODO: convert to shared_ptr
	btDefaultMotionState *wallMotionState3 = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, levelSize / 2, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI3(btScalar(0), wallMotionState3, wallShape3, btVector3(0, 0, 0));
	btRigidBody wallRigidBody3(wallRigidBodyCI3);

	btStaticPlaneShape *wallShape4 = new btStaticPlaneShape(-planeNormal, 0);
	// TODO: convert to shared_ptr
	btDefaultMotionState *wallMotionState4 = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, - levelSize / 2, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI4(btScalar(0), wallMotionState4, wallShape4, btVector3(0, 0, 0));
	btRigidBody wallRigidBody4(wallRigidBodyCI4);


	m_rigidBodies->push_back(wallRigidBody1);
	m_rigidBodies->push_back(wallRigidBody2);
	m_rigidBodies->push_back(wallRigidBody3);
	m_rigidBodies->push_back(wallRigidBody4);
	
}

std::shared_ptr<std::vector<btRigidBody>> LevelModel::getRigidBodies()
{
	return m_rigidBodies;
}


int LevelModel::getLevelSize() {
	return 1000;
}