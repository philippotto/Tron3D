#include "hudcontroller.h"
// troen
#include "../view/hudview.h"
#include "../constants.h"
#include "../controller/bikecontroller.h"
#include "../gamelogic.h"

using namespace troen;

HUDController::HUDController(const int i, const std::vector<std::shared_ptr<BikeController>>& bikeControllers) : AbstractController()
{
	m_view = std::static_pointer_cast<HUDView>(std::make_shared<HUDView>(i, bikeControllers));
	m_bikeController = bikeControllers[i];
}

void HUDController::resize(const int width, const int height)
{
	std::static_pointer_cast<HUDView>(m_view)->resize(width, height);
}

void HUDController::attachSceneToRadarCamera(osg::Group* scene)
{
	std::static_pointer_cast<HUDView>(m_view)->attachSceneToRadarCamera(scene);
}

void HUDController::update(
	const long double currentGameloopTime,
	const long double currentGameTime,
	const int timeLimit,
	const GameLogic::GAMESTATE gameState,
	const std::vector<std::shared_ptr<BikeController>>& bikeControllers)
{
	std::shared_ptr<BikeController> bikeController = m_bikeController.lock();
	float speed = bikeController->getSpeed();
	std::static_pointer_cast<HUDView>(m_view)->setSpeedText(speed);
    std::static_pointer_cast<HUDView>(m_view)->updateRadarCamera();

	float health = bikeController->getHealth();
	std::static_pointer_cast<HUDView>(m_view)->setHealthText(100 * health / BIKE_DEFAULT_HEALTH);

	float points = bikeController->getPoints();
	std::static_pointer_cast<HUDView>(m_view)->setPointsText(points);

	BikeController::BIKESTATE bikeState = bikeController->getState();
	if (gameState == GameLogic::GAMESTATE::GAME_OVER)
	{
		std::static_pointer_cast<HUDView>(m_view)->setCountdownText("GameOver");
	}
	else if (bikeState == BikeController::BIKESTATE::RESPAWN || bikeState == BikeController::BIKESTATE::RESPAWN_PART_2)
	{
		double respawnTime = bikeController->getRespawnTime();
		std::static_pointer_cast<HUDView>(m_view)->setCountdownText((int)(respawnTime - currentGameTime + RESPAWN_DURATION) / 1000 + 1);
	}
	else if (bikeState == BikeController::BIKESTATE::WAITING_FOR_GAMESTART)
	{
		double respawnTime = bikeController->getRespawnTime();
		std::static_pointer_cast<HUDView>(m_view)->setCountdownText((int)(respawnTime - currentGameloopTime + GAME_START_COUNTDOWN_DURATION) / 1000 + 1);
	}
	else
	{
		std::static_pointer_cast<HUDView>(m_view)->setCountdownText(-1);
	}

	std::static_pointer_cast<HUDView>(m_view)->setTimeText(currentGameTime, timeLimit);

	//std::static_pointer_cast<HUDView>(m_view)->setDeathCountText(bikeController->getDeathCount());
	for (int i = 0; i < bikeControllers.size(); i++)
	{
		std::static_pointer_cast<HUDView>(m_view)->setDeathCountText(i,bikeControllers[i]->getPlayerName(),bikeControllers[i]->getDeathCount());
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
