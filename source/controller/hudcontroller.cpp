#include "hudcontroller.h"
// troen
#include "../view/hudview.h"
//#include "../model/hudmodel.h"

#include "../controller/bikecontroller.h"

using namespace troen;

HUDController::HUDController(std::shared_ptr<BikeController> bikeController)
{
	m_view = std::static_pointer_cast<HUDView>(std::make_shared<HUDView>());
	m_bikeController = bikeController;
	// TODO: is a model really necessary?
	// m_model = std::static_pointer_cast<HUDModel>(std::make_shared<HUDModel>());

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
	float speed = m_bikeController->getSpeed();
	std::static_pointer_cast<HUDView>(m_view)->setSpeedText(speed);
}