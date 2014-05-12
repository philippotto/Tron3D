#pragma once
// OSG
#include <osg/ref_ptr>
#include <osg/Vec3d>
#include <osg/Group>
// troen
#include "../forwarddeclarations.h"
#include "abstractmodel.h"

namespace troen
{

	class BikeModel : public AbstractModel
	{
	public:
		BikeModel(
			btTransform initialTransform,
			osg::ref_ptr<osg::Group> node,
			Player * player,
			BikeController* bikeController);

		void setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState);

		long double getTimeSinceLastUpdate();
		float updateState(const long double time);
		float updateRemoteState();
		void updateAngularVelocity(float speed);
		float calculatePossibleTurboBoost();
		float calculateAttenuatedSpeed(float speed);
		float calculateAcceleratedSpeed(btVector3 velocityXY, float timeFactor);
		float calculateBikeFriction(const btScalar currentAngle, float timeFactor);
		btVector3 adaptVelocityToRealDirection(btVector3 &velocityXY, float speed, float timeFactor);
		void resetState();
		void freeze();

		void rotate(const float angle);
		void accelerate(const float velocity);
		float getRotation();
		float getVelocity();
		float getSteering();
		osg::Vec3d getPositionOSG();
		btVector3 getPositionBt();
		btQuaternion getRotationQuat();

		void moveBikeToPosition(const btTransform position);
		float getTurboFactor();
		void updateTurboFactor(const float newVelocity, const float time);
		float getInputAcceleration();
		float getInputAngle();
		btVector3 getLinearVelocity();
		btVector3 getAngularVelocity();
		osg::Quat getTilt();
		btTransform getTransform();
		btVector3 BikeModel::getDirection();
		std::shared_ptr<btRigidBody> getRigidBody();
	private:
		osg::ref_ptr<input::BikeInputState> m_bikeInputState;
		float m_oldVelocity;
		float m_rotation;
		float m_steering;
		float m_turboFactor;
		float m_timeOfLastTurboInitiation;
		long double m_lastUpdateTime;
		BikeController* m_bikeController;
		long double m_timeSinceLastUpdate;
		float m_currentSteeringTilt;
		float m_currentWheelyTilt;
		std::shared_ptr<btRigidBody> m_bikeRigidBody;

	};
}