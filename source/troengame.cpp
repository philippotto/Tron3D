#include "troengame.h"
// OSG
#include <osgViewer/ViewerEventHandlers>
#include <osg/LineWidth>
#include <osgUtil/Optimizer>

#ifdef WIN32
#include <osgViewer/config/SingleScreen>
#include <osgViewer/config/SingleWindow>
#endif
// troen
#include "constants.h"
#include "sampleosgviewer.h"
#include "gameeventhandler.h"
#include "gamelogic.h"

#include "input/bikeinputstate.h"
#include "input/keyboard.h"

#include "util/chronotimer.h"
#include "util/gldebugdrawer.h"
#include "sound/audiomanager.h"

#include "model/physicsworld.h"
#include "model/abstractmodel.h"
#include "controller/levelcontroller.h"
#include "controller/bikecontroller.h"
#include "controller/levelcontroller.h"
#include "controller/hudcontroller.h"
#include "view/shaders.h"
#include "view/skydome.h"
#include "view/postprocessing.h"
#include "view/nodefollowcameramanipulator.h"
#include "view/reflection.h"


#include "globals.h"


using namespace troen;
extern long double g_currentTime;

TroenGame::TroenGame(QThread* thread /*= nullptr*/) :
m_gameThread(thread),
m_numberOfBikes(0),
m_timeLimit(0),
m_fullscreen(false),
m_usePostProcessing(false),
m_testPerformance(false),
m_useReflection(false)
{
	QObject();

	if (m_gameThread == nullptr) {
		m_gameThread = new QThread(this);
	}
	moveToThread(m_gameThread);
	m_gameThread->start(QThread::HighestPriority);
}

TroenGame::~TroenGame()
{
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
	if(!m_gameTimer->paused()) m_gameTimer->pause();
}

void TroenGame::unpauseSimulation()
{
	if(m_gameTimer->paused()) m_gameTimer->start();
}

void TroenGame::resize(int width, int height){
	if (m_postProcessing){
		m_postProcessing->setupTextures(width, height);
	}

	for (auto hudController : m_HUDControllers)
	{
		hudController->resize(width, height);
	}
}


void TroenGame::prepareAndStartGame(const GameConfig& config)
{
	m_numberOfBikes = config.numberOfBikes;
	m_timeLimit = config.timeLimit;
	m_fullscreen = config.fullscreen;
	m_usePostProcessing = config.usePostProcessing;
	m_useDebugView = config.useDebugView;
	m_testPerformance = config.testPerformance;
	m_useReflection = config.reflection;

	m_playerInputTypes.clear();
	m_playerColors.clear();
	m_reflections.clear();
	for (int i = 0; i < m_numberOfBikes; i++)
	{
		//input
		m_playerInputTypes.push_back(config.playerInputTypes[i]);
		//color
		osg::Vec3 col = osg::Vec3(config.playerColors[i].red(), config.playerColors[i].green(), config.playerColors[i].blue());
		m_playerColors.push_back(col);
		//name
		QString playerName = config.playerNames[i];
		m_playerNames.push_back(playerName);
	}

	for (int i = 0; i < MAX_BIKES; i++) {
		m_ownView[i] = config.ownView[i];
	}

	startGameLoop();
}

