#include "hudcontroller.h"
// troen
#include "../player.h"
#include "../view/hudview.h"
#include "../constants.h"
#include "../controller/bikecontroller.h"
#include "../gamelogic.h"

using namespace troen;

HUDController::HUDController(const int id,
	const std::vector<std::shared_ptr<Player>>& players) :
AbstractController()
{
	m_view = std::static_pointer_cast<HUDView>(std::make_shared<HUDView>(id, players));
	m_player = players[id];
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
	const std::vector<std::shared_ptr<Player>>& players)
{
	std::shared_ptr<Player> player = m_player.lock();
	float speed = player->bikeController()->speed();
	std::static_pointer_cast<HUDView>(m_view)->setSpeedText(speed);
    std::static_pointer_cast<HUDView>(m_view)->updateRadarCamera();

	float health = player->health();
	std::static_pointer_cast<HUDView>(m_view)->setHealthText(100 * health / BIKE_DEFAULT_HEALTH);

	float points = player->points();
	std::static_pointer_cast<HUDView>(m_view)->setPointsText(points);

	BikeController::BIKESTATE bikeState = player->bikeController()->state();
	if (gameState == GameLogic::GAMESTATE::GAME_OVER)
	{
		std::static_pointer_cast<HUDView>(m_view)->setCountdownText("GameOver");
	}
	else if (bikeState == BikeController::BIKESTATE::RESPAWN || bikeState == BikeController::BIKESTATE::RESPAWN_PART_2)
	{
		double respawnTime = player->bikeController()->respawnTime();
		std::static_pointer_cast<HUDView>(m_view)->setCountdownText((int)(respawnTime - currentGameTime + RESPAWN_DURATION) / 1000 + 1);
	}
	else if (bikeState == BikeController::BIKESTATE::WAITING_FOR_GAMESTART)
	{
		double respawnTime = player->bikeController()->respawnTime();
		std::static_pointer_cast<HUDView>(m_view)->setCountdownText((int)(respawnTime - currentGameloopTime + GAME_START_COUNTDOWN_DURATION) / 1000 + 1);
	}
	else
	{
		std::static_pointer_cast<HUDView>(m_view)->setCountdownText(-1);
	}

	std::static_pointer_cast<HUDView>(m_view)->setTimeText(currentGameTime, timeLimit);

	//std::static_pointer_cast<HUDView>(m_view)->setDeathCountText(bikeController->deathCount());
	for (int i = 0; i < players.size(); i++)
	{
		std::static_pointer_cast<HUDView>(m_view)->setDeathCountText(i,players[i]->name(),players[i]->deathCount());
	}
}

void HUDController::setTrackNode(osg::Node* trackNode)
{
    std::static_pointer_cast<HUDView>(m_view)->setTrackNode(trackNode);
}