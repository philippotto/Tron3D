#pragma once

#include "abstractmodel.h"

#include <osg/Referenced>
#include <osg/ref_ptr>

#include "../forwarddeclarations.h"

namespace troen
{

	class BikeModel : public AbstractModel, public osg::Referenced
	{
	public:
		BikeModel(osg::ref_ptr<input::BikeInputState> bikeInputState);
		void resetState();
		void updateState();
		void rotate(float angle);
		void accelerate(float velocity);
		float getRotation();
		float getVelocity();

		std::vector<btRigidBody> BikeModel::getRigidBodies();

	private:
		osg::ref_ptr<input::BikeInputState> m_bikeInputState;
		float m_velocity;
		float m_rotation;
	};
}