#include "troengame.h"
// STD
#include <iostream>
// OSG
#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/LineWidth>

// troen
#include "sampleosgviewer.h"
#include "updatebikepositioncallback.h"

#include "input/bikeinputstate.h"
#include "input/keyboard.h"
#include "input/gamepad.h"

#include "util/chronotimer.h"
#include "util/gldebugdrawer.h"

#include "model/physicsworld.h"

#include "controller/bikecontroller.h"
#include "controller/levelcontroller.h"

using namespace troen;

// TODO: pass as parameter to troengame
#define USE_GAMEPAD true



//////////// TODO: move MotionBlurOperation into own class if it's working with nVidia and we are optimistic to get it working for ATI

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <iostream>

class MotionBlurOperation : public osg::Operation
{
public:
	MotionBlurOperation(double persistence) :
		osg::Operation("MotionBlur", true),
		cleared_(false),
		persistence_(persistence)
	{
		}

	virtual void operator () (osg::Object* object)
	{
		osg::GraphicsContext* gc = dynamic_cast<osg::GraphicsContext*>(object);
		if (!gc) return;

		double t = gc->getState()->getFrameStamp()->getSimulationTime();

		if (!cleared_)
		{
			// clear the accumulation buffer
			glClearColor(0, 0, 0, 0);
			glClear(GL_ACCUM_BUFFER_BIT);
			cleared_ = true;
			t0_ = t;
		}

		double dt = fabs(t - t0_);
		t0_ = t;

		// compute the blur factor
		double s = powf(0.2, dt / persistence_);

		// scale, accumulate and return
		glAccum(GL_MULT, s);
		glAccum(GL_ACCUM, 1 - s);
		glAccum(GL_RETURN, 1.0f);
	}

private:
	bool cleared_;
	double t0_;
	double persistence_;
};

////////////

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
	// initialize sound  here

	std::cout << "[TroenGame::initialize] initializing game ..." << std::endl;

	std::cout << "[TroenGame::initialize] models and scenegraph ..." << std::endl;
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

bool TroenGame::initializeControllers()
{
	m_levelController = std::make_shared<LevelController>();
	m_bikeController = std::make_shared<BikeController>();
	return true;
}

bool TroenGame::composeSceneGraph()
{
	m_rootNode->addChild(m_levelController->getViewNode());
	m_rootNode->addChild(m_bikeController->getViewNode());
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
	m_gameView->setSceneData(m_rootNode);
	m_gameView->setUpViewInWindow(100, 100, 1280, 720, 0);
	//m_gameView->setUpViewOnSingleScreen(0);

	// TODO
	// (possibly multiple ones for multiple rendering passes)
	return true;
}

bool TroenGame::initializeViewer()
{
	m_sampleOSGViewer = new SampleOSGViewer();
	m_sampleOSGViewer->addView(m_gameView);

	double persistence = 0.25;
	osgViewer::Viewer::Windows windows;
	m_sampleOSGViewer->getWindows(windows);
	for (osgViewer::Viewer::Windows::iterator itr = windows.begin();
		itr != windows.end();
		++itr)
	{
		(*itr)->add(new MotionBlurOperation(persistence));
	}

	
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
			runAI()
			// (network / multiplayer)
			updateModels() and checkForUserInput()
			stepSimulation() (Physics) + updateViews()
			//render();*/

			m_bikeController->updateModel();

			m_physicsWorld->stepSimulation(currTime);

			// do we have extra time (to draw the frame) or did we skip too many frames already?
			if (currTime < nextTime || (skippedFrames > maxSkippedFrames))
			{
				//std::cout << "drawing" << std::endl;
				emit newFrame(currTime);
				m_sampleOSGViewer->frame();
#ifdef DEBUG				
				if (notAdded) {
					m_rootNode->addChild(m_physicsWorld->m_debug->getSceneGraph());
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
	// clean up in reverse order from initialization

	// physics
	m_physicsWorld.reset();
	//timer
	m_timer.reset();
	//input

	//viewer & views
	m_sampleOSGViewer = NULL;
	m_gameView = NULL;

	// models & scenegraph
	m_rootNode = NULL;
	m_bikeController.reset();
	m_levelController.reset();

	std::cout << "[TroenGame::shutdown] shutdown complete " << std::endl;
	return true;
}
