#include "troengame.h"
// OSG
#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/LineWidth>
#include <osgViewer/ViewerEventHandlers>
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
#include "view/motionblur.h"
#include "view/nodefollowcameramanipulator.h"


using namespace troen;

TroenGame::TroenGame(QThread* thread /*= nullptr*/) :
m_gameThread(thread),
m_simulationPaused(false),
m_numberOfBikes(0),
m_splitscreen(false),
m_fullscreen(false),
m_usePostProcessing(false)
{
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

void TroenGame::setFovy(float newFovy)
{
	double fovy, aspect, znear, zfar;
	m_gameView->getCamera()->getProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
	std::cout << "old: " << fovy;
	m_gameView->getCamera()->setProjectionMatrixAsPerspective(newFovy, aspect, znear, zfar);
	std::cout << "   new: " << newFovy << std::endl;
}

float TroenGame::getFovy()
{
	double fovy, aspect, znear, zfar;
	m_gameView->getCamera()->getProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
	return fovy;
}


void TroenGame::refreshTextures(int width, int height){
	if (m_postProcessing){
		m_postProcessing->setupTextures(width, height);
	}
}


void TroenGame::prepareAndStartGame(GameConfig config)
{
	m_numberOfBikes = config.numberOfBikes;
	m_splitscreen = config.splitscreen;
	m_fullscreen = config.fullscreen;
	m_usePostProcessing = config.usePostProcessing;
	m_useDebugView = config.useDebugView;

	m_playerInputTypes.clear();
	for (int i = 0; i < m_numberOfBikes; i++)
		m_playerInputTypes.push_back(config.playerInputTypes[i]);

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
			input::BikeInputState::InputDevice)m_playerInputTypes[i],m_levelController->initialPositionTransformForBikeWithIndex(i)));
	}
	//m_HUDController = std::make_shared<HUDController>();
	return true;
}

bool TroenGame::initializeGameLogic()
{
	m_gameLogic = std::make_shared<GameLogic>(this, m_audioManager, m_levelController, m_bikeControllers);
	return true;
}

bool TroenGame::initializeViews()
{
	m_gameView = new osgViewer::View;

	osg::ref_ptr<NodeFollowCameraManipulator> manipulator
		= new NodeFollowCameraManipulator();
	m_bikeControllers[0]->attachTrackingCamera(manipulator);
	m_gameView->setCameraManipulator(manipulator.get());

	m_statsHandler = new osgViewer::StatsHandler;
	m_statsHandler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_T);
	m_statsHandler->setKeyEventPrintsOutStats(osgGA::GUIEventAdapter::KEY_P);
	m_statsHandler->setKeyEventToggleVSync(osgGA::GUIEventAdapter::KEY_V);
	m_gameView->addEventHandler(m_statsHandler);

	m_gameView->setSceneData(m_rootNode);
#ifdef WIN32
	if (m_fullscreen)
		m_gameView->apply(new osgViewer::SingleScreen(0));
	else
		m_gameView->apply(new osgViewer::SingleWindow(100, 100, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));
#else
    if (m_fullscreen)
		m_gameView->setUpViewOnSingleScreen(0);
	else
		m_gameView->setUpViewInWindow(100, 100, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
#endif

	m_gameEventHandler = new GameEventHandler(this,m_gameLogic);
	m_gameView->addEventHandler(m_gameEventHandler);

	m_bikeControllers[0]->attachGameView(m_gameView);

	if (m_splitscreen)
	{
		m_gameView2 = new osgViewer::View;

		osg::ref_ptr<NodeFollowCameraManipulator> manipulator2
			= new NodeFollowCameraManipulator();
		m_bikeControllers[1]->attachTrackingCamera(manipulator2);
		m_gameView2->setCameraManipulator(manipulator2.get());

		m_gameView2->setSceneData(m_rootNode);
		m_gameView2->setUpViewInWindow(500, 500, 640, 480);

		m_bikeControllers[1]->attachGameView(m_gameView2);
	}

	return true;
}

