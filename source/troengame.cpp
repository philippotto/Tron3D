#include "troengame.h"
// OSG
#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/LineWidth>
#include <osgViewer/ViewerEventHandlers>
// troen
#include "sampleosgviewer.h"
#include "updatebikepositioncallback.h"
#include "gameeventhandler.h"

#include "input/bikeinputstate.h"
#include "input/keyboard.h"
#include "input/gamepad.h"

#include "util/chronotimer.h"
#include "util/gldebugdrawer.h"

#include "sound/audiomanager.h"

#include "model/physicsworld.h"
#include "model/bikemodel.h"
#include "controller/levelcontroller.h"
#include "controller/bikecontroller.h"
#include "controller/levelcontroller.h"
#include "controller/hudcontroller.h"
#include "view/bikeview.h"
#include "view/shaders.h"
#include "view/skydome.h"

using namespace troen;

// TODO: pass as parameter to troengame
#define USE_GAMEPAD true
#define SOUND_VOLUME 1.f

TroenGame::TroenGame(QThread* thread /*= NULL*/) :
	m_gameThread(thread)
{
	if (m_gameThread == NULL) {
		m_gameThread = new QThread(this);
	}
	moveToThread(m_gameThread);
	m_gameThread->start(QThread::HighestPriority);
}

TroenGame::~TroenGame()
{
}

void TroenGame::switchSoundVolume()
{
	if (m_audioManager->GetMasterVolume() != 0)
		m_audioManager->SetMasterVolume(0);
	else
		m_audioManager->SetMasterVolume(SOUND_VOLUME);
}


bool TroenGame::initialize()
{
	m_rootNode = new osg::Group;

	// careful about the order of initialization
	osg::DisplaySettings::instance()->setNumMultiSamples(4);

	std::cout << "[TroenGame::initialize] initializing game ..." << std::endl;
	initializeShaders();

	std::cout << "[TroenGame::initialize] initializing sound ..." << std::endl;
	initializeSound();

	std::cout << "[TroenGame::initialize] models and scenegraph ..." << std::endl;
	initializeSkyDome();
	initializeControllers();
	composeSceneGraph();

	std::cout << "[TroenGame::initialize] views & viewer ..." << std::endl;

	initializeViews();
	initializeViewer();

	std::cout << "[TroenGame::initialize] input ..." << std::endl;
	initializeInput();

	std::cout << "[TroenGame::initialize] timer ..." << std::endl;
	initializeTimer();
	
	std::cout << "[TroenGame::initialize] physics ..." << std::endl;
	initializePhysicsWorld();

	std::cout << "[TroenGame::initialize] successfully initialized !" << std::endl;
	return true;
}

bool TroenGame::initializeSound()
{
	m_audioManager = std::shared_ptr<sound::AudioManager>(new sound::AudioManager);
	m_audioManager->LoadSFX("data/sound/explosion.wav");
	m_audioManager->LoadSong("data/sound/1.13. Derezzed.flac");
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
	m_bikeController = std::make_shared<BikeController>(m_audioManager);
	//m_HUDController = std::make_shared<HUDController>();
	return true;
}

bool TroenGame::composeSceneGraph()
{
	m_rootNode->addChild(m_skyDome.get());
	m_rootNode->addChild(m_levelController->getViewNode());
	m_rootNode->addChild(m_bikeController->getViewNode());
	//m_rootNode->addChild(m_HUDController->getViewNode());
	
	return true;
}

bool TroenGame::initializeShaders()
{
	shaders::reloadShaders();
	return true;
}

bool TroenGame::initializeInput()
{
	// TODO
	// dw: clean this up, move it to the appropriate place
	osg::ref_ptr<input::BikeInputState> bikeInputState = new input::BikeInputState();
	m_bikeController->setInputState(bikeInputState);

	//m_childNode->setUpdateCallback(new UpdateBikePositionCallback(m_bike));

	osg::ref_ptr<input::Keyboard> keyboardHandler = new input::Keyboard(bikeInputState);
	std::shared_ptr<input::Gamepad> gamepad = std::make_shared<input::Gamepad>(bikeInputState);

	if (USE_GAMEPAD)
	{
		if (gamepad->checkConnection())
		{
			std::cout << "[TroenGame::initializeInput] Gamepad connected on port " << gamepad->getPort() << std::endl;
			bikeInputState->setPollingDevice(gamepad);
		} else
		{
			std::cout << "[TroenGame::initializeInput] USE_GAMEPAD true but no gamepad connected!" << std::endl;
		}
	}

	m_gameView->addEventHandler(keyboardHandler);

	return true;
}

