#pragma once

#include <osg/PositionAttitudeTransform>

#include <btBulletDynamicsCommon.h>
#include "../controller/fencecontroller.h"
#include "../model/bikemodel.h"

namespace troen
{
	class BikeMotionState : public btMotionState 
	{
	public:
		BikeMotionState(
			const btTransform &initialpos,
			osg::PositionAttitudeTransform* pat,
			std::shared_ptr<FenceController> fenceController,
			BikeModel *bikeModel,
			btVector3 bikeDimensions) :
				m_visibleObj(pat),
                m_position(initialpos),
				m_fenceController(fenceController),
				m_bikeModel(bikeModel),
				m_bikeDimensions(bikeDimensions),
				m_currentTilt(0){}

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
			
			m_visibleObj->setAttitude(getTilt() * rotationQuat);

			btVector3 pos = worldTrans.getOrigin();
			m_visibleObj->setPosition(osg::Vec3(pos.x(), pos.y(), pos.z()));
			
			btVector3 fenceOffset = btVector3(
				0,
				-m_bikeDimensions.y() / 2,
				m_bikeDimensions.z() / 2).rotate(rot.getAxis(), rot.getAngle()
			);
			
			// update fence accordingly
			m_fenceController.lock()->update(pos - fenceOffset);
		}

		osg::Quat getTilt() {
			// tiltDampening = 1 would lead to immediate/unsmooth tilt
			// 1 / maximumTilt specifies angle in radiant
			static const float tiltDampening = 20.f;
			static const float maximumTilt = 16.f;

			float desiredTilt = m_bikeModel->getSteering() / maximumTilt;
			m_currentTilt = m_currentTilt + (desiredTilt - m_currentTilt) / tiltDampening;
			
			osg::Quat tiltingQuat;
			tiltingQuat.makeRotate(m_currentTilt, osg::Vec3(0, 1, 0));

			return tiltingQuat;
		}

	protected:
		osg::PositionAttitudeTransform* m_visibleObj;
		btTransform m_position;
		std::weak_ptr<FenceController> m_fenceController;
		BikeModel* m_bikeModel;
		std::weak_ptr<btRigidBody> m_rigidBody;
		btVector3 m_bikeDimensions;
		float m_currentTilt;
	};
}