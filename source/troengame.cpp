#include "troengame.h"

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>

#include <chrono>

#include "sampleosgviewer.h"

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
	// initialize physics, sound, level and ... here

	initializeModels();
	composeSceneGraph();

	initializeViews();
	initializeViewer();

	return true;
}

bool TroenGame::initializeModels()
{
	m_childNode = osgDB::readNodeFile("data/models/cessna.osgt");
	return true;
}

bool TroenGame::composeSceneGraph()
{
	m_rootNode->addChild(m_childNode);
	return true;
}

bool TroenGame::initializeViews()
{
	m_gameView = new osgViewer::View;
	m_gameView->setCameraManipulator(new osgGA::TrackballManipulator);
	m_gameView->setSceneData(m_rootNode);
	m_gameView->setUpViewOnSingleScreen(0);
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

void TroenGame::startGameLoop()
{
	// simplest version
	//while (!m_sampleOSGViewer->done())
	//{
	//	m_sampleOSGViewer->frame();
	//	std::cout << "insideGameLoop" << std::endl;
	//}

	// game loop from here
	// http://entropyinteractive.com/2011/02/game-engine-design-the-game-loop/

	m_gameIsRunning = true;

	initialize();

	// convert the time to seconds
	chrono::time_point<chrono::high_resolution_clock> nextTime =
		chrono::high_resolution_clock::now();
	chrono::milliseconds maxTimeDiff =
		chrono::milliseconds(200);
	int skippedFrames = 1;
	int maxSkippedFrames = 5;
	while (m_gameIsRunning)
	{
		// convert the time to seconds
		chrono::time_point<chrono::high_resolution_clock> currTime =
			chrono::high_resolution_clock::now();
		// have we rendered a frame recently?
		if ((currTime - nextTime) > maxTimeDiff)
			nextTime = currTime;
		// do we have extra time?
		if (currTime >= nextTime)
		{
			// assign the time for the next update
			nextTime += chrono::milliseconds(10);
			// physics update here
			//update();
			if ((currTime < nextTime) || (skippedFrames > maxSkippedFrames))
			{
				m_sampleOSGViewer->frame();
				skippedFrames = 1;
			}
			else
			{
				skippedFrames++;
			}
		}
		else
		{
			// calculate the time to sleep
			chrono::duration<chrono::high_resolution_clock::rep,chrono::high_resolution_clock::period> sleepTime = nextTime - currTime;
			// sanity check
			if (sleepTime > chrono::duration<chrono::high_resolution_clock::rep, chrono::high_resolution_clock::period>(0))
			{
				// sleep until the next update
				m_gameThread->msleep(sleepTime.count());
			}
		}
	}

	// TODO
	// shutdown();
}