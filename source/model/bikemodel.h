#pragma once
// OSG
#include <osg/Referenced>
#include <osg/ref_ptr>
// troen
#include "../forwarddeclarations.h"
#include "abstractmodel.h"

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

		std::shared_ptr<std::vector<btRigidBody>> BikeModel::getRigidBodies();

	private:
		osg::ref_ptr<input::BikeInputState> m_bikeInputState;
		float m_velocity;
		float m_rotation;
		std::shared_ptr<std::vector<btRigidBody>> m_rigidBodies;

	};
}