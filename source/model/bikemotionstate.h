#pragma once

#include <osg/PositionAttitudeTransform>

#include <btBulletDynamicsCommon.h>
#include "../controller/fencecontroller.h"

namespace troen
{
	class BikeMotionState : public btMotionState 
	{
	public:
		BikeMotionState(
			const btTransform &initialpos,
			osg::PositionAttitudeTransform* pat,
			std::shared_ptr<FenceController> fenceController,
			btVector3 bikeDimensions) :
				m_position(initialpos),
				m_visibleObj(pat),
				m_fenceController(fenceController),
				m_bikeDimensions(bikeDimensions){}

		virtual ~BikeMotionState() {}

		void setRigidBody(std::weak_ptr<btRigidBody> bikeRigidBody)
		{
			m_rigidBody = bikeRigidBody;
		}

		void setNode(osg::PositionAttitudeTransform* pat) {
			m_visibleObj = pat;
		}

		virtual void getWorldTransform(btTransform &worldTrans) const {
			worldTrans = m_position;
		}

		virtual void setWorldTransform(const btTransform &worldTrans) {
			if (nullptr == m_visibleObj)
				return; // silently return before we set a node
			btQuaternion rot = worldTrans.getRotation();
			osg::Vec3 axis = osg::Vec3(rot.getAxis().x(), rot.getAxis().y(), rot.getAxis().z());
			osg::Quat rotationQuat(rot.getAngle(), axis);

			// btVector3 angluarVelocity = m_rigidBody.lock()->getAngularVelocity();
			// std::cout << angluarVelocity.x() << " " << angluarVelocity.y() << " " << angluarVelocity.z() << std::endl;

			m_visibleObj->setAttitude(rotationQuat);

			btVector3 pos = worldTrans.getOrigin();
			m_visibleObj->setPosition(osg::Vec3(pos.x(), pos.y(), pos.z()));
			
			btVector3 fenceOffset = btVector3(
				0, -m_bikeDimensions.y() / 2, m_bikeDimensions.z() / 2).rotate(rot.getAxis(), rot.getAngle());
			
			// update fence accordingly
			m_fenceController.lock()->update(pos - fenceOffset);
		}

	protected:
		osg::PositionAttitudeTransform* m_visibleObj;
		btTransform m_position;
		std::weak_ptr<FenceController> m_fenceController;
		std::weak_ptr<btRigidBody> m_rigidBody;
		btVector3 m_bikeDimensions;
	};
}