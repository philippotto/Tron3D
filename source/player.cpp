#include "player.h"
// STD
// Qt
#include <QString>
// bullet
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

#include "controller/bikecontroller.h"
#include "controller/levelcontroller.h"
#include "controller/hudcontroller.h"

#include "input/bikeinputstate.h"

#include "view/skydome.h"
#include "view/reflection.h"
#include "view/nodefollowcameramanipulator.h"


using namespace troen;

// spawn point
// resource pool
Player::Player(
	TroenGame* game,
	const std::shared_ptr<GameConfig>& config,
	const int id) :
m_health(BIKE_DEFAULT_HEALTH), //TODO: rename constants
m_points(0),
m_killCount(0),
m_deathCount(0),
m_name(config->playerNames[id].toStdString()),
m_id(id)
{
	m_color = osg::Vec3(config->playerColors[id].red(), config->playerColors[id].green(), config->playerColors[id].blue());

	////////////////////////////////////////////////////////////////////////////////
	//
	// Controller
	//
	////////////////////////////////////////////////////////////////////////////////
	const btTransform initialTransform = game->getLevelController()->getSpawnPointForBikeWithIndex(id);

	m_bikeController = std::make_shared<BikeController>(
		this,
		(input::BikeInputState::InputDevice) config->playerInputTypes[m_id],
		initialTransform,
		&(game->getResourcePool()),
		config->ownView[m_id]);

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
		m_bikeController->setPlayerNode(m_playerNode);

		m_gameView = new osgViewer::View();
		m_gameView->getCamera()->setCullMask(CAMERA_MASK_MAIN);
		m_gameView->setSceneData(m_playerNode);

		osg::ref_ptr<NodeFollowCameraManipulator> manipulator
			= new NodeFollowCameraManipulator();

		m_bikeController->attachTrackingCamera(manipulator);
		m_bikeController->attachGameView(m_gameView);

		m_gameView->setCameraManipulator(manipulator.get());
		m_gameView->addEventHandler(game->getGameEventHandler());
		m_gameView->addEventHandler(game->getStatsHandler());

#ifdef WIN32
		if (config->fullscreen)
			m_gameView->apply(new osgViewer::SingleScreen(0));
		else
			m_gameView->apply(new osgViewer::SingleWindow(400, 200, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));
#else
		if (m_fullscreen)
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
	
	if (m_bikeController->hasGameView())
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
		m_reflection = std::make_shared<Reflection>(game->getLevelController()->getFloorView(), m_gameView, game->getSkyDome()->getSkyboxTexture(), m_id);
		m_playerNode->getOrCreateStateSet()->addUniform(new osg::Uniform("reflectionTex", 4 + m_id));
	}
}

void Player::createHUDController(const std::vector<std::shared_ptr<Player>>& players)
{
	if (!m_bikeController->hasGameView()) return;

	m_HUDController = std::make_shared<HUDController>(m_id, players);
	m_bikeController->attachTrackingCamera(m_HUDController);
}

Player::~Player()
{
	//TODO
	// DESTRUCT
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