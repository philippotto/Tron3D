#include "hudcontroller.h"
// troen
#include "../view/hudview.h"
#include "../constants.h"
#include "../controller/bikecontroller.h"

using namespace troen;

HUDController::HUDController(std::shared_ptr<BikeController> bikeController, const osg::Vec4 playerColor) : AbstractController()
{
	m_view = std::static_pointer_cast<HUDView>(std::make_shared<HUDView>(playerColor));
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

void HUDController::update(double currentTime)
{
	float speed = m_bikeController.lock()->getSpeed();
	std::static_pointer_cast<HUDView>(m_view)->setSpeedText(speed);
    std::static_pointer_cast<HUDView>(m_view)->updateRadarCamera();

	float health = m_bikeController.lock()->getHealth();
	std::static_pointer_cast<HUDView>(m_view)->setHealthText(100 * health / BIKE_DEFAULT_HEALTH);

	float points = m_bikeController.lock()->getPoints();
	std::static_pointer_cast<HUDView>(m_view)->setPointsText(points);

	BikeController::BIKESTATE state = m_bikeController.lock()->getState();
	if (state == BikeController::BIKESTATE::RESPAWN || state == BikeController::BIKESTATE::WAITING)
	{
		double respawnTime = m_bikeController.lock()->getRespawnTime();
		std::static_pointer_cast<HUDView>(m_view)->setCountdownText((int)(respawnTime - currentTime + 3000)/1000 + 1);
	}
	else
	{
		std::static_pointer_cast<HUDView>(m_view)->setCountdownText(-1);
	}
}

void HUDController::setTrackNode(osg::Node* trackNode)
{
    std::static_pointer_cast<HUDView>(m_view)->setTrackNode(trackNode);
}

std::weak_ptr<BikeController> HUDController::getBikeController()
{
	return m_bikeController;
}
