
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

		
		std::shared_ptr<btBoxShape> obstacleShape0 = std::make_shared<btBoxShape>(btVector3(510.4974365234375, 449.10552978515625, 3.4602096676826477));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState0 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0.0,0.0,0.0,1.0), btVector3(342.88257598876953, 918.4856414794922, 71.01015567779541)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI0(btScalar(0), obstacleMotionState0.get(), obstacleShape0.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody0 = std::make_shared<btRigidBody>(obstacleRigidBodyCI0);
		obstacleRigidBody0->setUserPointer((void *)levelController);
		obstacleRigidBody0->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape0);
		m_motionStates.push_back(obstacleMotionState0);
		m_rigidBodies.push_back(obstacleRigidBody0);

		
		std::shared_ptr<btBoxShape> obstacleShape1 = std::make_shared<btBoxShape>(btVector3(10.000004768371582, 10.000004768371582, 10.0));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState1 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0.0,0.0,0.0,1.0), btVector3(70.70180892944336, -5.369071960449219, -5.070757865905762)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI1(btScalar(0), obstacleMotionState1.get(), obstacleShape1.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody1 = std::make_shared<btRigidBody>(obstacleRigidBodyCI1);
		obstacleRigidBody1->setUserPointer((void *)levelController);
		obstacleRigidBody1->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape1);
		m_motionStates.push_back(obstacleMotionState1);
		m_rigidBodies.push_back(obstacleRigidBody1);

		
		std::shared_ptr<btBoxShape> obstacleShape2 = std::make_shared<btBoxShape>(btVector3(341.51233673095703, 160.96519470214844, 9.766480922698975));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState2 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0.0,0.0,0.0,1.0), btVector3(568.7586212158203, -879.2779541015625, 66.1803674697876)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI2(btScalar(0), obstacleMotionState2.get(), obstacleShape2.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody2 = std::make_shared<btRigidBody>(obstacleRigidBodyCI2);
		obstacleRigidBody2->setUserPointer((void *)levelController);
		obstacleRigidBody2->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape2);
		m_motionStates.push_back(obstacleMotionState2);
		m_rigidBodies.push_back(obstacleRigidBody2);

		
		std::shared_ptr<btBoxShape> obstacleShape3 = std::make_shared<btBoxShape>(btVector3(144.27645683288574, 1202.7391815185547, 5.94524621963501));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState3 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0.0,0.0,0.0,1.0), btVector3(988.9772033691406, 162.55027770996094, 69.9608039855957)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI3(btScalar(0), obstacleMotionState3.get(), obstacleShape3.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody3 = std::make_shared<btRigidBody>(obstacleRigidBodyCI3);
		obstacleRigidBody3->setUserPointer((void *)levelController);
		obstacleRigidBody3->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape3);
		m_motionStates.push_back(obstacleMotionState3);
		m_rigidBodies.push_back(obstacleRigidBody3);

		
		std::shared_ptr<btBoxShape> obstacleShape4 = std::make_shared<btBoxShape>(btVector3(265.5851745605469, 265.5851745605469, 4.221258461475372));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState4 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0.0,0.0,0.0,1.0), btVector3(-10.782440900802612, -778.228759765625, 72.63442039489746)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI4(btScalar(0), obstacleMotionState4.get(), obstacleShape4.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody4 = std::make_shared<btRigidBody>(obstacleRigidBodyCI4);
		obstacleRigidBody4->setUserPointer((void *)levelController);
		obstacleRigidBody4->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape4);
		m_motionStates.push_back(obstacleMotionState4);
		m_rigidBodies.push_back(obstacleRigidBody4);

		
		std::shared_ptr<btBoxShape> obstacleShape5 = std::make_shared<btBoxShape>(btVector3(139.0300750732422, 258.3808898925781, 4.938436150550842));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState5 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(-2.33497376989078e-09,0.08276040852069855,0.9965696334838867,-4.364898131825612e-08), btVector3(-10.782440900802612, -264.0263366699219, 30.399227142333984)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI5(btScalar(0), obstacleMotionState5.get(), obstacleShape5.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody5 = std::make_shared<btRigidBody>(obstacleRigidBodyCI5);
		obstacleRigidBody5->setUserPointer((void *)levelController);
		obstacleRigidBody5->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape5);
		m_motionStates.push_back(obstacleMotionState5);
		m_rigidBodies.push_back(obstacleRigidBody5);

		
		std::shared_ptr<btBoxShape> obstacleShape6 = std::make_shared<btBoxShape>(btVector3(10.000004768371582, 10.000004768371582, 10.0));
		std::shared_ptr<btDefaultMotionState> obstacleMotionState6 = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0.0,0.0,0.0,1.0), btVector3(-80.16692161560059, -5.369071960449219, -5.070757865905762)));
		btRigidBody::btRigidBodyConstructionInfo
			obstacleRigidBodyCI6(btScalar(0), obstacleMotionState6.get(), obstacleShape6.get(), btVector3(0, 0, 0));
		std::shared_ptr<btRigidBody> obstacleRigidBody6 = std::make_shared<btRigidBody>(obstacleRigidBodyCI6);
		obstacleRigidBody6->setUserPointer((void *)levelController);
		obstacleRigidBody6->setUserIndex(LEVELWALLTYPE);


		m_collisionShapes.push_back(obstacleShape6);
		m_motionStates.push_back(obstacleMotionState6);
		m_rigidBodies.push_back(obstacleRigidBody6);

		

	}
	