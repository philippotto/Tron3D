#include "troengame.h"

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/MatrixTransform>

#include <chrono>

#include "sampleosgviewer.h"
#include "input/bikeinputstate.h"
#include "input/keyboardeventhandler.h"

#include "physics/physicsworld.h"
#include "physics/bike.h"
#include "updatebikepositioncallback.h"
#include "util/chronotimer.h"

using namespace troen;
using namespace std;

TroenGame::TroenGame(QThread* thread /*= NULL*/) :
	m_gameThread(thread)
{
	if (m_gameThread == NULL) {
		m_gameThread = new QThread(this);
	}
	moveToThread(m_gameThread);
	m_gameThread->start(QThread::HighestPriority);

	//initialize();
}

TroenGame::~TroenGame()
{
}

bool TroenGame::initialize()
{
	m_rootNode = new osg::Group;

	// careful about the order of initialization
	// TODO
	// initialize sound, level and ... here

	std::cout << "initializing game ..." << std::endl;

	std::cout << "models and scenegraph ..." << std::endl;
	initializeModels();
	initializeLevel();
	composeSceneGraph();

	std::cout << "views & viewer ..." << std::endl;
	initializeViews();
	initializeViewer();

	std::cout << "input ..." << std::endl;
	initializeInput();

	std::cout << "timer ..." << std::endl;
	initializeTimer();
	
	std::cout << "physics ..." << std::endl;
	initializePhysics();

	std::cout << "successfully initialized !" << std::endl;

	return true;
}

bool TroenGame::initializeModels()
{
	m_childNode = new osg::PositionAttitudeTransform();
	
	
	m_childNode->addChild(osgDB::readNodeFile("data/models/cessna.osgt"));
	// m_childNode->setPosition(osg::Vec3(0, 0, 500));

	return true;
}


bool TroenGame::initializeLevel() {


	osg::ref_ptr<osg::Group> levelGroup = new osg::Group();

	osg::ref_ptr<osg::Geode> levelGeode = new osg::Geode();

	// ground
	osg::ref_ptr<osg::Box> ground
		= new osg::Box(osg::Vec3(0, 0, 0), 1000, 1000, 1);
	
	osg::ref_ptr<osg::ShapeDrawable> groundDrawable
		= new osg::ShapeDrawable(ground);


	// wall right
	osg::ref_ptr<osg::Box> wallRight
		= new osg::Box(osg::Vec3(500, 0, 50), 1, 1000, 100);
	
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableRight
		= new osg::ShapeDrawable(wallRight);

	// wall left
	osg::ref_ptr<osg::Box> wallLeft
		= new osg::Box(osg::Vec3(-500, 0, 50), 1, 1000, 100);
	
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableLeft
		= new osg::ShapeDrawable(wallLeft);
	
	
	// wall back
	osg::ref_ptr<osg::Box> wallBack
		= new osg::Box(osg::Vec3(0, -500, 50), 1000, 1, 100);
	
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableBack
		= new osg::ShapeDrawable(wallBack);

	// wall front
	osg::ref_ptr<osg::Box> wallFront
		= new osg::Box(osg::Vec3(0, 500, 50), 1000, 1, 100);
	
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableFront
		= new osg::ShapeDrawable(wallFront);


	levelGeode->addDrawable(groundDrawable);
	levelGeode->addDrawable(wallDrawableLeft);
	levelGeode->addDrawable(wallDrawableRight);
	levelGeode->addDrawable(wallDrawableFront);
	levelGeode->addDrawable(wallDrawableBack);
	levelGroup->addChild(levelGeode);


	// fence
	// osg::Box *fence = new osg::Box(osg::Vec3(0, 0, 0), 10, 10, 0.1);
	// osg::ShapeDrawable* fenceDrawable = new osg::ShapeDrawable(fence);
	// levelGroup->addDrawable(fenceDrawable);
	
	// Add the goede to the scene:
	m_rootNode->addChild(levelGroup);

	return true;
}

bool TroenGame::composeSceneGraph()
{
	m_rootNode->addChild(m_childNode);
	return true;
}

bool TroenGame::initializeInput()
{
	// TODO
	// dw: clean this up, move it to the appropriate place
	osg::ref_ptr<input::BikeInputState> bikeInputState = new input::BikeInputState();
	osg::ref_ptr<physics::Bike> bike = new physics::Bike(bikeInputState);

	m_childNode->setUpdateCallback(new UpdateBikePositionCallback(bike));

	osg::ref_ptr<input::KeyboardEventHandler> keyboardHandler = new input::KeyboardEventHandler(bikeInputState);
	m_gameView->addEventHandler(keyboardHandler);
	return true;
}

bool TroenGame::initializeViews()
{
	m_gameView = new osgViewer::View;
	m_gameView->setCameraManipulator(new osgGA::TrackballManipulator);
	m_gameView->setSceneData(m_rootNode);
	m_gameView->setUpViewInWindow(100, 100, 800, 640, 0);
	// TODO
	// add camera to gameview
	// (possibly multiple ones for multiple rendering passes)
	return true;
}

bool TroenGame::initializeViewer()
{
	m_sampleOSGViewer = new SampleOSGViewer();
	m_sampleOSGViewer->addView(m_gameView);
	// TODO
	// add event handlers to the viewer here
	return true;
}

bool TroenGame::initializeTimer()
{
	m_timer = make_shared<util::ChronoTimer>(false, true);
	return true;
}


bool TroenGame::initializePhysics()
{
	physics::PhysicsWorld *world;
	world = new physics::PhysicsWorld();
	world->initialize();
	delete world;

	return true;
}

void TroenGame::startGameLoop()
{
	// simplest version, use this if the below gameloop does not work
	// or you still have timer issues
	//while (!m_sampleOSGViewer->done())
	//{
	//	m_sampleOSGViewer->frame();
	//}

	// game loop from here
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

			// do we have extra time (to draw the frame) or have we rendered a frame recently?
			if (currTime < nextTime || (skippedFrames > maxSkippedFrames))
			{
				//std::cout << "drawing" << std::endl;
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
	m_timer.reset();
	m_sampleOSGViewer = NULL;
	m_gameView = NULL;

	m_childNode = NULL;
	m_rootNode = NULL;

	return true;
}