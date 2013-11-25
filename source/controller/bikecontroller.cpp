#include "bikecontroller.h"
// OSG
#include "osg/PositionAttitudeTransform"
//troen
#include "../input/bikeinputstate.h"
#include "../view/bikeview.h"
#include "../model/bikemodel.h"
#include "../controller/fencecontroller.h"
#include "../model/physicsworld.h"

using namespace troen;

BikeController::BikeController()
{
	// use static casts to convert from Abstract class type
	m_view = std::static_pointer_cast<BikeView>(std::make_shared<BikeView>());

	m_fenceController = std::make_shared<FenceController>();
	m_model = std::static_pointer_cast<BikeModel>(std::make_shared<BikeModel>(getViewNode(), m_fenceController));
}

void BikeController::setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState)
{
	std::static_pointer_cast<BikeModel>(m_model)->setInputState(bikeInputState);
}

void BikeController::attachTrackingCamera(osg::ref_ptr<osgGA::NodeTrackerManipulator> manipulator)
{
	osg::Matrixd cameraOffset;
	cameraOffset.makeTranslate(0, -100, -20);

	osg::PositionAttitudeTransform* pat = dynamic_cast<osg::PositionAttitudeTransform*> (getViewNode()->getChild(0));
	// set the actual node as the track node, not the pat
	manipulator->setTrackNode(pat->getChild(0));
	manipulator->setHomePosition(pat->getPosition(), pat->getPosition() * cameraOffset, osg::Vec3d(0, -1, 0));
}

void BikeController::updateModel()
{
	std::static_pointer_cast<BikeModel>(m_model)->updateState();
}

osg::ref_ptr<osg::Group> BikeController::getViewNode()
{
	osg::ref_ptr<osg::Group> group = std::static_pointer_cast<BikeView>(m_view)->getNode();
	group->addChild(m_fenceController->getViewNode());
	return group;
};

std::shared_ptr<std::vector<btRigidBody>> BikeController::getRigidBodies()
{
	std::shared_ptr<std::vector<btRigidBody>> bikeBodies = std::static_pointer_cast<BikeModel>(m_model)->getRigidBodies();
	return bikeBodies;
};

void BikeController::attachWorld(std::shared_ptr<PhysicsWorld> world) {
	
	world->addRigidBodies(getRigidBodies());

	m_fenceController->attachWorld(world);
}