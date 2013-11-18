#pragma once
// OSG
#include <osg/ref_ptr>
#include <osg/Vec3d>
// troen
#include "../forwarddeclarations.h"
#include "abstractmodel.h"

namespace troen
{

	class BikeModel : public AbstractModel
	{
	public:
		BikeModel();
		void setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState);
		void resetState();
		void updateState();
		void rotate(float angle);
		void accelerate(float velocity);
		float getRotation();
		float getVelocity();
		osg::Vec3d getPositionOSG();

	private:
		osg::ref_ptr<input::BikeInputState> m_bikeInputState;
		float m_velocity;
		float m_rotation;

	};
}