/*
Bullet Continuous Collision Detection and Physics Library
Ragdoll Demo
Copyright (c) 2007 Starbreeze Studios

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Written by: Marten Svanfeldt
*/

const float CONSTRAINT_DEBUG_SIZE = 0.2f;
const float SHAPE_SIZE_MULTIPLIER = 10.0f;
const float Z_OFFSET = 0.5;




#include "ragdollmodel.h"
#include "abstractmodel.h"

#include "../util/gldebugdrawer.h"
#include "../constants.h"
#include "../globals.h"
#include "objectinfo.h"
#include "../view/ragdollview.h"

using namespace troen;

char *troen::bodyPartNames[BODYPART_COUNT] = { "BODYPART_PELVIS", "BODYPART_SPINE", "BODYPART_HEAD", "BODYPART_LEFT_UPPER_LEG",
"BODYPART_LEFT_LOWER_LEG", "BODYPART_RIGHT_UPPER_LEG", "BODYPART_RIGHT_LOWER_LEG",
"BODYPART_LEFT_UPPER_ARM", "BODYPART_LEFT_LOWER_ARM", "BODYPART_RIGHT_UPPER_ARM", "BODYPART_RIGHT_LOWER_ARM" };


RagdollModel::RagdollModel(btDynamicsWorld* ownerWorld, RagdollController *controller, const btVector3& positionOffset)
		: m_ownerWorld (ownerWorld)
	{
		AbstractModel();

		m_ragdollController = controller;
		float ssm = SHAPE_SIZE_MULTIPLIER;
		// Setup the geometry

		m_bones[BODYPART_PELVIS] = new ModelBone(BODYPART_PELVIS,
			btTransform(btQuaternion(), btVector3(0.0, 1.0, 0.0)),
			btScalar(0.15), btScalar(0.2), btScalar(1.0),
			m_ragdollController, m_ownerWorld);

		m_bones[BODYPART_SPINE] = new ModelBone(BODYPART_SPINE,
			btTransform(btQuaternion(), btVector3(0.0, 1.2, 0.0)),
			btScalar(0.15), btScalar(0.28), btScalar(1.0),
			m_ragdollController, m_ownerWorld, 
			m_bones[BODYPART_PELVIS]);
		
		m_bones[BODYPART_HEAD] = new ModelBone(BODYPART_HEAD,
			btTransform(btQuaternion(), btVector3(0.0, 1.6, 0.0)),
			btScalar(0.1), btScalar(0.05), btScalar(1.0),
			m_ragdollController, m_ownerWorld,
			m_bones[BODYPART_SPINE]);

		m_bones[BODYPART_LEFT_UPPER_LEG] = new ModelBone(BODYPART_LEFT_UPPER_LEG,
			btTransform(btQuaternion(), btVector3(-0.18, 0.65, 0.0)),
			btScalar(0.07), btScalar(0.45), btScalar(1.0),
			m_ragdollController, m_ownerWorld,
			m_bones[BODYPART_PELVIS]);
		
		m_bones[BODYPART_LEFT_LOWER_LEG] = new ModelBone(BODYPART_LEFT_LOWER_LEG,
			btTransform(btQuaternion(), btVector3(-0.18, 0.2, 0.0)),
			btScalar(0.05), btScalar(0.37), btScalar(1.0),
			m_ragdollController, m_ownerWorld,
			m_bones[BODYPART_LEFT_UPPER_LEG]);

		m_bones[BODYPART_RIGHT_UPPER_LEG] = new ModelBone(BODYPART_RIGHT_UPPER_LEG,
			btTransform(btQuaternion(), btVector3(0.18, 0.65, 0.0)),
			btScalar(0.07), btScalar(0.45), btScalar(1.0),
			m_ragdollController, m_ownerWorld,
			m_bones[BODYPART_PELVIS]);
		
		m_bones[BODYPART_RIGHT_LOWER_LEG] = new ModelBone(BODYPART_RIGHT_LOWER_LEG,
			btTransform(btQuaternion(), btVector3(0.18, 0.2, 0.0)),
			btScalar(0.05), btScalar(0.37), btScalar(1.0),
			m_ragdollController, m_ownerWorld,
			m_bones[BODYPART_RIGHT_UPPER_LEG]);

		m_bones[BODYPART_LEFT_UPPER_ARM] = new ModelBone(BODYPART_LEFT_UPPER_ARM,
			btTransform(btQuaternion(0, 0, PI_2), btVector3(-0.35, 1.45, 0.0)),
			btScalar(0.05), btScalar(0.33), btScalar(1.0),
			m_ragdollController, m_ownerWorld,
			m_bones[BODYPART_SPINE]);

		m_bones[BODYPART_LEFT_LOWER_ARM] = new ModelBone(BODYPART_LEFT_LOWER_ARM,
			btTransform(btQuaternion(0, 0, PI_2), btVector3(-0.7, 1.45, 0.0)),
			btScalar(0.04), btScalar(0.25), btScalar(1.0),
			m_ragdollController, m_ownerWorld,
			m_bones[BODYPART_LEFT_UPPER_ARM]);

		m_bones[BODYPART_RIGHT_UPPER_ARM] = new ModelBone(BODYPART_RIGHT_UPPER_ARM,
			btTransform(btQuaternion(0, 0, -PI_2), btVector3(0.35, 1.45, 0.0)),
			btScalar(0.05), btScalar(0.33), btScalar(1.0),
			m_ragdollController, m_ownerWorld,
			m_bones[BODYPART_SPINE]);

		m_bones[BODYPART_RIGHT_LOWER_ARM] = new ModelBone(BODYPART_RIGHT_LOWER_ARM,
			btTransform(btQuaternion(0, 0, -PI_2), btVector3(0.7, 1.45, 0.0)),
			btScalar(0.04), btScalar(0.25), btScalar(1.0),
			m_ragdollController, m_ownerWorld,
			m_bones[BODYPART_RIGHT_UPPER_ARM]);


		// Setup some damping on the m_bodies
		for (int i = 0; i < BODYPART_COUNT; ++i)
		{
			
			m_bones[i]->m_rigidBody->setDamping(0.05, 0.85);
			m_bones[i]->m_rigidBody->setDeactivationTime(0.8);
			m_bones[i]->m_rigidBody->setSleepingThresholds(1.6, 2.5);
		}

		// Now setup the constraints
		btHingeConstraint* hingeC;
		btConeTwistConstraint* coneC;

		btTransform localA, localB;

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0, PI_2, 0); localA.setOrigin(btVector3(btScalar(0.), btScalar(0.15), btScalar(0.)) * ssm);
		localB.getBasis().setEulerZYX(0, PI_2, 0); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.15), btScalar(0.)) * ssm);
		hingeC = new btHingeConstraint(*m_bones[BODYPART_PELVIS]->m_rigidBody, *m_bones[BODYPART_SPINE]->m_rigidBody, localA, localB);
		hingeC->setLimit(btScalar(-PI_4), btScalar(PI_2));
		m_joints[JOINT_PELVIS_SPINE] = hingeC;
		hingeC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_PELVIS_SPINE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0, 0, PI_2); localA.setOrigin(btVector3(btScalar(0.), btScalar(0.30), btScalar(0.)) * ssm);
		localB.getBasis().setEulerZYX(0, 0, PI_2); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)) * ssm);
		coneC = new btConeTwistConstraint(*m_bones[BODYPART_SPINE]->m_rigidBody, *m_bones[BODYPART_HEAD]->m_rigidBody, localA, localB);
		coneC->setLimit(PI_4, PI_4, PI_2);
		m_joints[JOINT_SPINE_HEAD] = coneC;
		coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_SPINE_HEAD], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,-PI_4*5); localA.setOrigin(btVector3(btScalar(-0.18), btScalar(-0.10), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,-PI_4*5); localB.setOrigin(btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bones[BODYPART_PELVIS]->m_rigidBody, *m_bones[BODYPART_LEFT_UPPER_LEG]->m_rigidBody, localA, localB);
		coneC->setLimit(PI_4, PI_4, 0);
		m_joints[JOINT_LEFT_HIP] = coneC;
		coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_HIP], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0, PI_2, 0); localA.setOrigin(btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)) * ssm);
		localB.getBasis().setEulerZYX(0, PI_2, 0); localB.setOrigin(btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)) * ssm);
		hingeC = new btHingeConstraint(*m_bones[BODYPART_LEFT_UPPER_LEG]->m_rigidBody, *m_bones[BODYPART_LEFT_LOWER_LEG]->m_rigidBody, localA, localB);
		hingeC->setLimit(btScalar(0), btScalar(PI_2));
		m_joints[JOINT_LEFT_KNEE] = hingeC;
		hingeC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_KNEE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0, 0, PI_4); localA.setOrigin(btVector3(btScalar(0.18), btScalar(-0.10), btScalar(0.)) * ssm);
		localB.getBasis().setEulerZYX(0, 0, PI_4); localB.setOrigin(btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)) * ssm);
		coneC = new btConeTwistConstraint(*m_bones[BODYPART_PELVIS]->m_rigidBody, *m_bones[BODYPART_RIGHT_UPPER_LEG]->m_rigidBody, localA, localB);
		coneC->setLimit(PI_4, PI_4, 0);
		m_joints[JOINT_RIGHT_HIP] = coneC;
		coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_HIP], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0, PI_2, 0); localA.setOrigin(btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)) * ssm);
		localB.getBasis().setEulerZYX(0, PI_2, 0); localB.setOrigin(btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)) * ssm);
		hingeC = new btHingeConstraint(*m_bones[BODYPART_RIGHT_UPPER_LEG]->m_rigidBody, *m_bones[BODYPART_RIGHT_LOWER_LEG]->m_rigidBody, localA, localB);
		hingeC->setLimit(btScalar(0), btScalar(PI_2));
		m_joints[JOINT_RIGHT_KNEE] = hingeC;
		hingeC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_KNEE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0, 0, PI); localA.setOrigin(btVector3(btScalar(-0.2), btScalar(0.15), btScalar(0.)) * ssm);
		localB.getBasis().setEulerZYX(0, 0, PI_2); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)) * ssm);
		coneC = new btConeTwistConstraint(*m_bones[BODYPART_SPINE]->m_rigidBody, *m_bones[BODYPART_LEFT_UPPER_ARM]->m_rigidBody, localA, localB);
		coneC->setLimit(PI_2, PI_2, 0);
		coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_joints[JOINT_LEFT_SHOULDER] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_SHOULDER], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0, PI_2, 0); localA.setOrigin(btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)) * ssm);
		localB.getBasis().setEulerZYX(0, PI_2, 0); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)) * ssm);
		hingeC = new btHingeConstraint(*m_bones[BODYPART_LEFT_UPPER_ARM]->m_rigidBody, *m_bones[BODYPART_LEFT_LOWER_ARM]->m_rigidBody, localA, localB);