bool TroenGame::initialize()
{
	m_rootNode = new osg::Group;

	// careful about the order of initialization
	osg::DisplaySettings::instance()->setNumMultiSamples(NUM_MULTISAMPLES);

	std::cout << "[TroenGame::initialize] initializing game ..." << std::endl;

	std::cout << "[TroenGame::initialize] timer ..." << std::endl;
	initializeTimers();

	std::cout << "[TroenGame::initialize] shaders ..." << std::endl;
	initializeShaders();

	std::cout << "[TroenGame::initialize] sound ..." << std::endl;
	initializeSound();

	std::cout << "[TroenGame::initialize] controllers (models & views) ..." << std::endl;
	initializeSkyDome();
	initializeControllers();
	//initializeLighting();

	std::cout << "[TroenGame::initialize] gameLogic ..." << std::endl;
	initializeGameLogic();

	std::cout << "[TroenGame::initialize] views & viewer ..." << std::endl;
	initializeViews();
	initializeViewer();
	initializeReflection();

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

bool TroenGame::initializeTimers()
{
	m_gameloopTimer = std::make_shared<util::ChronoTimer>(false, true);
	m_gameTimer = std::make_shared<util::ChronoTimer>(false, true);
	return true;
}

bool TroenGame::initializeShaders()
{
	shaders::reloadShaders();
	return true;
}

bool TroenGame::initializeSound()
{
	m_audioManager = std::shared_ptr<sound::AudioManager>(new sound::AudioManager);
	m_audioManager->LoadSFX("data/sound/explosion.wav");
	m_audioManager->LoadSong("data/sound/theGameHasChanged.mp3");
	m_audioManager->LoadEngineSound();
	m_audioManager->SetSongsVolume(0.5);

	m_audioManager->PlaySong("data/sound/theGameHasChanged.mp3");
	m_audioManager->PlayEngineSound();
	m_audioManager->SetMasterVolume(0.f);

	return true;
}

bool TroenGame::initializeSkyDome()
{
	m_skyDome = new SkyDome;
	return true;
}


bool TroenGame::initializeControllers()
{
	m_levelController = std::make_shared<LevelController>();

	for (int i = 0; i < m_numberOfBikes; i++)
	{
		m_bikeControllers.push_back(std::make_shared<BikeController>(
			(input::BikeInputState::InputDevice)m_playerInputTypes[i],
			m_levelController->getSpawnPointForBikeWithIndex(i),
			m_playerColors[i],
			m_playerNames[i].toStdString(),
			&m_resourcePool, m_ownView[i],i)
		);
	}

	for (int i = 0; i < m_bikeControllers.size(); i++) {
		// only attach a HUD if a corresponding gameview exists
		if (m_bikeControllers[i]->hasGameView())
		{
			m_HUDControllers.push_back(std::make_shared<HUDController>(i,m_bikeControllers));
		}
	}

	return true;
}

bool TroenGame::initializeLighting()
{
	osg::ref_ptr<osg::Light> sunLight = new osg::Light(0);
	sunLight->setPosition(osg::Vec4f(0.0f, 0.0f, 300.0f, 1.0f));
	sunLight->setAmbient(osg::Vec4f(0.05f, 0.05f, 0.04f, 1.0f));
	sunLight->setDiffuse(osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
	sunLight->setSpecular(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

	osg::ref_ptr<osg::LightSource> m_sunLightSource = new osg::LightSource;
	m_sunLightSource->setLight(sunLight.get());
	return true;
}

bool TroenGame::initializeGameLogic()
{
	m_gameLogic = std::make_shared<GameLogic>(this, m_audioManager, m_levelController, m_bikeControllers, m_timeLimit);
	return true;
}

bool TroenGame::initializeReflection()
{
	if (m_useReflection)
	{
		for (int playerID = 0; playerID < m_playerNodes.size(); playerID++)
		{
			m_reflections.push_back(std::make_shared<Reflection>(m_levelController->getFloorView(), m_gameViews[playerID], m_skyDome->getSkyboxTexture(),playerID));
			m_playerNodes[playerID]->getOrCreateStateSet()->addUniform(new osg::Uniform("reflectionTex", 4 + playerID));

		}
	}
	return true;
}


bool TroenGame::initializeViews()
{
	m_statsHandler = new osgViewer::StatsHandler;
	m_statsHandler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_T);
	m_statsHandler->setKeyEventPrintsOutStats(osgGA::GUIEventAdapter::KEY_P);
	m_statsHandler->setKeyEventToggleVSync(osgGA::GUIEventAdapter::KEY_V);

	m_gameEventHandler = new GameEventHandler(this, m_gameLogic);

	// iterate over hudcontrollers because they only exist, if the corresponding bike has an own gameView
	for (int i = 0; i < m_HUDControllers.size(); i++)
	{
		std::weak_ptr<BikeController> bikeController = m_HUDControllers[i]->getBikeController();

		// TODO: is there a better place for this?
		osg::Group* playerNode = new osg::Group();
		m_playerNodes.push_back(playerNode);
		bikeController.lock()->setPlayerNode(playerNode);

		osg::ref_ptr<osgViewer::View> newGameView = new osgViewer::View();
		newGameView->getCamera()->setCullMask(CAMERA_MASK_MAIN);
		newGameView->setSceneData(playerNode);

		osg::ref_ptr<NodeFollowCameraManipulator> manipulator
			= new NodeFollowCameraManipulator();

		bikeController.lock()->attachTrackingCameras(manipulator, m_HUDControllers[i]);
		bikeController.lock()->attachGameView(newGameView);

		newGameView->setCameraManipulator(manipulator.get());
		newGameView->addEventHandler(m_gameEventHandler);
		newGameView->addEventHandler(m_statsHandler);

		m_gameViews.push_back(newGameView);

#ifdef WIN32
		if (m_fullscreen)
			newGameView->apply(new osgViewer::SingleScreen(0));
		else
			newGameView->apply(new osgViewer::SingleWindow(100, 100, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));
#else
		if (m_fullscreen)
			newGameView->setUpViewOnSingleScreen(0);
		else
			newGameView->setUpViewInWindow(100, 100, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
#endif
	}


	return true;
}

bool TroenGame::initializeViewer()
{
	for (auto bikeController : m_bikeControllers)
	{
		if (bikeController->hasGameView()) {
			osg::ref_ptr<SampleOSGViewer> viewer = new SampleOSGViewer();
			viewer.get()->addView(bikeController->getGameView());

#ifdef WIN32
			// turn of vSync (we implement an adaptive gameLoop that syncs itself)
			osg::ref_ptr<RealizeOperation> operation = new RealizeOperation;
			viewer->setRealizeOperation(operation);
			viewer->realize();
#endif
			m_viewers.push_back(viewer);
		}
	}


	return true;
}

bool TroenGame::composeSceneGraph()
{
	/* Scene graph
	m_hudSwitch - HUDController
	/
	GameView#Camera - m_rootNode#Group
	\
	m_postprocessing#Cameras - each camera child node to m_rootNode
	- SELECT_GLOW_OBJECTS
	\
	m_sceneNode#Group - Bike&LevelView
	- HBLUR
	- VBLUR
	- PostProcesingCamera
	\
	Quad#Geode
	*/

	if (m_usePostProcessing)
	{
		// viewport of all windows has to be equal since only the first is taken for PostProcessing Texture Sizes
		osg::Viewport * viewport = m_gameViews[0]->getCamera()->getViewport();

		m_postProcessing = std::make_shared<PostProcessing>(m_rootNode, viewport->width(), viewport->height());

		m_sceneNode = m_postProcessing->getSceneNode();

		//explicit call, to enable glow from start
		resize(viewport->width(), viewport->height());
	}
	else
		m_sceneNode = m_rootNode;

	for (auto playerNode : m_playerNodes) {
		playerNode->addChild(m_rootNode);
	}

	m_skyDome->getOrCreateStateSet()->setRenderBinDetails(-1, "RenderBin");
	m_sceneNode->addChild(m_skyDome.get());

	m_sceneNode->addChild(m_levelController->getViewNode());
	m_sceneNode->addChild(m_sunLightSource.get());

	for (auto bikeController : m_bikeControllers)
	{
		m_sceneNode->addChild(bikeController->getViewNode());
	}

	m_sceneNode->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::ON);

	if (m_useReflection)
	{
		//sceneNode has to be added to reflection after adding all (non hud) objects
		for (int playerID = 0; playerID < m_playerNodes.size(); playerID++)
		{
			m_reflections[playerID]->addSceneNode(m_sceneNode);
			m_playerNodes[playerID]->addChild(m_reflections[playerID]->getReflectionCameraGroup());
		}
	}

	int currentIndex = -1;
	for (auto playerNode : m_playerNodes) {
		currentIndex++;
		playerNode->addChild(m_HUDControllers[currentIndex]->getViewNode());
	}


	if (m_usePostProcessing)
		m_rootNode->addChild(m_sceneNode);

	osg::ref_ptr<osg::Group> radarScene = new osg::Group;

	for (auto bikeController : m_bikeControllers)
		radarScene->addChild(bikeController->getViewNode());
	radarScene->addChild(m_levelController->getViewNode());

	for (auto hudController : m_HUDControllers) {
		hudController->attachSceneToRadarCamera(radarScene);
	}

	osgUtil::Optimizer optimizer;
	optimizer.optimize(m_rootNode, optimizer.REMOVE_REDUNDANT_NODES |
		optimizer.TRISTRIP_GEOMETRY | optimizer.OPTIMIZE_TEXTURE_SETTINGS | 
		optimizer.VERTEX_POSTTRANSFORM | optimizer.INDEX_MESH);

	return true;
}

bool TroenGame::initializeInput()
{
	int currentIndex = -1;
	for (auto bikeController : m_bikeControllers)
	{
		currentIndex++;
		// attach all keyboard handlers to all gameViews
		if (bikeController->hasKeyboardHandler())
		{
			for (auto gameView : m_gameViews)
			{
				gameView->addEventHandler(bikeController->getKeyboardHandler());
			}
		}
	}
	return true;
}

bool TroenGame::initializePhysicsWorld()
{
	m_physicsWorld = std::make_shared<PhysicsWorld>(m_gameLogic, m_useDebugView);
	m_physicsWorld->addRigidBodies(m_levelController->getRigidBodies(), COLGROUP_LEVEL, COLMASK_LEVEL);

	// attach world
	for (auto bikeController : m_bikeControllers)
		bikeController->attachWorld(m_physicsWorld);
	m_levelController->attachWorld(m_physicsWorld);

	m_gameLogic->attachPhysicsWorld(m_physicsWorld);
	return true;
}

void TroenGame::startGameLoop()
{
	// adaptive game loop from here:
	// http://entropyinteractive.com/2011/02/game-engine-design-the-game-loop/

	// INITIALIZATION
	initialize();

	if (m_useDebugView)
		m_sceneNode->addChild(m_physicsWorld->m_debug->getSceneGraph());

	if (m_fullscreen)
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
	while (!m_viewers[0]->done())
	{
		g_gameLoopTime = m_gameloopTimer->elapsed();
		g_gameTime = m_gameTimer->elapsed();

		// are we significantly behind? if yes, "resync", force rendering
		if ((g_gameLoopTime - nextTime) > maxMillisecondsBetweenFrames)
			nextTime = g_gameLoopTime;
		// is it time to render the next frame?
		if (m_testPerformance || g_gameLoopTime >= nextTime)
		{
			// assign the time for the next update
			nextTime += minMillisecondsBetweenFrames;

			// LOOP REALLY STARTS HERE:
			m_gameLogic->step(g_gameLoopTime, g_gameTime);
			if (!m_gameTimer->paused())
			{
				for (auto bikeController : m_bikeControllers)
				{
					bikeController->updateModel(g_gameTime);
				}
				m_physicsWorld->stepSimulation(g_gameTime);
				m_levelController->update();
			}

			m_audioManager->Update(g_gameLoopTime / 1000);
			m_audioManager->setMotorSpeed(m_bikeControllers[0]->getSpeed());

			if (m_postProcessing) m_postProcessing->setBeat(m_audioManager->getTimeSinceLastBeat());

			// do we have extra time (to draw the frame) or did we skip too many frames already?
			if (g_gameLoopTime < nextTime || (skippedFrames > maxSkippedFrames))
			{
				for (int i = 0; i < m_viewers.size(); i++)
					m_HUDControllers[i]->update(g_gameLoopTime, g_gameTime, m_timeLimit, m_gameLogic->getGameState(), m_bikeControllers);

				for (auto viewer : m_viewers)
					viewer->frame();
				// TODO: find a way to eleminate this workaround
				// doesn't work if it's executed earlier
				if (!nearPlaneAdapted)
					for (auto gameView : m_gameViews)
						fixCulling(gameView);
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
				if (!m_testPerformance) m_gameThread->msleep(sleepTime);
		}
	}

	if (m_fullscreen)
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

	// physics & gamelogic
	m_physicsWorld.reset();
	m_gameLogic.reset();

	//viewer & views
	m_viewers.clear();
	for (auto gameView : m_gameViews)
		gameView = nullptr;
	m_gameViews.clear();
	m_statsHandler = nullptr;

	// models & scenegraph
	m_rootNode = nullptr;

	m_levelController.reset();
	m_bikeControllers.clear();

	// TODO: is this still necessary if we clear the vector?
	for (auto hudController : m_HUDControllers)
		hudController.reset();
	m_HUDControllers.clear();

	m_playerNodes.clear();

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
