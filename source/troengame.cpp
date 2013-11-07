#include "troengame.h"

#include <osgQt/GraphicsWindowQt>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>

#include "sampleosgviewer.h"

using namespace troen;

TroenGame::TroenGame(osg::GraphicsContext * context, QThread* thread /*= NULL*/) :
m_graphicsContext(context), m_gameThread(thread)
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
	initializeModels();

	initializeViewer();

	composeSceneGraph();

	return true;
}

bool TroenGame::initializeViewer()
{
	m_sampleOSGViewer = new SampleOSGViewer(m_graphicsContext);
	m_sampleOSGViewer->setCameraManipulator(new osgGA::TrackballManipulator);
	m_sampleOSGViewer->setSceneData(m_rootNode.get());
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

