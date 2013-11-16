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
	int levelSize = 1000;
	
	btStaticPlaneShape *wallShape = new btStaticPlaneShape(planeNormal, 0);
	// TODO: convert to shared_ptr
	btDefaultMotionState *wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(levelSize / 2, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wallRigidBodyCI(btScalar(0), wallMotionState, wallShape, btVector3(0, 0, 0));
	btRigidBody wallRigidBody(wallRigidBodyCI);

	
	m_rigidBodies->push_back(wallRigidBody);
	
}

std::shared_ptr<std::vector<btRigidBody>> LevelModel::getRigidBodies()
{
	return m_rigidBodies;
}