#include "troengame.h"
// OSG
#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/LineWidth>

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

#include "globals.h"


using namespace troen;
extern long double g_currentTime;

TroenGame::TroenGame(QThread* thread /*= nullptr*/) :
m_gameThread(thread),
m_simulationPaused(true),
m_numberOfBikes(0),
m_splitscreen(false),
m_fullscreen(false),
m_usePostProcessing(false),
m_testPerformance(false)
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
	m_simulationPaused = !m_simulationPaused;
}

void TroenGame::pauseSimulation()
{
	m_simulationPaused = true;
}

void TroenGame::unpauseSimulation()
{
	m_simulationPaused = false;
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


void TroenGame::prepareAndStartGame(GameConfig config)
{
	m_numberOfBikes = config.numberOfBikes;
	m_splitscreen = config.splitscreen;
	m_fullscreen = config.fullscreen;
	m_usePostProcessing = config.usePostProcessing;
	m_useDebugView = config.useDebugView;
	m_testPerformance = config.testPerformance;

	m_playerInputTypes.clear();
	for (int i = 0; i < m_numberOfBikes; i++)
		m_playerInputTypes.push_back(config.playerInputTypes[i]);

	m_playerColors.clear();
	for (int j = 0; j < m_numberOfBikes; j++)
	{
		osg::Vec3 col = osg::Vec3(config.playerColors[j].red(), config.playerColors[j].green(), config.playerColors[j].blue());
		m_playerColors.push_back(col);
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
	initializeTimer();

	std::cout << "[TroenGame::initialize] shaders ..." << std::endl;
	initializeShaders();

	std::cout << "[TroenGame::initialize] sound ..." << std::endl;
	initializeSound();

	std::cout << "[TroenGame::initialize] controllers (models & views) ..." << std::endl;
	initializeSkyDome();
	initializeControllers();
	initializeLighting();

	std::cout << "[TroenGame::initialize] gameLogic ..." << std::endl;
	initializeGameLogic();

	std::cout << "[TroenGame::initialize] views & viewer ..." << std::endl;
	initializeViews();
	initializeViewer();

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

bool TroenGame::initializeTimer()
{
	m_timer = std::make_shared<util::ChronoTimer>(false, true);
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
		m_bikeControllers.push_back(std::make_shared<BikeController>((
			input::BikeInputState::InputDevice)m_playerInputTypes[i],
			m_levelController->getSpawnPointForBikeWithIndex(i),
			m_playerColors[i],
			&m_resourcePool)
		);
	}

	for (int i = 0; i < m_bikeControllers.size(); i++) {
		m_HUDControllers.push_back(std::make_shared<HUDController>(m_bikeControllers[i], osg::Vec4(m_playerColors[i], 1)));
		if (!m_splitscreen) break;
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
	m_gameLogic = std::make_shared<GameLogic>(this, m_audioManager, m_levelController, m_bikeControllers);
	return true;
}

bool TroenGame::initializeViews()
{

	m_statsHandler = new osgViewer::StatsHandler;
	m_statsHandler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_T);
	m_statsHandler->setKeyEventPrintsOutStats(osgGA::GUIEventAdapter::KEY_P);
	m_statsHandler->setKeyEventToggleVSync(osgGA::GUIEventAdapter::KEY_V);

	m_gameEventHandler = new GameEventHandler(this, m_gameLogic);


		int currentIndex = -1;
		for (auto bikeController : m_bikeControllers) {
			currentIndex++;

			// TODO: is there a better place for this?
			m_playerNodes.push_back(new osg::Group());

			osg::ref_ptr<osgViewer::View> newGameView = new osgViewer::View();
			newGameView->getCamera()->setCullMask(CAMERA_MASK_MAIN);
			newGameView->setSceneData(m_playerNodes[currentIndex]);

			osg::ref_ptr<NodeFollowCameraManipulator> manipulator
				= new NodeFollowCameraManipulator();

			newGameView->setCameraManipulator(manipulator.get());

			bikeController->attachTrackingCameras(manipulator, m_HUDControllers[currentIndex]);
			bikeController->attachGameView(newGameView);

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

			if (!m_splitscreen)
				break;
		}


	return true;
}

bool TroenGame::initializeViewer()
{
	int currentIndex = -1;
	for (auto bikeController : m_bikeControllers)
	{
		currentIndex++;

		osg::ref_ptr<SampleOSGViewer> viewer = new SampleOSGViewer();
		// TODO: splitscreen gameviews...
		viewer.get()->addView(m_gameViews[currentIndex]);

#ifdef WIN32
		// turn of vSync (we implement an adaptive gameLoop that syncs itself)
		osg::ref_ptr<RealizeOperation> operation = new RealizeOperation;
		viewer->setRealizeOperation(operation);
		// viewer->realize();
#endif
		m_viewers.push_back(viewer);
		if (!m_splitscreen)	{
			return true;
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


	// TODO (Philipp): uniforms
	/*m_test1 = new osg::Uniform("test", 0.1);
	m_firstPlayerGroup->getOrCreateStateSet()->addUniform(m_test1);*/



	m_skyDome->getOrCreateStateSet()->setRenderBinDetails(-1, "RenderBin");
	m_sceneNode->addChild(m_skyDome.get());

	m_sceneNode->addChild(m_levelController->getViewNode());
	m_sceneNode->addChild(m_sunLightSource.get());

	for (auto bikeController : m_bikeControllers)
	{
		m_sceneNode->addChild(bikeController->getViewNode());
	}

	m_sceneNode->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::ON);

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
	m_physicsWorld->addRigidBodies(m_levelController->getRigidBodies(),COLGROUP_LEVEL,COLMASK_LEVEL);

	// attach world
	for (auto bikeController : m_bikeControllers)
	{
		bikeController->attachWorld(m_physicsWorld);
	}
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

	m_timer->start();

	m_audioManager->PlaySong("data/sound/theGameHasChanged.mp3");
	m_audioManager->PlayEngineSound();

	m_audioManager->SetMasterVolume(0.f);

	if (m_useDebugView)
		m_sceneNode->addChild(m_physicsWorld->m_debug->getSceneGraph());

    btVector3 itemBoxVector(500, 255, +0.5);
	m_levelController->addItemBox(itemBoxVector);

	// GAME LOOP VARIABLES
	long double nextTime = m_timer->elapsed();
	const double minMillisecondsBetweenFrames = 16.7; // vSync to 60 fps
	const double maxMillisecondsBetweenFrames = 4* minMillisecondsBetweenFrames + 1;
	int skippedFrames = 0;
	const int maxSkippedFrames = 4;

	bool nearPlaneAdapted = false;

	// GAME LOOP
	// terminates when first viewer is closed
	while (!m_viewers[0]->done())
	{
		long double currTime = m_timer->elapsed();
		g_currentTime = currTime;

		// are we significantly behind? if yes, "resync", force rendering
		if ((currTime - nextTime) > maxMillisecondsBetweenFrames)
			nextTime = currTime;
		// is it time to render the next frame?
		if (m_testPerformance || currTime >= nextTime)
		{
			//std::cout << "difference: " << currTime - nextTime << std::endl;
			// assign the time for the next update
			nextTime += minMillisecondsBetweenFrames;

			// LOOP REALLY STARTS HERE:
			/* FROM GameProg Info session:
			runAI()
			// (network / multiplayer)
			updateModels() and checkForUserInput()
			stepSimulation() (Physics) + updateViews()
			//render();*/
			if (!m_simulationPaused)
			{
				for (auto bikeController : m_bikeControllers)
				{
					bikeController->updateModel(currTime);
				}
				m_physicsWorld->stepSimulation(currTime);
			}

			m_audioManager->Update(currTime/1000);
			m_audioManager->setMotorSpeed(m_bikeControllers[0]->getSpeed());

			if (m_postProcessing)
				m_postProcessing->setBeat(m_audioManager->getTimeSinceLastBeat());

			// do we have extra time (to draw the frame) or did we skip too many frames already?
			if (currTime < nextTime || (skippedFrames > maxSkippedFrames))
			{
				for (int i = 0; i < m_viewers.size(); i++)
					m_HUDControllers[i]->update();

				for (auto viewer : m_viewers) {
					viewer->frame();
				}

				if (!nearPlaneAdapted) {
					// TODO: find a way to eleminate this workaround
					// doesn't work if it's executed earlier
					for (auto gameView : m_gameViews) {
						fixCulling(gameView);
					}
				}

				skippedFrames = 0;
			}
			else
			{
				skippedFrames++;
			}
		}
		else // WAIT
		{
			// calculate the time to sleep
			long double sleepTime = (nextTime - currTime);
			// sanity check
			if (sleepTime > 0)
			{
				// sleep until nextTime
				//std::cout << "sleep for: " << sleepTime << std::endl;
				if (!m_testPerformance) m_gameThread->msleep(sleepTime);
			}
		}
	}

	// SHUTDOWN
	shutdown();
}

bool TroenGame::shutdown()
{
	// clean up in reverse order from initialization

	//timer
	m_timer.reset();
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
};
