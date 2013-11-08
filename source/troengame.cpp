#include "troengame.h"

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>

#include "sampleosgviewer.h"

using namespace troen;

TroenGame::TroenGame(QThread* thread /*= NULL*/) :
		m_gameThread(thread)
{
	if (m_gameThread == NULL) {
		m_gameThread = new QThread(this);
	}
	moveToThread(m_gameThread);

	m_gameThread->start();

	initialize();
}

TroenGame::~TroenGame()
{
}

bool TroenGame::initialize()
{
	m_rootNode = new osg::Group;

	// order of initialization is important (models before viewer)

	// TODO
	// initialize physics, sound, level and ... here

	initializeModels();

	initializeViewer();

	composeSceneGraph();

	return true;
}

bool TroenGame::initializeViewer()
{
	m_sampleOSGViewer = new SampleOSGViewer();
	// TODO
	// view setup here at the moment, should be moved to own initialize call
	m_gameView = new osgViewer::View;
	m_gameView->setCameraManipulator(new osgGA::TrackballManipulator);
	m_gameView->setSceneData(m_rootNode);
	m_gameView->setUpViewOnSingleScreen(0);
	m_sampleOSGViewer->addView(m_gameView);

	// TODO
	// add event handlers to the viewer here
	return true;
}

bool TroenGame::initializeModels()
{
	m_childNode = osgDB::readNodeFile("data/models/cow.osgt");
	return true;
}

bool TroenGame::composeSceneGraph()
{
	m_rootNode->addChild(m_childNode);
	return true;
}

void TroenGame::startGameLoop()
{
	while (true)
	{
		m_sampleOSGViewer->frame();
		std::cout << "insideGameLoop" << std::endl;
	}
}