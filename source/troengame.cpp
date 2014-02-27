#include "troengame.h"
// OSG
#include <osg/LineWidth>
#include <osgUtil/Optimizer>

// troen
#include "constants.h"
#include "sampleosgviewer.h"
#include "gamelogic.h"
#include "player.h"
#include "gameeventhandler.h"

#include "input/bikeinputstate.h"
#include "input/keyboard.h"
#include "input/gamepad.h"

#include "util/chronotimer.h"
#include "util/gldebugdrawer.h"
#include "sound/audiomanager.h"

#include "model/physicsworld.h"
#include "model/abstractmodel.h"
#include "controller/levelcontroller.h"
#include "controller/bikecontroller.h"
#include "controller/fencecontroller.h"
#include "controller/levelcontroller.h"
#include "controller/hudcontroller.h"
#include "view/shaders.h"
#include "view/postprocessing.h"
#include "view/nodefollowcameramanipulator.h"
#include "view/reflection.h"


#include "globals.h"


using namespace troen;
extern long double g_currentTime;

TroenGame::TroenGame(QThread* thread /*= nullptr*/) :
QObject(),
m_gameThread(thread),
m_gameConfig(nullptr)
{
	if (m_gameThread == nullptr) {
		m_gameThread = new QThread(this);
	}
	moveToThread(m_gameThread);
	m_gameThread->start(QThread::HighestPriority);
}

bool TroenGame::initialize()
{
	m_rootNode = new osg::Group;

	// careful about the order of initialization
	osg::DisplaySettings::instance()->setNumMultiSamples(NUM_MULTISAMPLES);

	std::cout << "[TroenGame::initialize] initializing game ..." << std::endl;

	std::cout << "[TroenGame::initialize] timer ..." << std::endl;
	m_gameloopTimer = std::make_shared<util::ChronoTimer>(false, true);
	m_gameTimer = std::make_shared<util::ChronoTimer>(false, true);

	std::cout << "[TroenGame::initialize] shaders ..." << std::endl;
	shaders::reloadShaders();

	std::cout << "[TroenGame::initialize] sound ..." << std::endl;
	{
		m_audioManager = std::shared_ptr<sound::AudioManager>(new sound::AudioManager);
		m_audioManager->LoadSFX("data/sound/explosion.wav");
		m_audioManager->LoadSong("data/sound/theGameHasChanged.mp3");
		m_audioManager->LoadEngineSound();
		m_audioManager->SetSongsVolume(0.5);

		m_audioManager->PlaySong("data/sound/theGameHasChanged.mp3");
		m_audioManager->PlayEngineSound();
		m_audioManager->SetMasterVolume(0.f);
	}

	std::cout << "[TroenGame::initialize] controllers (models & views) ..." << std::endl;
	m_skyDome = new SkyDome;
	{ // event handlers
		m_statsHandler = new osgViewer::StatsHandler;
		m_statsHandler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_T);
		m_statsHandler->setKeyEventPrintsOutStats(osgGA::GUIEventAdapter::KEY_P);
		m_statsHandler->setKeyEventToggleVSync(osgGA::GUIEventAdapter::KEY_V);

		m_gameEventHandler = new GameEventHandler(this);
	}

	{ // controllers
		m_levelController = std::make_shared<LevelController>();
		for (int i = 0; i < m_gameConfig->numberOfPlayers; i++)
		{
			std::shared_ptr<Player> player = std::make_shared<Player>(this, m_gameConfig, i);
			m_players.push_back(player);
			if (m_gameConfig->ownView[i])
			{
				m_playersWithView.push_back(player);
			}
		}
		for (auto player : m_players)
		{
			player->createHUDController(m_players);
		}
	}

	std::cout << "[TroenGame::initialize] gameLogic ..." << std::endl;
	m_gameLogic = std::make_shared<GameLogic>(this, m_audioManager, m_levelController, m_players, m_gameConfig->timeLimit);
	m_gameEventHandler->attachGameLogic(m_gameLogic);

	std::cout << "[TroenGame::initialize] postprocessing & scenegraph ..." << std::endl;
	composeSceneGraph();
	std::cout << "[TroenGame::initialize] input ..." << std::endl;
	initializeInput();
	std::cout << "[TroenGame::initialize] physics ..." << std::endl;
	initializePhysicsWorld();
	m_physicsWorld->stepSimulation(0);

	std::cout << "[TroenGame::initialize] successfully initialized !" << std::endl;
	return true;
}

