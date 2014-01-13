#pragma once
// OSG
#include <osg/PositionAttitudeTransform>
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../controller/fencecontroller.h"
#include "../model/bikemodel.h"

namespace troen
{
	class BikeMotionState : public btMotionState 
	{
	public:
		BikeMotionState(
			const btTransform &initialTransform,
			osg::PositionAttitudeTransform* pat,
			std::shared_ptr<FenceController> fenceController,
			BikeModel *bikeModel) :
				m_visibleObj(pat),
                m_positionTransform(initialTransform),
				m_fenceController(fenceController),
				m_bikeModel(bikeModel),
				m_currentTilt(0), btMotionState() {	}

		virtual ~BikeMotionState() {}

		void setRigidBody(std::weak_ptr<btRigidBody> bikeRigidBody)
		{
			m_rigidBody = bikeRigidBody;
		}

		void setNode(osg::PositionAttitudeTransform* pat) {
			m_visibleObj = pat;
		}

		virtual void getWorldTransform(btTransform &worldTrans) const {
			worldTrans = m_positionTransform;
		}

		virtual void setWorldTransform(const btTransform &worldTrans) {
			if (nullptr == m_visibleObj)
				return; // silently return before we set a node
			btQuaternion rot = worldTrans.getRotation();

			osg::Vec3 axis = osg::Vec3(rot.getAxis().x(), rot.getAxis().y(), rot.getAxis().z());
			osg::Quat rotationQuat(rot.getAngle(), axis);
			
			m_visibleObj->setAttitude(getTilt() * rotationQuat);

			btVector3 pos = worldTrans.getOrigin();
			m_visibleObj->setPosition(osg::Vec3(pos.x(), pos.y(), pos.z()));
			
			// update fence accordingly
			m_fenceController.lock()->update(pos, rot);
		}

		osg::Quat getTilt()
		{
			float desiredTilt = m_bikeModel->getSteering() / BIKE_TILT_MAX;
			m_currentTilt = m_currentTilt + (desiredTilt - m_currentTilt) / BIKE_TILT_DAMPENING;
			
			osg::Quat tiltingQuat;
			tiltingQuat.makeRotate(m_currentTilt, osg::Vec3(0, 1, 0));

			return tiltingQuat;
		}

	protected:
		osg::PositionAttitudeTransform* m_visibleObj;
		btTransform m_positionTransform;
		std::weak_ptr<FenceController> m_fenceController;
		BikeModel* m_bikeModel;
		std::weak_ptr<btRigidBody> m_rigidBody;
		float m_currentTilt;
	};
}