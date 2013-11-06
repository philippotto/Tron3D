#include "viewer.h"

#include <memory>

#include <QVBoxLayout>

#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>

#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osgQt/GraphicsWindowQt>
#include <osg/ShapeDrawable>
#include <osg/Vec3>

#include "keyboardeventhandler.h"
#include "bikeinputstate.h"
#include "updatebikepositioncallback.h"

Viewer::Viewer() {
	m_viewer = new osgViewer::Viewer;

	m_camera = createCamera(50, 50, 600, 480);
	m_scene = osgDB::readNodeFile("data/models/cow.osg");
	//osg::Box* box = new osg::Box(osg::Vec3(0.0, 0.0, 0.0), 100.0);
	//osg::ref_ptr<osg::Geode> m_scene = new osg::Geode;
	//m_scene->addDrawable(new osg::ShapeDrawable(box));

	m_viewer->setCamera(m_camera);
	m_viewer->setSceneData(m_scene);
	m_viewer->addEventHandler(new osgViewer::StatsHandler);
	m_viewer->setCameraManipulator(new osgGA::TrackballManipulator);

	// Use single thread here to avoid known issues under Linux
	m_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	osgQt::GraphicsWindowQt* gw =
		dynamic_cast<osgQt::GraphicsWindowQt*>(m_camera->getGraphicsContext());
	if (gw)
	{
		QVBoxLayout* layout = new QVBoxLayout;
		// setting all margins to 0 make widget disappear...why?
		layout->setContentsMargins(1, 1, 1, 1);
		layout->addWidget(gw->getGLWidget());
		setLayout(layout);
	}

	// add keyboard event handling
	BikeInputState* bikeInputState = new BikeInputState();

	m_scene->setUpdateCallback(new UpdateBikePositionCallback(bikeInputState));

	osg::ref_ptr<KeyboardEventHandler> keyboardHandler = new KeyboardEventHandler(bikeInputState);
	m_viewer->addEventHandler(keyboardHandler);

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
	m_timer.start(40);
}

void Viewer::paintEvent(QPaintEvent* event)
{
	m_viewer->frame();
}

void Viewer::run()
{
	m_viewer->run();
}

//OSG with Qt
osg::Camera* Viewer::createCamera(int x, int y, int w, int h) {
	osg::DisplaySettings* ds =
		osg::DisplaySettings::instance().get();
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->windowDecoration = false;
	traits->x = x;
	traits->y = y;
	traits->width = w;
	traits->height = h;
	traits->doubleBuffer = true;

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(new osgQt::GraphicsWindowQt(traits.get()));
	camera->setClearColor(osg::Vec4(0.8, 0.8, 0.8, 1.0));
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) /
		static_cast<double>(traits->height), 1.0f, 10000.0f);
	return camera.release();
}