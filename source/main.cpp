#include "mainwindow.h"

#include <iostream>
#include <chrono>
#include <ctime>

#include <QApplication>

#include "viewerwidget.h"

using namespace std;

//OSG with Qt
osg::Camera* createCamera(int x, int y, int w, int h) {
	osg::DisplaySettings* ds =
		osg::DisplaySettings::instance().get();
	osg::ref_ptr<osg::GraphicsContext::Traits> traits =
		new osg::GraphicsContext::Traits;
	traits->windowDecoration = false;
	traits->x = x;
	traits->y = y;
	traits->width = w;
	traits->height = h;
	traits->doubleBuffer = true;
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(
		new osgQt::GraphicsWindowQt(traits.get()));
	camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
	camera->setViewport(new osg::Viewport(
		0, 0, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width) /
		static_cast<double>(traits->height), 1.0f, 10000.0f);
	return camera.release();
}


int main(int argc, char* argv[])
{
    int result = -1;

    // setup application base settings
    QApplication::setApplicationName("Troen");
	QApplication * application = new QApplication(argc, argv);

	osg::Camera* camera = createCamera(50, 50, 640, 480);
	osg::Node* scene = osgDB::readNodeFile("cow.osg");

	// create and open MainWindow (subclass of QWidget)
	ViewerWidget* widget = new ViewerWidget(camera, scene);
	widget->setGeometry(100, 100, 800, 600);
	widget->show();

	result = application->exec();

	// Clean Up
	delete widget;
	delete application;

    return result;
}