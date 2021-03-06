#include "player.h"
// Qt
#include <QString>
// OSG
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#ifdef WIN32
#include <osgViewer/config/SingleScreen>
#include <osgViewer/config/SingleWindow>
#endif
//troen
#include "constants.h"
#include "gameeventhandler.h"
#include "sampleosgviewer.h"

#include "controller/levelcontroller.h"

#include "input/bikeinputstate.h"
#include "network/networkmanager.h"

#include "view/skydome.h"
#include "view/reflection.h"
#include "view/nodefollowcameramanipulator.h"

#include "model/bikemodel.h"
#include "troengame.h"


using namespace troen;

// spawn point
// resource pool
Player::Player(
	TroenGame* game,
	const std::shared_ptr<GameConfig>& config,
	const int id) :
m_id(id),
m_name(config->playerNames[id].toStdString()),
m_health(BIKE_DEFAULT_HEALTH), //TODO: rename constants
m_points(0),
m_killCount(0),
m_deathCount(0),
m_hasGameView(config->ownView[id])
{

	m_troenGame = game;

	if (game->isNetworking())
		m_networkID = game->getNetworkManager()->getPlayerWithName(QString(m_name.c_str()))->networkID;
	
	////////////////////////////////////////////////////////////////////////////////
	//
	// Color
	//
	////////////////////////////////////////////////////////////////////////////////

	if (!game->isNetworking())
		m_color = osg::Vec3(config->playerColors[id].red(), config->playerColors[id].green(), config->playerColors[id].blue());
	else
	{
		QColor color = game->getNetworkManager()->getPlayerColor(m_networkID);
		m_color = osg::Vec3(color.red(), color.green(), color.blue());
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	// Controller
	//
	////////////////////////////////////////////////////////////////////////////////
	const btTransform initialTransform = game->levelController()->getSpawnPointForBikeWithIndex(id);

	m_bikeController = std::make_shared<BikeController>(
		this,
		(input::BikeInputState::InputDevice) config->playerInputTypes[m_id],
		initialTransform,
		game->resourcePool());

	m_fenceController = std::make_shared<FenceController>(this, initialTransform);

	// HUDController must be initialized later, because it
	// can only be created, once all Players are created

	////////////////////////////////////////////////////////////////////////////////
	//
	// View
	//
	////////////////////////////////////////////////////////////////////////////////

	if (config->ownView[m_id])
	{
		m_playerNode = new osg::Group();
		m_bikeController->addUniformsToPlayerNode();

		m_gameView = new osgViewer::View();
		m_gameView->getCamera()->setCullMask(CAMERA_MASK_MAIN);
		m_gameView->getCamera()->getOrCreateStateSet()->addUniform(new osg::Uniform("isReflecting", false));
		m_gameView->setSceneData(m_playerNode);

		osg::ref_ptr<NodeFollowCameraManipulator> manipulator
			= new NodeFollowCameraManipulator();

		m_bikeController->attachTrackingCamera(manipulator);
		m_bikeController->attachGameView(m_gameView);

		m_gameView->setCameraManipulator(manipulator.get());
		m_gameView->addEventHandler(game->gameEventHandler());
		m_gameView->addEventHandler(game->statsHandler());

#ifdef WIN32
		if (config->fullscreen)
			m_gameView->apply(new osgViewer::SingleScreen(0));
		else
			m_gameView->apply(new osgViewer::SingleWindow(400, 200, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));
#else
		if (config->fullscreen)
			m_gameView->setUpViewOnSingleScreen(0);
		else
			m_gameView->setUpViewInWindow(100, 100, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
#endif
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	// Viewer
	//
	////////////////////////////////////////////////////////////////////////////////

	if (config->ownView[m_id])
	{
		m_viewer = new SampleOSGViewer();
		m_viewer->addView(m_gameView);

#ifdef WIN32
		// turn of vSync (since we implement
		// an adaptive gameLoop that syncs itself)
		osg::ref_ptr<RealizeOperation> operation = new RealizeOperation;
		m_viewer->setRealizeOperation(operation);
		m_viewer->realize();
#endif
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	// Reflection
	//
	////////////////////////////////////////////////////////////////////////////////
	
	if (config->useReflection && config->ownView[id])
	{
		m_reflection = std::make_shared<Reflection>(game->levelController()->getFloorView(), m_gameView, game->skyDome()->getSkyboxTexture(), m_id);
		m_playerNode->getOrCreateStateSet()->addUniform(new osg::Uniform("reflectionTex", 4 + m_id));
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	// Networking
	//
	////////////////////////////////////////////////////////////////////////////////
	m_isRemote = false;
	if (game->isNetworking())
	{
		if (config->ownView[m_id] || config->playerInputTypes[m_id] == input::BikeInputState::AI)
		{
			game->getNetworkManager()->registerLocalPlayer(this);

		}
		else if (config->playerInputTypes[m_id] == input::BikeInputState::REMOTE_PLAYER)
		{
			m_isRemote = true;
			game->getNetworkManager()->registerRemotePlayerInput(m_networkID, m_bikeController->getRemote());

		}
		btTransform networkedTransform = game->getNetworkManager()->getStartPosition();
		m_bikeController->getModel()->moveBikeToPosition(networkedTransform);
	}




}

void Player::setupReflections(TroenGame* game, osg::ref_ptr<osg::Group>& sceneNode) {
	m_reflection = std::make_shared<Reflection>(game->levelController()->getFloorView(), m_gameView, game->skyDome()->getSkyboxTexture(), m_id);
	m_playerNode->getOrCreateStateSet()->addUniform(new osg::Uniform("reflectionTex", 5 + m_id));

	reflection()->addSceneNode(sceneNode);
	playerNode()->addChild(reflection()->getReflectionCameraGroup());
}

void Player::createHUDController(const std::vector<std::shared_ptr<Player>>& players)
{
	if (!hasGameView()) return;

	m_HUDController = std::make_shared<HUDController>(m_id, players);
	m_bikeController->attachTrackingCamera(m_HUDController);
}

void Player::update(int g_gameTime)
{
	bikeController()->updateModel(g_gameTime);
}

Player::~Player()
{
	//
	// controllers
	//
	m_bikeController.reset();
	m_fenceController.reset();
	m_HUDController.reset();

	//
	// osg elements
	//
	m_viewer = nullptr;
	m_gameView = nullptr;
	m_playerNode = nullptr;
	m_reflection = nullptr;
}

float Player::increaseHealth(float diff)
{
	m_health = clamp(0, BIKE_DEFAULT_HEALTH, m_health + diff);
	return m_health;
}

float Player::increasePoints(float diff)
{
	m_points += diff;
	return m_points;
}

bool Player::isDead()
{
	return m_health <= 0 && bikeController()->state() == BikeController::BIKESTATE::DRIVING;
}