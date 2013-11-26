#pragma once
// STD
#include <memory>
// OSG
#include <osg/ref_ptr>
#include <osgGA/NodeTrackerManipulator>
// troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"

namespace troen
{
	class BikeController : public AbstractController
	{
	public:
		BikeController(const std::shared_ptr<sound::AudioManager>& audioManager);
		void setInputState(osg::ref_ptr<input::BikeInputState> &bikeInputState);
		void attachTrackingCamera(osg::ref_ptr<osgGA::NodeTrackerManipulator> &manipulator);
		void attachWorld(std::shared_ptr<PhysicsWorld> &world);

		void updateModel();

		const COLLISIONTYPE getCollisionType() { return BIKETYPE; };

		// getters
		virtual osg::ref_ptr<osg::Group> getViewNode() override;
		virtual std::vector<std::shared_ptr<btRigidBody>> getRigidBodies() override;
		const std::shared_ptr<sound::AudioManager> getAudioManager();

	private:
		std::shared_ptr<FenceController> m_fenceController;
		std::shared_ptr<sound::AudioManager> m_audioManager;
	};
}