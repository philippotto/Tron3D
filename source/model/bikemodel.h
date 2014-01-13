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
			std::shared_ptr<FenceController> fenceController,
			BikeController* bikeController);
		void setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState);
		void resetState();
		float updateState(long double time);
		void rotate(float angle);
		void accelerate(float velocity);
		float getRotation();
		float getVelocity();
		float getSteering();
		osg::Vec3d getPositionOSG();
		btVector3 getPositionBt();

		void moveBikeToPosition(btTransform position);

	private:
		osg::ref_ptr<input::BikeInputState> m_bikeInputState;
		float m_velocity;
		float m_rotation;
		float m_steering;
		float m_bikeFriction;
		long double m_lastUpdateTime;
		BikeController* m_bikeController;
	};
}