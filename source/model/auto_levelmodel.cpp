
		#include "levelmodel.h"
		//bullet
		#include <btBulletDynamicsCommon.h>
		#include "LinearMath/btHashMap.h"

		using namespace troen;

		void LevelModel::auto_addObstacles(const LevelController* levelController)
	{
		// obstacles
		// TODO grab the value from origin
		btScalar levelSize = btScalar(getLevelSize());

		
		std::shared_ptr<btBoxShape> obstacleShape0 = std::make_shared<btBoxShape>(btVector3(10.000004768371582, 10.000004768371582, 10.0));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState0 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0, 40.0, 0.0)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI0(btScalar(0), obstacleMotionState0.get(), obstacleShape0.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody0 = std::make_shared<btRigidBody>(obstacleRigidBodyCI0);
		obstacleRigidBody0->setUserPointer((void *)levelController);
		obstacleRigidBody0->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape0);
		m_motionStates.push_back(obstacleMotionState0);
		m_rigidBodies.push_back(obstacleRigidBody0);

		
		std::shared_ptr<btBoxShape> obstacleShape1 = std::make_shared<btBoxShape>(btVector3(10.000004768371582, 10.000004768371582, 10.0));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState1 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0, -40.0, 0.0)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI1(btScalar(0), obstacleMotionState1.get(), obstacleShape1.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody1 = std::make_shared<btRigidBody>(obstacleRigidBodyCI1);
		obstacleRigidBody1->setUserPointer((void *)levelController);
		obstacleRigidBody1->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape1);
		m_motionStates.push_back(obstacleMotionState1);
		m_rigidBodies.push_back(obstacleRigidBody1);

		

	}
	