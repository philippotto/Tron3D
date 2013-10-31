#include "viewerwidget.h"

ViewerWidget::ViewerWidget(osg::Camera* camera, osg::Node* scene) {

	_viewer.setCamera(camera);
	_viewer.setSceneData(scene);
	_viewer.addEventHandler(new osgViewer::StatsHandler);
	_viewer.setCameraManipulator(
		new osgGA::TrackballManipulator);

	osgQt::GraphicsWindowQt* gw =
		dynamic_cast<osgQt::GraphicsWindowQt*>(
		camera->getGraphicsContext());
	if (gw)
	{
		QVBoxLayout* layout = new QVBoxLayout;
		layout->addWidget(gw->getGLWidget());
		setLayout(layout);
	}

	connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
	_timer.start(40);
}