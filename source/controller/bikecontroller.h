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
		BikeController();
		void setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState);
		void attachTrackingCamera(osg::ref_ptr<osgGA::NodeTrackerManipulator> manipulator);
		void updateModel();
		virtual osg::ref_ptr<osg::Group> getViewNode() override;
		virtual std::shared_ptr<std::vector<btRigidBody>> getRigidBodies() override;

	private:
		std::shared_ptr<FenceController> m_fenceController;
	};
}