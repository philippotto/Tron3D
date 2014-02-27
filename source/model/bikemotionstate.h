#pragma once
#include <algorithm>
// OSG
#include <osg/PositionAttitudeTransform>
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../player.h"
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
			Player * player,
			BikeModel *bikeModel) :
                btMotionState(),
				m_player(player),
				m_visibleObj(pat),
                m_positionTransform(initialTransform),
				m_bikeModel(bikeModel),
				m_currentSteeringTilt(0)
		{}

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
			m_player->fenceController()->update(pos, rot);
		}

		osg::Quat getTilt()
		{

			float desiredSteeringTilt = m_bikeModel->getSteering() / BIKE_TILT_MAX;
			
			// timeFactor is 1 for 60 frames, 0.5 for 30 frames etc..
			long double timeFactor = 16.7f / m_bikeModel->getTimeSinceLastUpdate();
			// sanity check for very large delays
			if (timeFactor < 1 / BIKE_TILT_DAMPENING)
				timeFactor = 1 / BIKE_TILT_DAMPENING;
			
			m_currentSteeringTilt = m_currentSteeringTilt + (desiredSteeringTilt - m_currentSteeringTilt) / (BIKE_TILT_DAMPENING * timeFactor);

			float turboFactor = m_bikeModel->getTurboFactor();

			if (turboFactor < 0) {
				// no interpolation on abrupt speed change
				m_currentWheelyTilt = 0;
			}
			else{
				const float desiredWheelyTilt = m_bikeModel->getTurboFactor() / BIKE_WHEELY_TILT_MAX;
				const float tiltDifference = desiredWheelyTilt - m_currentWheelyTilt;
				m_currentWheelyTilt = m_currentWheelyTilt + tiltDifference / (BIKE_TILT_DAMPENING * timeFactor);
			}


			osg::Quat tiltSteeringQuat, tiltWheelyQuat;
			tiltSteeringQuat.makeRotate(m_currentSteeringTilt, osg::Vec3(0, 1, 0));
			tiltWheelyQuat.makeRotate(m_currentWheelyTilt, osg::Vec3(-1, 0, 0));

			return tiltSteeringQuat * tiltWheelyQuat;
		}

	protected:
		Player * m_player;
		BikeModel* m_bikeModel;
		osg::PositionAttitudeTransform* m_visibleObj;
		btTransform m_positionTransform;
		std::weak_ptr<btRigidBody> m_rigidBody;
		float m_currentSteeringTilt;
		float m_currentWheelyTilt;
	};
}