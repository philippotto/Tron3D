#include "viewer.h"

#include <QVBoxLayout>

#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>

#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osgQt/GraphicsWindowQt>

#include <iostream>

Viewer::Viewer() {
	m_viewer = new osgViewer::Viewer;

	m_camera = createCamera(50, 50, 600, 480);
	m_scene = osgDB::readNodeFile("data/models/cow.osg");


	osg::ref_ptr<osg::Group> group = new osg::Group();
	
	group->addChild(m_scene);
	group->addChild(createAxis());

	m_viewer->setCamera(m_camera);
	m_viewer->setSceneData(group.get());
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
		layout->setContentsMargins(1,1,1,1);
		layout->addWidget(gw->getGLWidget());
		setLayout(layout);
	}

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
	m_timer.start(40);
}

void Viewer::paintEvent(QPaintEvent* event)
{
	std::cout << "painting";
	m_viewer->frame();
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

	// right handed system, x ->, y ^, z o
	camera->setViewMatrixAsLookAt(osg::Vec3(0, 0, -1), osg::Vec3(0, 0, 0), osg::Vec3(0, 1, 0));
	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) /
		static_cast<double>(traits->height), 1.0f, 10000.0f);
	return camera.release();
}



osg::Geode* Viewer::createAxis()
{
    osg::Geode*     geode    = new osg::Geode();
    osg::Geometry*  geometry = new osg::Geometry();
    osg::Vec3Array* vertices = new osg::Vec3Array();
    osg::Vec4Array* colors   = new osg::Vec4Array();
 
    vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
    vertices->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
    vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
    vertices->push_back(osg::Vec3(0.0f, 1.0f, 0.0f));
    vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
    vertices->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
	
	// x is red, y is green and z is blue
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
 
    geometry->setVertexArray(vertices);
    geometry->setColorArray(colors, osg::Array::BIND_PER_VERTEX);
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 6));
    geometry->getOrCreateStateSet()->setMode(GL_LIGHTING, false);
 
    geode->addDrawable(geometry);
 
    return geode;
}