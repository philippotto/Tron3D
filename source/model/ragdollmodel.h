
#pragma once
#include "../forwarddeclarations.h"
#include "abstractmodel.h"
#include <osgAnimation/Bone>

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "../controller/ragdollcontroller.h"
#include "../util/conversionutils.h"

namespace troen
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
	//declare extern to allow definition in cpp file
	extern char *bodyPartNames[BODYPART_COUNT];


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



	class ModelBone
	{
	public:
		BODYPART m_bodyType;
		btCollisionShape *m_shape = nullptr;
		btRigidBody *m_rigidBody = nullptr;
		btMotionState *m_motionState = nullptr;
		
		ModelBone *m_parent = nullptr;
		osgAnimation::Bone *m_viewBone = nullptr;

		ModelBone(BODYPART bodyType, btTransform origin, btScalar radius, btScalar height, float mass, RagdollController *ragdollController, btDynamicsWorld* ownerWorld, ModelBone *parent=nullptr);
		btTransform localBoneTransform(btTransform worldTransform);

		btTransform m_initialTransform;
		btScalar m_radius;
		btScalar m_height;
		btDynamicsWorld* m_ownerWorld;
		RagdollController *m_ragdollController;

	};



	class RagdollModel : public AbstractModel
	{

	public:
		RagdollModel(btDynamicsWorld* ownerWorld, RagdollController *controller, const btVector3& positionOffset);

		btDynamicsWorld *getOwnerWorld()
		{ return m_ownerWorld; }

	protected:

		btDynamicsWorld* m_ownerWorld;
		ModelBone *m_bones[BODYPART_COUNT];
		btTypedConstraint* m_joints[JOINT_COUNT];

		RagdollController *m_ragdollController;


		//btRigidBody* localCreateRigidBody(btScalar mass, const btTransform& startTransform, BODYPART bodyPart);
		//virtual	~RagdollModel();



	};




	class BoneMotionState : public btMotionState
	{
	public:
		BoneMotionState(ModelBone *bone):
			m_bone(bone)
		{
			m_positionTransform = m_bone->m_initialTransform;
		}

		virtual ~BoneMotionState() {}



		virtual void getWorldTransform(btTransform &worldTrans) const {
			worldTrans = m_positionTransform;
		}

		virtual void setWorldTransform(const btTransform &worldTrans) {
			if (nullptr == m_bone->m_viewBone)
				return; // silently return before we set a node

			//osg::NodePathList     paths = m_visibleBodyPart->getParentalNodePaths();
			//osg::Matrix   localMatrix = osg::computeWorldToLocal(paths.at(0)) * Conversion::asOsgMatrix(worldTrans);
			//m_visibleBodyPart->setAttitude(localMatrix.getRotate());
			//m_visibleBodyPart->setPosition(localMatrix.getTrans());
			btTransform localTransform = m_bone->localBoneTransform(worldTrans);
			m_positionTransform = localTransform;// localTransform;
		}


	protected:
		ModelBone *m_bone;
		btTransform m_positionTransform;

	};

}