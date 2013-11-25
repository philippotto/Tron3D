#include "troengame.h"
// OSG
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/LineWidth>

// troen
#include "sampleosgviewer.h"
#include "gamelogic.h"
#include "updatebikepositioncallback.h"

#include "input/bikeinputstate.h"
#include "input/keyboard.h"
#include "input/gamepad.h"

#include "util/chronotimer.h"

#include "model/bikemodel.h"
#include "controller/levelcontroller.h"
#include "view/bikeview.h"
#include "view/shaders.h"


// TODO remove?
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"

#include "util/gldebugdrawer.h"


using namespace troen;
#define TROENGAME
#define USE_GAMEPAD true

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

bool TroenGame::initialize()
{
	m_rootNode = new osg::Group;

	// careful about the order of initialization
	// TODO
	// initialize sound ... here

	std::cout << "[TroenGame::initialize] initializing game ..." << std::endl;
	initializeShaders();

	std::cout << "[TroenGame::initialize] models and scenegraph ..." << std::endl;
	initializeModels();
	initializeLevel();
	composeSceneGraph();

	std::cout << "[TroenGame::initialize] views & viewer ..." << std::endl;

	initializeViews();
	initializeViewer();

	std::cout << "[TroenGame::initialize] input ..." << std::endl;
	initializeInput();

	std::cout << "[TroenGame::initialize] timer ..." << std::endl;
	initializeTimer();
	
	std::cout << "[TroenGame::initialize] GameLogic ..." << std::endl;
	initializeGameLogic();

	std::cout << "[TroenGame::initialize] successfully initialized !" << std::endl;
	return true;
}

bool TroenGame::initializeModels()
{
	m_childNode = new osg::PositionAttitudeTransform();
	//

#ifdef _DEBUG
	m_childNode->addChild(osgDB::readNodeFile("data/models/cessna.osgt"));
#endif
#ifndef _DEBUG
	BikeView *bikeView = new BikeView();
	m_childNode->addChild( bikeView->get_rootNode() );
	// currently, initial position is set in updateBikePositoinCallback.cpp
#endif
	return true;
}

bool TroenGame::initializeLevel()
{
	m_levelController = std::make_shared<LevelController>();	
	return true;
}

bool TroenGame::composeSceneGraph()
{
	m_rootNode->addChild(m_levelController->getViewNode());
	m_rootNode->addChild(m_childNode);

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
	m_bike = new BikeModel(bikeInputState);

	m_childNode->setUpdateCallback(new UpdateBikePositionCallback(m_bike));

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
	osg::ref_ptr<osgGA::NodeTrackerManipulator> manip
		= new osgGA::NodeTrackerManipulator;
	manip->setTrackNode(m_childNode->getChild(0));
	manip->setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION);

	/*manip->setRotationMode(
		osgGA::NodeTrackerManipulator::RotationMode::TRACKBALL);*/

	osg::Matrixd cameraOffset;
	cameraOffset.makeTranslate(0,-100, -20);

	manip->setHomePosition(m_childNode->getPosition(), m_childNode->getPosition() * cameraOffset, osg::Vec3d(0, -1, 0));
	m_gameView->setCameraManipulator(manip.get());
	
	
	/* weird looking (no depth-test / backface culling?) but cool fovy
	osg::Camera *camera = new osg::Camera;
	camera->setProjectionMatrixAsPerspective(90.0, 1.0, 0.5, 1000);	
	m_gameView->setCamera(camera);*/

	
	
	//m_gameView->setCameraManipulator(new osgGA::TrackballManipulator);
	m_gameView->setSceneData(m_rootNode);
	m_gameView->setUpViewInWindow(100, 100, 1280, 720, 0);
	//m_gameView->setUpViewOnSingleScreen(0);
	// TODO
	// add camera to gameview
	// (possibly multiple ones for multiple rendering passes)
	return true;
}

bool TroenGame::initializeViewer()
{
	m_sampleOSGViewer = new SampleOSGViewer();
	m_sampleOSGViewer->addView(m_gameView);
	return true;
}

bool TroenGame::initializeTimer()
{
	m_timer = std::make_shared<util::ChronoTimer>(false, true);
	return true;
}

bool TroenGame::initializeGameLogic()
{
	m_gameLogic = std::make_shared<GameLogic>();
	m_gameLogic->initialize();
	m_gameLogic->addRigidBodies(m_levelController->getRigidBodies());

	// TODO: should we encapsulate all models in m_levelController ?
	m_gameLogic->addRigidBodies(m_bike->getRigidBodies());

	return true;
}

void TroenGame::startGameLoop()
{
	// game loop from here:
	// http://entropyinteractive.com/2011/02/game-engine-design-the-game-loop/

	// INITIALIZATION
	initialize();
	m_timer->start();

	// GAME LOOP VARIABLES
	long double nextTime = m_timer->elapsed();
	const long double minMillisecondsBetweenFrames = 10;
	const long double maxMillisecondsBetweenFrames = 50;
	int skippedFrames = 0;
	int maxSkippedFrames = 4;

	bool notAdded = true;

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
			checkForUserInput()
			runAI()
			moveEnemies()
			// (network / multiplayer)
			resolveCollisisons() (Physics)
			// (moveEnemies)
			/**/
			//update();

			m_gameLogic->stepSimulation();

			// do we have extra time (to draw the frame) or did we skip too many frames already?
			if (currTime < nextTime || (skippedFrames > maxSkippedFrames))
			{
				//std::cout << "drawing" << std::endl;
				m_sampleOSGViewer->frame();
#if defined DEBUG				
				if (notAdded) {
					m_rootNode->addChild(m_gameLogic->m_debug->getSceneGraph());
					notAdded = false;
				}
#endif				
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
	m_timer.reset();
	m_gameLogic.reset();
	m_sampleOSGViewer = NULL;
	m_gameView = NULL;
	m_childNode = NULL;
	m_rootNode = NULL;

	std::cout << "[TroenGame::shutdown] shutdown complete " << std::endl;
	return true;
}