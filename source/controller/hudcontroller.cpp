#include "hudcontroller.h"
// troen
#include "../view/hudview.h"
#include "../controller/bikecontroller.h"

using namespace troen;

HUDController::HUDController(std::shared_ptr<BikeController> bikeController) : AbstractController()
{
	m_view = std::static_pointer_cast<HUDView>(std::make_shared<HUDView>());
	m_bikeController = bikeController;
}

void HUDController::resize(int width, int height)
{
	std::static_pointer_cast<HUDView>(m_view)->resize(width, height);
}

void HUDController::attachSceneToRadarCamera(osg::Group* scene)
{
	std::static_pointer_cast<HUDView>(m_view)->attachSceneToRadarCamera(scene);
}

void HUDController::update() {
	float speed = m_bikeController.lock()->getSpeed();
	std::static_pointer_cast<HUDView>(m_view)->setSpeedText(speed);
    std::static_pointer_cast<HUDView>(m_view)->updateRadarCamera();
}

void HUDController::setTrackNode(osg::Node* trackNode)
{
    std::static_pointer_cast<HUDView>(m_view)->setTrackNode(trackNode);
}
