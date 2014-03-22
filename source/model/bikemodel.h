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
		void resetState();
		void freeze();

		void rotate(const float angle);
		void accelerate(const float velocity);
		float getRotation();
		float getVelocity();
		float getSteering();
		osg::Vec3d getPositionOSG();
		btVector3 getPositionBt();

		void moveBikeToPosition(const btTransform position);
		float getTurboFactor();
		void updateTurboFactor(const float newVelocity, const float time);
		btVector3 BikeModel::getDirection();
		std::shared_ptr<btRigidBody> getRigidBody();
	private:
		osg::ref_ptr<input::BikeInputState> m_bikeInputState;
		float m_oldVelocity;
		float m_rotation;
		float m_steering;
		float m_bikeFriction;
		float m_turboFactor;
		float m_timeOfLastTurboInitiation;
		long double m_lastUpdateTime;
		BikeController* m_bikeController;
		long double m_timeSinceLastUpdate;
		std::shared_ptr<btRigidBody> m_bikeRigidBody;
	};
}