/*
Bullet Continuous Collision Detection and Physics Library
RagdollDemo
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

#pragma once
#include "../forwarddeclarations.h"
#include "abstractmodel.h"

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "../controller/ragdollcontroller.h"

namespace troen
{

	class RagdollModel : public AbstractModel
	{
		enum BODYPART
		{
			BODYPART_PELVIS = 0,
			BODYPART_SPINE,
			BODYPART_HEAD,

			BODYPART_LEFT_UPPER_LEG,
			BODYPART_LEFT_LOWER_LEG,

			BODYPART_RIGHT_UPPER_LEG,
			BODYPART_RIGHT_LOWER_LEG,

			BODYPART_LEFT_UPPER_ARM,
			BODYPART_LEFT_LOWER_ARM,

			BODYPART_RIGHT_UPPER_ARM,
			BODYPART_RIGHT_LOWER_ARM,

			BODYPART_COUNT
		};

		enum
		{
			JOINT_PELVIS_SPINE = 0,
			JOINT_SPINE_HEAD,

			JOINT_LEFT_HIP,
			JOINT_LEFT_KNEE,

			JOINT_RIGHT_HIP,
			JOINT_RIGHT_KNEE,

			JOINT_LEFT_SHOULDER,
			JOINT_LEFT_ELBOW,

			JOINT_RIGHT_SHOULDER,
			JOINT_RIGHT_ELBOW,

			JOINT_COUNT
		};


	public:
		RagdollModel(btDynamicsWorld* ownerWorld, RagdollController *controller, const btVector3& positionOffset);

		btMotionState** getMotionStates() { return m_motionStates; };


	protected:

		btDynamicsWorld* m_ownerWorld;
		btCollisionShape* m_shapes[BODYPART_COUNT];
		btRigidBody* m_bodies[BODYPART_COUNT];
		btTypedConstraint* m_joints[JOINT_COUNT];
		btMotionState* m_motionStates[BODYPART_COUNT];

		RagdollController *m_ragdollController;


		btRigidBody* localCreateRigidBody(btScalar mass, const btTransform& startTransform, BODYPART bodyPart);
		//virtual	~RagdollModel();


	};

}