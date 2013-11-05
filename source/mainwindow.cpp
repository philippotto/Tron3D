#include "MainWindow.h"

#include <QtOpenGL>
#include <QDesktopWidget>

#include <osg/ref_ptr>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>

#include "ViewerWidget.h"


MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags)
:   QMainWindow(parent, flags)
{
    // configure window
    setWindowTitle("Troen");
    
    // set window size
    QDesktopWidget desktopWidget;
    QRect displaySize = desktopWidget.screenGeometry();
    this->resize(displaySize.width()/3*2, displaySize.height()/3*2);

	// FMOD TEST
	/*
	m_fmodManager = cFMODManager::GetInstance();
	m_fmodManager->InitFMOD(MAX_CHANNELS, BG_MUSIC_BASE);
	uint soundIndex0 = m_fmodManager->LoadSound("sound.mp3", false, false, 1.0f);
	uint soundIndex1 = m_fmodManager->LoadSound("sound.mp3", false, false, 1.0f);
	uint soundIndex2 = m_fmodManager->LoadSound("sound.mp3", false, false, 1.0f);

	std::cout << soundIndex0 << soundIndex1 << soundIndex2 << std::endl;
	m_fmodManager->PlayFMODSound(soundIndex2);
	*/

	m_camera = createCamera(50, 50, displaySize.width() / 3 * 2, displaySize.height() / 3 * 2);
	m_scene = osgDB::readNodeFile("data/models/cycle/HQ_Movie cycle.obj");

	m_viewerWidget = new ViewerWidget(m_camera, m_scene);
	setCentralWidget(m_viewerWidget);
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
	camera->setGraphicsContext(new osgQt::GraphicsWindowQt(traits.get()));
	camera->setClearColor(osg::Vec4(0.8, 0.8, 0.8, 1.0));
	camera->setViewport(new osg::Viewport( 0, 0, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective( 30.0f, static_cast<double>(traits->width) /
		static_cast<double>(traits->height), 1.0f, 10000.0f);
	return camera.release();
}