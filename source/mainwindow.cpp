#include "mainwindow.h"
// Qt
#include <QThread>
// OSG
#include <osg/ref_ptr>
// troen
#include "troengame.h"

using namespace troen;

MainWindow::MainWindow(QWidget * parent)
{
    // configure window
    this->setWindowTitle("Troen");

	// create widgets
	m_statusBar = new QStatusBar(this);
	m_statusBar->showMessage("fps:");
	m_stackedWidget = new QStackedWidget(this);
	m_pushButton = new QPushButton(QString("start GameLoop"), m_stackedWidget);

	// order widgets
	int index = m_stackedWidget->addWidget(m_pushButton);
	m_stackedWidget->setCurrentIndex(index);
	setCentralWidget(this->m_stackedWidget);
	setStatusBar(m_statusBar);

	// create GameThread and Game
	m_gameThread = new QThread(this);
	m_troenGame = new TroenGame(m_gameThread);

	connect(m_pushButton, SIGNAL(clicked()), m_troenGame, SLOT(startGameLoop()) );
	connect(m_troenGame, SIGNAL(newFrame(double)), this, SLOT(fpsChanged(double)));
}

MainWindow::~MainWindow()
{
	m_gameThread->terminate();
}

void MainWindow::fpsChanged(double time)
{
	double timeBetweenFrames = time - m_lastTime;
	m_lastTime = time;
	if (timeBetweenFrames < 0) return;

	float fps = 1 / (timeBetweenFrames / 1000);
	if (abs(fps - m_fps) > 4)
	{
		m_fps = fps;
		QString fpsString;
		fpsString.setNum(m_fps);
		fpsString = "fps: " + fpsString;
		m_statusBar->showMessage(fpsString);
	}
}