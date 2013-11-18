#include "bikecontroller.h"
//troen
#include "../input/bikeinputstate.h"
#include "../view/bikeview.h"
#include "../model/bikemodel.h"

using namespace troen;

BikeController::BikeController()
{
	// use static casts to convert from Abstract class type
	m_model = std::static_pointer_cast<BikeModel>(std::make_shared<BikeModel>());
	m_view = std::static_pointer_cast<BikeView>(std::make_shared<BikeView>(std::static_pointer_cast<BikeModel>(m_model)));
}

void BikeController::setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState)
{
	std::static_pointer_cast<BikeModel>(m_model)->setInputState(bikeInputState);
}

void BikeController::attachTrackingCamera(osg::ref_ptr<osgGA::NodeTrackerManipulator> manipulator)
{
	osg::Matrixd cameraOffset;
	cameraOffset.makeTranslate(0, -100, -20);

	manipulator->setTrackNode(getViewNode());
	manipulator->setHomePosition(
		std::static_pointer_cast<BikeModel>(m_model)->getPositionOSG(), 
		std::static_pointer_cast<BikeModel>(m_model)->getPositionOSG() * cameraOffset,
		osg::Vec3d(0, -1, 0)
	);
}