bool TroenGame::composeSceneGraph()
{
	if (m_gameConfig->usePostProcessing)
	{
		// viewport of all windows has to be equal since only
		// the first is taken for PostProcessing Texture Sizes
		osg::Viewport * viewport =
			m_players[0]->gameView()->getCamera()->getViewport();

		m_postProcessing = std::make_shared<PostProcessing>(m_rootNode, viewport->width(), viewport->height());

		m_sceneNode = m_postProcessing->getSceneNode();

		//explicit call, to enable glow from start
		resize(viewport->width(), viewport->height());
	}
	else
	{
		m_sceneNode = m_rootNode;
	}

	for (auto player : m_playersWithView)
	{
			player->playerNode()->addChild(m_rootNode);
	}

	m_skyDome->getOrCreateStateSet()->setRenderBinDetails(-1, "RenderBin");
	m_sceneNode->addChild(m_skyDome.get());

	m_sceneNode->addChild(m_levelController->getViewNode());
	//m_sceneNode->addChild(m_sunLightSource.get());

	for (auto player : m_players)
	{
		m_sceneNode->addChild(player->bikeController()->getViewNode());
		m_sceneNode->addChild(player->fenceController()->getViewNode());
	}

	m_sceneNode->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::ON);

	if (m_gameConfig->useReflection)
	{
		//sceneNode has to be added to reflection after adding all (non hud) objects
		for (auto player : m_playersWithView)
		{
			player->reflection()->addSceneNode(m_sceneNode);
			player->playerNode()->addChild(player->reflection()->getReflectionCameraGroup());
		}
	}

	for (auto player : m_playersWithView)
	{
			osg::Group * node = player->hudController()->getViewNode();
			osg::Group * playerNode = player->playerNode();
			playerNode->addChild(node);
	}


	if (m_gameConfig->usePostProcessing)
		m_rootNode->addChild(m_sceneNode);

	osg::ref_ptr<osg::Group> radarScene = new osg::Group;

	for (auto player : m_players)
	{
		radarScene->addChild(player->bikeController()->getViewNode());
	}
	radarScene->addChild(m_levelController->getViewNode());

	for (auto player : m_playersWithView)
	{
		player->hudController()->attachSceneToRadarCamera(radarScene);
	}

	std::cout << "[TroenGame::composeSceneGraph] starting Optimizer" << std::endl;
	osgUtil::Optimizer optimizer;
	optimizer.optimize(m_rootNode, optimizer.REMOVE_REDUNDANT_NODES |
		optimizer.TRISTRIP_GEOMETRY | optimizer.OPTIMIZE_TEXTURE_SETTINGS | 
		optimizer.VERTEX_POSTTRANSFORM | optimizer.INDEX_MESH);
	std::cout << "[TroenGame::composeSceneGraph] done optimizing" << std::endl;

	return true;
}

bool TroenGame::initializeInput()
{
	for (auto player : m_players)
	{
		// attach all keyboard handlers to all gameViews
		if (player->bikeController()->hasKeyboardHandler())
		{
			for (auto otherPlayer : m_playersWithView)
			{
					otherPlayer->gameView()->addEventHandler(player->bikeController()->keyboardHandler());
			}
		}
	}
	return true;
}

bool TroenGame::initializePhysicsWorld()
{
	m_physicsWorld = std::make_shared<PhysicsWorld>(m_gameLogic, m_gameConfig->useDebugView);
	m_physicsWorld->addRigidBodies(m_levelController->getRigidBodies(), COLGROUP_LEVEL, COLMASK_LEVEL);

	// attach world
	for (auto player : m_players)
	{
		player->bikeController()->attachWorld(m_physicsWorld);
		player->fenceController()->attachWorld(m_physicsWorld);
	}
	m_levelController->attachWorld(m_physicsWorld);
	m_gameLogic->attachPhysicsWorld(m_physicsWorld);
	return true;
}

void TroenGame::prepareAndStartGame(const GameConfig& gameConfig)
{
	m_gameConfig = std::make_shared<GameConfig>(gameConfig);
	startGameLoop();
}

