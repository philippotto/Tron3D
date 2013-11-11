#pragma once

#include "abstractmodel.h"
#include <osg/Referenced>
#include "../forwarddeclarations.h"

namespace troen
{

	class BikeModel : public AbstractModel, public osg::Referenced
	{
	public:
		BikeModel(input::BikeInputState* bikeInputState);
		void resetState();
		void updateState();
		void rotate(float angle);
		void accelerate(float velocity);
		float getRotation();
		float getVelocity();

		std::vector<btRigidBody> BikeModel::getRigidBodies();

	private:
		input::BikeInputState* m_bikeInputState;
		float m_velocity;
		float m_rotation;
	};
}