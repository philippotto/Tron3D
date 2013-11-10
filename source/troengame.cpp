#include "troengame.h"

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>

#include <chrono>

#include "sampleosgviewer.h"
#include "util\chronotimer.h"

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

	initializeTimer();

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

bool TroenGame::initializeTimer()
{
	m_timer = make_shared<util::ChronoTimer>(false, true);
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

	// INITIALIZATION
	initialize();
	m_gameIsRunning = true;
	m_timer->start();

	// GAME LOOP VARIABLES
	long double nextTime = m_timer->elapsed();
	const long double minMillisecondsBetweenFrames = 10;
	const long double maxMillisecondsBetweenFrames = 50;
	int skippedFrames = 0;
	int maxSkippedFrames = 4;

	// GAME LOOP
	while (m_gameIsRunning)
	{
		long double currTime = m_timer->elapsed();

		// are we significantly behind? if yes, "resync", force rendering
		if ((currTime - nextTime) > maxMillisecondsBetweenFrames)
			nextTime = currTime;
		// is it time to render the next frame?
		if (currTime >= nextTime)
		{
			std::cout << "difference: " << currTime - nextTime << std::endl;
			// assign the time for the next update
			nextTime += minMillisecondsBetweenFrames;

			// LOOP REALLY STARTS HERE:
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
				std::cout << "sleep for: " << sleepTime << std::endl;
				m_gameThread->msleep(sleepTime);
			}
		}
	}
	// TODO
	// shutdown();
}