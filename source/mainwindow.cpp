
#include "mainwindow.h"

#include <QtOpenGL>
#include <QDesktopWidget>


MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags)
:   QMainWindow(parent, flags),
    glWidget(NULL)
{
    // configure window
    setWindowTitle("Troen");
    
    // set window size
    QDesktopWidget desktopWidget;
    QRect displaySize = desktopWidget.screenGeometry();
    this->resize(displaySize.width()/3*2, displaySize.height()/3*2);
	
    // probably subclass the QGLWidget class for specific purposes
    //glWidget = new QGLWidget;
    //setCentralWidget(glWidget);

	m_camera = createCamera(50, 50, 640, 480);
	m_scene = osgDB::readNodeFile("cow.osg");

	// create and open MainWindow (subclass of QWidget)
	m_viewerWidget = new ViewerWidget(m_camera, m_scene);
	m_viewerWidget->setGeometry(100, 100, 800, 600);
	m_viewerWidget->show();

	//setCentralWidget(m_viewerWidget);
}

MainWindow::~MainWindow()
{
	delete m_viewerWidget;
}

//OSG with Qt
osg::Camera* MainWindow::createCamera(int x, int y, int w, int h) {
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
	camera->setGraphicsContext(new osgQt::GraphicsWindowQt(traits.get(),this));
	camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
	camera->setViewport(new osg::Viewport( 0, 0, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective( 30.0f, static_cast<double>(traits->width) /
		static_cast<double>(traits->height), 1.0f, 10000.0f);
	return camera.release();
}