bool TroenGame::initializeViewer()
{
	double persistence = 0.06;
	osgViewer::Viewer::Windows windows;

	m_sampleOSGViewer = new SampleOSGViewer();
	m_sampleOSGViewer.get()->addView(m_gameView);

	m_sampleOSGViewer->getWindows(windows);
	windows.at(0)->add(new MotionBlurOperation(persistence));
    m_sampleOSGViewer->setRunFrameScheme( osgViewer::Viewer::ON_DEMAND );

	if (m_splitscreen)
	{
		m_sampleOSGViewer2 = new SampleOSGViewer();
		m_sampleOSGViewer2.get()->addView(m_gameView2);

		m_sampleOSGViewer2->getWindows(windows);
		windows.at(0)->add(new MotionBlurOperation(persistence));
	}

	return true;
}

bool TroenGame::composeSceneGraph()
{
	if (m_usePostProcessing)
	{
		osg::Viewport * viewport = m_gameView->getCamera()->getViewport();
		m_postProcessing = std::make_shared<PostProcessing>(m_rootNode, viewport->width(), viewport->height());
		m_sceneNode = m_postProcessing->getSceneNode();
	}
	else
		m_sceneNode = m_rootNode;

	m_sceneNode->addChild(m_skyDome.get());
	m_sceneNode->addChild(m_levelController->getViewNode());
	//m_sceneNode->addChild(m_HUDController->getViewNode());

	for (auto bikeController : m_bikeControllers)
		m_sceneNode->addChild(bikeController->getViewNode());

	if (m_usePostProcessing)
		m_rootNode->addChild(m_sceneNode);

	return true;
}

bool TroenGame::initializeInput()
{
	for (auto bikeController : m_bikeControllers)
	{
		// attach keyboard handler to the gameView if existent
		if (bikeController->hasEventHandler())
		{
			m_gameView->addEventHandler(bikeController->getEventHandler());
		}
	}
	return true;
}

bool TroenGame::initializePhysicsWorld()
{
	m_physicsWorld = std::make_shared<PhysicsWorld>(m_gameLogic, m_useDebugView);
	m_physicsWorld->addRigidBodies(m_levelController->getRigidBodies(),COLGROUP_LEVEL,COLMASK_LEVEL);

	for (auto bikeController : m_bikeControllers)
	{
		bikeController->attachWorld(m_physicsWorld);
	}
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
	m_audioManager->SetMasterVolume(0.f);

	if (m_useDebugView)
		m_rootNode->addChild(m_physicsWorld->m_debug->getSceneGraph());

	// GAME LOOP VARIABLES
	long double nextTime = m_timer->elapsed();
	const long double minMillisecondsBetweenFrames = 10;
	const long double maxMillisecondsBetweenFrames = 50;
	int skippedFrames = 0;
	int maxSkippedFrames = 4;

	bool nearPlaneAdapted = false;
    
    bool testPerformance = true;

	// GAME LOOP
	while (!m_sampleOSGViewer->done())
	{
		long double currTime = m_timer->elapsed();
		// are we significantly behind? if yes, "resync", force rendering
		if ((currTime - nextTime) > maxMillisecondsBetweenFrames)
			nextTime = currTime;
		// is it time to render the next frame?
		if (testPerformance || currTime >= nextTime)
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

			// do we have extra time (to draw the frame) or did we skip too many frames already?
			if (currTime < nextTime || (skippedFrames > maxSkippedFrames))
			{
				m_sampleOSGViewer->frame();
				
				if (!nearPlaneAdapted) {
					// doesn't work if it's executed earlier
					// TODO should be done for m_gameView2 and other possible views
					double fovy, aspect, znear, zfar;
					m_gameView->getCamera()->getProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
					m_gameView->getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
					znear = 1.0;
					m_gameView->getCamera()->setProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
				}

				if(m_splitscreen) m_sampleOSGViewer2->frame();
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
				if (!testPerformance) m_gameThread->msleep(sleepTime);
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
	m_sampleOSGViewer = nullptr;
	m_sampleOSGViewer2 = nullptr;
	m_gameView = nullptr;
	m_gameView2 = nullptr;
	m_statsHandler = nullptr;

	// models & scenegraph
	m_rootNode = nullptr;

	m_levelController.reset();
	m_bikeControllers.clear();
	m_HUDController.reset();

	// sound
	m_audioManager->StopSFXs();
	m_audioManager->StopSongs();
	m_audioManager.reset();

	// shaders
	shaders::m_allShaderPrograms.clear();

	std::cout << "[TroenGame::shutdown] shutdown complete " << std::endl;
	return true;
}