//		hingeC->setLimit(btScalar(-M_PI_2), btScalar(0));
		hingeC->setLimit(btScalar(0), btScalar(PI_2));
		m_joints[JOINT_LEFT_ELBOW] = hingeC;
		hingeC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_ELBOW], true);



		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0, 0, 0); localA.setOrigin(btVector3(btScalar(0.2), btScalar(0.15), btScalar(0.)) * ssm);
		localB.getBasis().setEulerZYX(0, 0, PI_2); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)) * ssm);
		coneC = new btConeTwistConstraint(*m_bones[BODYPART_SPINE]->m_rigidBody, *m_bones[BODYPART_RIGHT_UPPER_ARM]->m_rigidBody, localA, localB);
		coneC->setLimit(PI_2, PI_2, 0);
		m_joints[JOINT_RIGHT_SHOULDER] = coneC;
		coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_SHOULDER], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0, PI_2, 0); localA.setOrigin(btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)) * ssm);
		localB.getBasis().setEulerZYX(0, PI_2, 0); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)) * ssm);
		hingeC = new btHingeConstraint(*m_bones[BODYPART_RIGHT_UPPER_ARM]->m_rigidBody, *m_bones[BODYPART_RIGHT_LOWER_ARM]->m_rigidBody, localA, localB);
