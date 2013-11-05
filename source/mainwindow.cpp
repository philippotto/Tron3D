#include "mainwindow.h"

#include <QtOpenGL>
#include <QDesktopWidget>

#include <osg/ref_ptr>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>

#include "viewer.h"


MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags)
:   QMainWindow(parent, flags)
{
    // configure window
    this->setWindowTitle("Troen");

	// set window size
	QDesktopWidget desktopWidget;
	QRect displaySize = desktopWidget.screenGeometry();
	this->resize(displaySize.width() / 3 * 2, displaySize.height() / 3 * 2);

	this->m_viewer = new Viewer();
	this->setCentralWidget(this->m_viewer);

	// FMOD TEST
	/*
	m_fmodManager = cFMODManager::GetInstance();
	m_fmodManager->InitFMOD(MAX_CHANNELS, FMOD_INIT_NORMAL);
	uint soundIndex0 = m_fmodManager->LoadSound("sound.mp3", false, false, 1.0f);
	uint soundIndex1 = m_fmodManager->LoadSound("sound.mp3", false, false, 1.0f);
	uint soundIndex2 = m_fmodManager->LoadSound("sound.mp3", false, false, 1.0f);

	std::cout << soundIndex0 << soundIndex1 << soundIndex2 << std::endl;
	m_fmodManager->PlayFMODSound(soundIndex2);
	*/

}

MainWindow::~MainWindow()
{
	delete m_viewer;
}