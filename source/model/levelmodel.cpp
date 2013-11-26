#include "levelmodel.h"
//bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"

using namespace troen;

LevelModel::LevelModel(const LevelController* levelController)
{
	m_rigidBodies = std::vector<std::shared_ptr<btRigidBody>>();

	btStaticPlaneShape *groundShape = new btStaticPlaneShape(btVector3(0, 0, 1), 0);
	btDefaultMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(btScalar(0), groundMotionState, groundShape, btVector3(0, 0, 0));
	std::shared_ptr<btRigidBody> groundRigidBody = std::make_shared<btRigidBody>(groundRigidBodyCI);

	m_rigidBodies.push_back(groundRigidBody);

	addWalls();

	for (auto body : m_rigidBodies)
		body->setUserPointer((void*)levelController);
	
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
	btScalar levelSize = btScalar(getLevelSize());
	 
	btStaticPlaneShape *wallShape1 = new btStaticPlaneShape(planeNormal, 0);
	btDefaultMotionState *wallMotionState1 = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(levelSize / 2, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI1(btScalar(0), wallMotionState1, wallShape1, btVector3(0, 0, 0));
	std::shared_ptr<btRigidBody> wallRigidBody1 = std::make_shared<btRigidBody>(wallRigidBodyCI1);

	btStaticPlaneShape *wallShape2 = new btStaticPlaneShape(-planeNormal, 0);
	btDefaultMotionState *wallMotionState2 = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(- levelSize / 2, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI2(btScalar(0), wallMotionState2, wallShape2, btVector3(0, 0, 0));
	std::shared_ptr<btRigidBody> wallRigidBody2 = std::make_shared<btRigidBody>(wallRigidBodyCI2);

	planeNormal.setX(0); planeNormal.setY(-1);

	btStaticPlaneShape *wallShape3 = new btStaticPlaneShape(planeNormal, 0);
	btDefaultMotionState *wallMotionState3 = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, levelSize / 2, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI3(btScalar(0), wallMotionState3, wallShape3, btVector3(0, 0, 0));
	std::shared_ptr<btRigidBody> wallRigidBody3 = std::make_shared<btRigidBody>(wallRigidBodyCI3);

	btStaticPlaneShape *wallShape4 = new btStaticPlaneShape(-planeNormal, 0);
	btDefaultMotionState *wallMotionState4 = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, - levelSize / 2, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI4(btScalar(0), wallMotionState4, wallShape4, btVector3(0, 0, 0));
	std::shared_ptr<btRigidBody> wallRigidBody4 = std::make_shared<btRigidBody>(wallRigidBodyCI4);

	m_rigidBodies.push_back(wallRigidBody1);
	m_rigidBodies.push_back(wallRigidBody2);
	m_rigidBodies.push_back(wallRigidBody3);
	m_rigidBodies.push_back(wallRigidBody4);
}

int LevelModel::getLevelSize() {
	return 6000;
}