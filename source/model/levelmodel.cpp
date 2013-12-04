#include "levelmodel.h"
//bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"

using namespace troen;

LevelModel::LevelModel(const LevelController* levelController)
{
	m_rigidBodies = std::vector<std::shared_ptr<btRigidBody>>();

	btScalar levelSize = btScalar(getLevelSize());
	std::shared_ptr<btBoxShape> groundShape = std::make_shared<btBoxShape>(btVector3(levelSize / 2, levelSize / 2, 10));
	std::shared_ptr<btDefaultMotionState> groundMotionState = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -10)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(btScalar(0), groundMotionState.get(), groundShape.get(), btVector3(0, 0, 0));
	std::shared_ptr<btRigidBody> groundRigidBody = std::make_shared<btRigidBody>(groundRigidBodyCI);
	groundRigidBody->setUserIndex(LEVELGROUNDTYPE);

	m_collisionShapes.push_back(groundShape);
	m_motionStates.push_back(groundMotionState);
	m_rigidBodies.push_back(groundRigidBody);

	addWalls();

	for (auto body : m_rigidBodies)
		body->setUserPointer((void*)levelController);
}

void LevelModel::addWalls()
{
	// walls

	// TODO
	// get data from levelView ? or the other way round?
	// in levelView: wallRight = new osg::Box(osg::Vec3(levelSize / 2, 0, 50), 1, levelSize, 100);
	// center: levelSize/2, 0, 50
	// width: 1
	// length: levelSize
	// height: 100

	// TODO grab the value from origin
	btScalar levelSize = btScalar(getLevelSize());
	 
	std::shared_ptr<btBoxShape> wallShape1 = std::make_shared<btBoxShape>(btVector3(1, levelSize/2, 20));
	std::shared_ptr<btDefaultMotionState> wallMotionState1 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(levelSize / 2, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI1(btScalar(0), wallMotionState1.get(), wallShape1.get(), btVector3(0, 0, 0));
	std::shared_ptr<btRigidBody> wallRigidBody1 = std::make_shared<btRigidBody>(wallRigidBodyCI1);
	wallRigidBody1->setUserIndex(LEVELWALLTYPE);

	std::shared_ptr<btBoxShape> wallShape2 = std::make_shared<btBoxShape>(btVector3(1, levelSize / 2, 20));
	std::shared_ptr<btDefaultMotionState> wallMotionState2 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-levelSize / 2, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI2(btScalar(0), wallMotionState2.get(), wallShape2.get(), btVector3(0, 0, 0));
	std::shared_ptr<btRigidBody> wallRigidBody2 = std::make_shared<btRigidBody>(wallRigidBodyCI2);
	wallRigidBody2->setUserIndex(LEVELWALLTYPE);

	std::shared_ptr<btBoxShape> wallShape3 = std::make_shared<btBoxShape>(btVector3(levelSize / 2, 1, 20));
	std::shared_ptr<btDefaultMotionState> wallMotionState3 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, levelSize / 2, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI3(btScalar(0), wallMotionState3.get(), wallShape3.get(), btVector3(0, 0, 0));
	std::shared_ptr<btRigidBody> wallRigidBody3 = std::make_shared<btRigidBody>(wallRigidBodyCI3);
	wallRigidBody3->setUserIndex(LEVELWALLTYPE);


	std::shared_ptr<btBoxShape> wallShape4 = std::make_shared<btBoxShape>(btVector3(levelSize / 2, 1, 20));
	std::shared_ptr<btDefaultMotionState> wallMotionState4 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -levelSize / 2, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI4(btScalar(0), wallMotionState4.get(), wallShape4.get(), btVector3(0, 0, 0));
	std::shared_ptr<btRigidBody> wallRigidBody4 = std::make_shared<btRigidBody>(wallRigidBodyCI4);
	wallRigidBody4->setUserIndex(LEVELWALLTYPE);

	m_collisionShapes.push_back(wallShape1);
	m_collisionShapes.push_back(wallShape2);
	m_collisionShapes.push_back(wallShape3);
	m_collisionShapes.push_back(wallShape4);

	m_motionStates.push_back(wallMotionState1);
	m_motionStates.push_back(wallMotionState2);
	m_motionStates.push_back(wallMotionState3);
	m_motionStates.push_back(wallMotionState4); 

	m_rigidBodies.push_back(wallRigidBody1);
	m_rigidBodies.push_back(wallRigidBody2);
	m_rigidBodies.push_back(wallRigidBody3);
	m_rigidBodies.push_back(wallRigidBody4);
}

int LevelModel::getLevelSize() {
	return 6000;
}