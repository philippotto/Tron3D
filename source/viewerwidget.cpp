#include "ViewerWidget.h"

#include <QVBoxLayout>

#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osgQt/GraphicsWindowQt>

ViewerWidget::ViewerWidget(osg::Camera* camera, osg::Node* scene) {
	m_viewer = new osgViewer::Viewer;

	m_viewer->setCamera(camera);
	m_viewer->setSceneData(scene);
	m_viewer->addEventHandler(new osgViewer::StatsHandler);
	m_viewer->setCameraManipulator(new osgGA::TrackballManipulator);

	// Use single thread here to avoid known issues under Linux
	m_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	osgQt::GraphicsWindowQt* gw =
		dynamic_cast<osgQt::GraphicsWindowQt*>(camera->getGraphicsContext());
	if (gw)
	{
		QVBoxLayout* layout = new QVBoxLayout;
		layout->addWidget(gw->getGLWidget());
		setLayout(layout);
	}

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
	m_timer.start(40);
}

void ViewerWidget::paintEvent(QPaintEvent* event)
{
	m_viewer->frame();
}