#include "hudcontroller.h"
// troen
#include "../view/hudview.h"
#include "../constants.h"
//#include "../model/hudmodel.h"

#include "../controller/bikecontroller.h"

using namespace troen;

HUDController::HUDController(std::shared_ptr<BikeController> bikeController)
{
	AbstractController();
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
	float speed = m_bikeController.lock()->getSpeed();
	std::static_pointer_cast<HUDView>(m_view)->setSpeedText(speed);

	float health = m_bikeController.lock()->getHealth();
	std::static_pointer_cast<HUDView>(m_view)->setHealthText(100 * health / BIKE_DEFAULT_HEALTH);

	float points = m_bikeController.lock()->getPoints();
	std::static_pointer_cast<HUDView>(m_view)->setPointsText(100 * points);

}