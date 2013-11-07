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
	//this->setWindowState(Qt::WindowMaximized);

	// create widgets
	m_stackedWidget = new QStackedWidget(this);
	m_osgWidget = new OSGWidget(m_stackedWidget);
	m_pushButton = new QPushButton(QString("start GameLoop"), m_stackedWidget);

	// order widgets
	m_stackedWidget->addWidget(m_osgWidget);
	int index = m_stackedWidget->addWidget(m_pushButton);
	m_stackedWidget->setCurrentIndex(index);
	setCentralWidget(this->m_stackedWidget);

	m_gameThread = new QThread(this);

	m_troenGame = new TroenGame(m_osgWidget->graphicsContext(), m_gameThread);

	connect(m_pushButton, SIGNAL(clicked()), this, SLOT(showGame()) );
	connect(m_pushButton, SIGNAL(clicked()), m_troenGame, SLOT(startGameLoop()) );

}

MainWindow::~MainWindow()
{
	delete m_troenGame;
	// need to figure out how threads are correctly shut down
	m_gameThread->terminate();
	delete m_gameThread;
	delete m_osgWidget;
	delete m_stackedWidget;
}

void MainWindow::showGame()
{
	m_stackedWidget->setCurrentWidget(m_osgWidget);
}