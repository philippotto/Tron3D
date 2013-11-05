#include "viewer.h"

#include <QVBoxLayout>

#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>

#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osgQt/GraphicsWindowQt>
#include <osgUtil/Optimizer>
#include <osg/MatrixTransform>

Viewer::Viewer() : QWidget() {


	
	//osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("data/models/cessna.osg");
	// Use single thread here to avoid known issues under Linux
	setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
	setKeyEventSetsDone(0);

	//m_viewer = new osgViewer::Viewer;
	osgViewer::View* view = new osgViewer::View;
	addView(view);
	
	osgQt::GraphicsWindowQt* gw = createGraphicsWindow(50, 50, 600, 480);
	m_camera = createCamera(gw,view);
	
	//cow.osgt also needs images blueflower and purpleflower to display correctly
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("data/models/cow.osgt");
	osg::ref_ptr<osg::Node> cessna = osgDB::readNodeFile("data/models/cessna.osgt");

	osg::ref_ptr<osg::Group> m_scene = new osg::Group;
	m_scene->addChild(cow.get());

	osg::ref_ptr<osg::MatrixTransform> transformation1 = new osg::MatrixTransform;
	transformation1->setMatrix(osg::Matrix::translate(0.0f, 0.0f, 10.0f));
	transformation1->addChild(cessna.get());
	m_scene->addChild(transformation1.get());

	
	view->setSceneData(m_scene);
	view->addEventHandler(new osgViewer::StatsHandler);
	view->setCameraManipulator(new osgGA::TrackballManipulator);

	
	QVBoxLayout* layout = new QVBoxLayout;
	// setting all margins to 0 make widget disappear...why?
	//layout->setContentsMargins(1,1,1,1);
	layout->addWidget(gw->getGLWidget());
	setLayout(layout);

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
	m_timer.start(40);
}

void Viewer::paintEvent(QPaintEvent* event)
{
	frame();
}

//OSG with Qt
osgQt::GraphicsWindowQt* Viewer::createGraphicsWindow(int x, int y, int w, int h) {
	osg::DisplaySettings* ds =
		osg::DisplaySettings::instance().get();
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->windowDecoration = false;
	traits->x = x;
	traits->y = y;
	traits->width = w;
	traits->height = h;
	traits->doubleBuffer = true;
	traits->alpha = ds->getMinimumNumAlphaBits();
	traits->stencil = ds->getMinimumNumStencilBits();
	traits->sampleBuffers = ds->getMultiSamples();
	traits->samples = ds->getNumMultiSamples();

	osgQt::GraphicsWindowQt* gw = new osgQt::GraphicsWindowQt(traits.get());

	return gw;
}

osg::Camera* Viewer::createCamera(osgQt::GraphicsWindowQt* gw, osgViewer::View* view)
{
	const osg::GraphicsContext::Traits* traits = gw->getTraits();

	osg::ref_ptr<osg::Camera> camera = view->getCamera();// new osg::Camera;
	camera->setGraphicsContext(gw);

	camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);
	return camera.release();
}