#include "mainwindow.h"

#include <QThread>

#include <osg/ref_ptr>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>

#include "osgwidget.h"
#include "troengame.h"

using namespace troen;

MainWindow::MainWindow(QWidget * parent)
{
    // configure window
    this->setWindowTitle("Troen");
	this->setWindowState(Qt::WindowMaximized);
	
	m_osgWidget = new OSGWidget();
	setCentralWidget(this->m_osgWidget);

	m_gameThread = new QThread(this);

	m_troenGame = new TroenGame(m_osgWidget->graphicsContext(), m_gameThread);


}

MainWindow::~MainWindow()
{
	delete m_troenGame;
	delete m_gameThread;
	delete m_osgWidget;
}