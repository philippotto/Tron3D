
		#include "levelmodel.h"
		//bullet
		#include <btBulletDynamicsCommon.h>
		#include "LinearMath/btHashMap.h"

		using namespace troen;
		
		//!!!!!!!!!!!!! WARNING: AUTO_GENERATED !!!!!!!!!!!!!!!!!!!!!!
		// If you want to change something generally, please edit obstacle_export.py, otherwise be sure to mark changes to this code otherwise it might be overwritten


		void LevelModel::auto_addObstacles(const LevelController* levelController)
	{
		// obstacles
		// TODO grab the value from origin
		btScalar levelSize = btScalar(getLevelSize());

		
		std::shared_ptr<btBoxShape> obstacleShape0 = std::make_shared<btBoxShape>(btVector3(10.000004768371582, 10.000004768371582, 10.0));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState0 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-15.566339492797852, -85.02395629882812, 0.0)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI0(btScalar(0), obstacleMotionState0.get(), obstacleShape0.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody0 = std::make_shared<btRigidBody>(obstacleRigidBodyCI0);
		obstacleRigidBody0->setUserPointer((void *)levelController);
		obstacleRigidBody0->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape0);
		m_motionStates.push_back(obstacleMotionState0);
		m_rigidBodies.push_back(obstacleRigidBody0);

		
		std::shared_ptr<btBoxShape> obstacleShape1 = std::make_shared<btBoxShape>(btVector3(10.000004768371582, 10.000004768371582, 10.0));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState1 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(72.07053184509277, 0.0, 0.0)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI1(btScalar(0), obstacleMotionState1.get(), obstacleShape1.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody1 = std::make_shared<btRigidBody>(obstacleRigidBodyCI1);
		obstacleRigidBody1->setUserPointer((void *)levelController);
		obstacleRigidBody1->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape1);
		m_motionStates.push_back(obstacleMotionState1);
		m_rigidBodies.push_back(obstacleRigidBody1);

		
		std::shared_ptr<btBoxShape> obstacleShape2 = std::make_shared<btBoxShape>(btVector3(10.000004768371582, 10.000004768371582, 10.0));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState2 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-86.83469772338867, 0.0, 0.0)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI2(btScalar(0), obstacleMotionState2.get(), obstacleShape2.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody2 = std::make_shared<btRigidBody>(obstacleRigidBodyCI2);
		obstacleRigidBody2->setUserPointer((void *)levelController);
		obstacleRigidBody2->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape2);
		m_motionStates.push_back(obstacleMotionState2);
		m_rigidBodies.push_back(obstacleRigidBody2);

		
		std::shared_ptr<btBoxShape> obstacleShape3 = std::make_shared<btBoxShape>(btVector3(10.000004768371582, 10.000004768371582, 10.0));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState3 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-14.399127960205078, 98.87603759765625, 0.0)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI3(btScalar(0), obstacleMotionState3.get(), obstacleShape3.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody3 = std::make_shared<btRigidBody>(obstacleRigidBodyCI3);
		obstacleRigidBody3->setUserPointer((void *)levelController);
		obstacleRigidBody3->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape3);
		m_motionStates.push_back(obstacleMotionState3);
		m_rigidBodies.push_back(obstacleRigidBody3);

		

	}
	