bool TroenGame::initializeViews()
{
	m_gameView = new osgViewer::View;

	osg::ref_ptr<osgGA::NodeTrackerManipulator> manipulator
		= new osgGA::NodeTrackerManipulator;
	manipulator->setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION);
	m_bikeController->attachTrackingCamera(manipulator);
	m_gameView->setCameraManipulator(manipulator.get());

	m_statsHandler = new osgViewer::StatsHandler;
	m_statsHandler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_T);
	m_statsHandler->setKeyEventPrintsOutStats(osgGA::GUIEventAdapter::KEY_P);
	m_statsHandler->setKeyEventToggleVSync(osgGA::GUIEventAdapter::KEY_V);
	m_gameView->addEventHandler(m_statsHandler);

	m_gameView->setSceneData(m_rootNode);
	m_gameView->setUpViewInWindow(100, 100, 1280, 720, 0);
	//m_gameView->setUpViewOnSingleScreen(0);

	m_gameEventHandler = new GameEventHandler(this);
	m_gameView->addEventHandler(m_gameEventHandler);
	return true;
}

bool TroenGame::initializeViewer()
{
	m_sampleOSGViewer = new SampleOSGViewer();
	m_sampleOSGViewer.get()->addView(m_gameView);
	return true;
}

bool TroenGame::initializeTimer()
{
	m_timer = std::make_shared<util::ChronoTimer>(false, true);
	return true;
}

bool TroenGame::initializePhysicsWorld()
{
	m_physicsWorld = std::make_shared<PhysicsWorld>();
	m_physicsWorld->addRigidBodies(m_levelController->getRigidBodies());
	// m_physicsWorld->addRigidBodies(m_bikeController->getRigidBodies());
	m_bikeController->attachWorld(m_physicsWorld);
	return true;
}

void TroenGame::startGameLoop()
{
	// game loop from here:
	// http://entropyinteractive.com/2011/02/game-engine-design-the-game-loop/

	// INITIALIZATION
	initialize();
	m_timer->start();

	m_audioManager->PlaySong("data/sound/1.13. Derezzed.flac");
	m_audioManager->SetMasterVolume(SOUND_VOLUME);
	m_audioManager->SetSongsVolume(SOUND_VOLUME);

	// GAME LOOP VARIABLES
	long double nextTime = m_timer->elapsed();
	const long double minMillisecondsBetweenFrames = 10;
	const long double maxMillisecondsBetweenFrames = 50;
	int skippedFrames = 0;
	int maxSkippedFrames = 4;

// comment out to disable debug mode
//#define DEBUG_DRAW
#ifdef DEBUG_DRAW				
	m_rootNode->addChild(m_physicsWorld->m_debug->getSceneGraph());
#endif	

	// GAME LOOP
	while (!m_sampleOSGViewer->done())
	{
		long double currTime = m_timer->elapsed();
		// are we significantly behind? if yes, "resync", force rendering
		if ((currTime - nextTime) > maxMillisecondsBetweenFrames)
			nextTime = currTime;
		// is it time to render the next frame?
		if (currTime >= nextTime)
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

			m_bikeController->updateModel();

			m_physicsWorld->stepSimulation(currTime);
			m_audioManager->Update(currTime/1000);

			// do we have extra time (to draw the frame) or did we skip too many frames already?
			if (currTime < nextTime || (skippedFrames > maxSkippedFrames))
			{
				m_sampleOSGViewer->frame();			
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
				m_gameThread->msleep(sleepTime);
			}
		}
	}

	// SHUTDOWN
	shutdown();
}

bool TroenGame::shutdown()
{
	// clean up in reverse order from initialization

	// physics
	m_physicsWorld.reset();
	//timer
	m_timer.reset();
	//input

	//viewer & views
	m_sampleOSGViewer = NULL;
	m_gameView = NULL;
	m_statsHandler = NULL;

	// models & scenegraph
	m_rootNode = NULL;
	m_bikeController.reset();
	m_levelController.reset();
	m_HUDController.reset();

	// sound
	m_audioManager->StopSFXs();
	m_audioManager->StopSongs();
	m_audioManager.reset();

	std::cout << "[TroenGame::shutdown] shutdown complete " << std::endl;
	return true;
}