//		hingeC->setLimit(btScalar(-M_PI_2), btScalar(0));
		hingeC->setLimit(btScalar(0), btScalar(PI_2));
		m_joints[JOINT_RIGHT_ELBOW] = hingeC;
		hingeC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_ELBOW], true);
	}




	ModelBone::ModelBone(BODYPART bodyType, btTransform origin, btScalar radius, btScalar height, float mass, RagdollController *ragdollController, btDynamicsWorld* ownerWorld, ModelBone *parent):
		m_bodyType(bodyType), m_worldTransform(origin), m_radius(radius), m_height(height), 
		m_ragdollController(ragdollController), m_ownerWorld(ownerWorld), m_parent(parent)
	{
		btScalar ssm = SHAPE_SIZE_MULTIPLIER;
		m_worldTransform.setOrigin((m_worldTransform.getOrigin() + btVector3(0.0,0.0,Z_OFFSET)) * ssm);
		m_shape = new btCapsuleShape(radius * ssm, height * ssm);

		btScalar _mass = mass;
		bool isDynamic = (_mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			m_shape->calculateLocalInertia(_mass, localInertia);


		//osg::PositionAttitudeTransform *pat = m_ragdollController->getView()->createBodyPart(origin,
		//	((btCapsuleShape*)m_shape)->getRadius(), ((btCapsuleShape*)m_shape)->getHalfHeight() * 2.0);

		BoneMotionState* myMotionState = new BoneMotionState(this);
		m_motionState = myMotionState;

		RagdollView *ragdollView = m_ragdollController->getView().get();

		if (parent == nullptr)
		{
			m_viewBone = ragdollView->createBone(bodyPartNames[bodyType], Conversion::asOsgMatrix(m_worldTransform), ragdollView->getSkelRoot(),myMotionState);
		}
		else //if (parent->m_bodyType == BODYPART_PELVIS)
		{

			m_viewBone = ragdollView->createBone(bodyPartNames[bodyType], Conversion::asOsgMatrix(localBoneTransform(m_worldTransform)), parent->m_viewBone, myMotionState);
		}

		// draw bone name in debugview
		static_cast<troen::util::GLDebugDrawer*>(m_ownerWorld->getDebugDrawer())->setTextSize(200.0);
		static_cast<troen::util::GLDebugDrawer*>(m_ownerWorld->getDebugDrawer())->draw3dText(origin.getOrigin(), bodyPartNames[m_bodyType]);




		btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass, myMotionState, m_shape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		m_rigidBody = body;

		ObjectInfo* info = new ObjectInfo(m_ragdollController, ABSTRACTTYPE);
		body->setUserPointer(info);

		m_ownerWorld->addRigidBody(body, COLGROUP_BIKE, COLMASK_BIKE);
	}

	btTransform ModelBone::localBoneTransform(btTransform worldTransform)
	{
		if (m_parent != nullptr)
		{
			btVector3 parentToChild = m_parent->m_worldTransform.getOrigin() - worldTransform.getOrigin() ;
			btQuaternion parentToChildQuat = m_parent->m_worldTransform.getRotation() * worldTransform.getRotation().inverse();

			return btTransform(parentToChildQuat, parentToChild);
			//return btTransform(worldTransform.getRotation(), parentToChild);
		}
		else
			return worldTransform;
	}
