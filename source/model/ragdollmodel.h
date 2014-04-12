
#pragma once
#include "../forwarddeclarations.h"
#include "abstractmodel.h"

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "../controller/ragdollcontroller.h"
#include "../util/conversionutils.h"

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


	class RagdollMotionState : public btMotionState
	{
	public:
		RagdollMotionState(
			const btTransform &initialTransform,
			osg::PositionAttitudeTransform* pat,
			RagdollController *ragdollController) :
			btMotionState(),
			m_ragdollController(ragdollController),
			m_visibleBodyPart(pat),
			m_positionTransform(initialTransform)
		{}

		virtual ~RagdollMotionState() {}


		void setNode(osg::PositionAttitudeTransform* pat) {
			m_visibleBodyPart = pat;
		}

		virtual void getWorldTransform(btTransform &worldTrans) const {
			worldTrans = m_positionTransform;
		}


		virtual void setWorldTransform(const btTransform &worldTrans) {
			if (nullptr == m_visibleBodyPart)
				return; // silently return before we set a node

			//osg::NodePathList     paths = m_visibleBodyPart->getParentalNodePaths();
			//osg::Matrix   localMatrix = osg::computeWorldToLocal(paths.at(0)) * Conversion::asOsgMatrix(worldTrans);
			//m_visibleBodyPart->setAttitude(localMatrix.getRotate());
			//m_visibleBodyPart->setPosition(localMatrix.getTrans());
			Conversion::updateWithTransform(worldTrans, m_visibleBodyPart);
		}


	protected:
		RagdollModel* m_ragdollModel;
		RagdollController *m_ragdollController;
		osg::PositionAttitudeTransform* m_visibleBodyPart;
		btTransform m_positionTransform;

	};

}