void TroenGame::startGameLoop()
{
	// adaptive game loop from here:
	// http://entropyinteractive.com/2011/02/game-engine-design-the-game-loop/

	// INITIALIZATION
	initialize();

	if (m_gameConfig->useDebugView)
		m_sceneNode->addChild(m_physicsWorld->m_debug->getSceneGraph());

	if (m_gameConfig->fullscreen)
		setupForFullScreen();

	m_gameloopTimer->start();
	m_gameTimer->start();
	m_gameTimer->pause();

	// GAME LOOP VARIABLES
	long double nextTime = m_gameloopTimer->elapsed();
	const double minMillisecondsBetweenFrames = 16.7; // vSync to 60 fps
	const double maxMillisecondsBetweenFrames = 4 * minMillisecondsBetweenFrames + 1;
	int skippedFrames = 0;
	const int maxSkippedFrames = 4;

	bool nearPlaneAdapted = false;

	// GAME LOOP
	// - AI
	// - network
	// - checkForUserInput and updateModels
	// - physics + updateViews
	// - render;

	// terminates when first viewer is closed
	while (!m_players[0]->viewer()->done())
	{
		g_gameLoopTime = m_gameloopTimer->elapsed();
		g_gameTime = m_gameTimer->elapsed();

		// are we significantly behind? if yes, "resync", force rendering
		if ((g_gameLoopTime - nextTime) > maxMillisecondsBetweenFrames)
			nextTime = g_gameLoopTime;
		// is it time to render the next frame?
		if (m_gameConfig->testPerformance || g_gameLoopTime >= nextTime)
		{
			// assign the time for the next update
			nextTime += minMillisecondsBetweenFrames;

			// LOOP REALLY STARTS HERE:
			m_gameLogic->step(g_gameLoopTime, g_gameTime);
			if (!m_gameTimer->paused())
			{
				for (auto player : m_players)
				{
					player->bikeController()->updateModel(g_gameTime);
				}
				m_physicsWorld->stepSimulation(g_gameTime);
				m_levelController->update();
			}

			m_audioManager->Update(g_gameLoopTime / 1000);
			m_audioManager->setMotorSpeed(m_players[0]->bikeController()->speed());

			if (m_postProcessing) m_postProcessing->setBeat(m_audioManager->getTimeSinceLastBeat());

			// do we have extra time (to draw the frame) or did we skip too many frames already?
			if (g_gameLoopTime < nextTime || (skippedFrames > maxSkippedFrames))
			{
				for (auto player : m_playersWithView)
				{
					player->hudController()->update(
						g_gameLoopTime,
						g_gameTime,
						m_gameConfig->timeLimit,
						m_gameLogic->getGameState(),
						m_players);
				}

				for (auto player : m_playersWithView)
				{
					player->viewer()->frame();
				}
				// TODO: find a way to eleminate this workaround
				// doesn't work if it's executed earlier
				if (!nearPlaneAdapted)
				{
					for (auto player : m_playersWithView)
					{
						fixCulling(player->gameView());
					}
				}
				skippedFrames = 0;
			}
			else
				skippedFrames++;
		}
		else // WAIT
		{
			// calculate the time to sleep
			long double sleepTime = (nextTime - g_gameLoopTime);
			if (sleepTime > 0)	// sanity check, sleep until nextTime
			if (!m_gameConfig->testPerformance) m_gameThread->msleep(sleepTime);
		}
	}

	if (m_gameConfig->fullscreen)
		returnFromFullScreen();

	// SHUTDOWN
	shutdown();
}

bool TroenGame::shutdown()
{
	// clean up in reverse order from initialization

	//timer
	m_gameloopTimer.reset();
	m_gameTimer.reset();

	//input
	input::Gamepad::clearPorts();
	m_statsHandler = nullptr;

	// physics & gamelogic
	m_physicsWorld.reset();
	m_gameLogic.reset();

	// models & scenegraph
	m_rootNode = nullptr;
	m_levelController.reset();

	for (auto player : m_players)
		player.reset();
	m_players.clear();

	// sound
	m_audioManager->StopSFXs();
	m_audioManager->StopSongs();
	m_audioManager.reset();

	// shaders
	shaders::m_allShaderPrograms.clear();

	std::cout << "[TroenGame::shutdown] shutdown complete " << std::endl;
	return true;
}

void TroenGame::fixCulling(osg::ref_ptr<osgViewer::View>& view)
{
	double fovy, aspect, znear, zfar;
	view->getCamera()->getProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
	view->getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	znear = 1.0;
	view->getCamera()->setProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
}

void TroenGame::setupForFullScreen()
{
	osg::GraphicsContext::WindowingSystemInterface* wsi =
	osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi)
	{
		std::cout << "[TroenGame::setupForFullScreen] error ..." << std::endl;
		return;
	}
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), m_originalWidth, m_originalHeight);

	//wsi->setScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), 1024, 768);
	this->resize(m_originalWidth, m_originalHeight);
}

void TroenGame::returnFromFullScreen()
{
	osg::GraphicsContext::WindowingSystemInterface* wsi =
		osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi)
	{
		std::cout << "[TroenGame::returnFromFullScreen] error ..." << std::endl;
		return;
	}
	wsi->setScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), m_originalWidth, m_originalHeight);
}

void TroenGame::switchSoundVolumeEvent()
{
	m_audioManager->SetMasterVolume(1 - m_audioManager->GetMasterVolume());
}

void TroenGame::pauseEvent()
{
	if (m_gameLogic->getGameState() == GameLogic::GAMESTATE::GAME_START)
		return;
	else if (!m_gameTimer->paused())
		pauseSimulation();
	else
		unpauseSimulation();
}

void TroenGame::pauseSimulation()
{
	if (!m_gameTimer->paused()) m_gameTimer->pause();
}

void TroenGame::unpauseSimulation()
{
	if (m_gameTimer->paused()) m_gameTimer->start();
}

void TroenGame::resize(int width, int height){
	if (m_postProcessing){
		m_postProcessing->setupTextures(width, height);
	}

	for (auto player : m_playersWithView)
	{
			player->hudController()->resize(width, height);
	}
}

osg::ref_ptr<GameEventHandler> TroenGame::getGameEventHandler()
{
	return m_gameEventHandler;
}