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
	m_statusBar->showMessage("...");
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
}

MainWindow::~MainWindow()
{
	m_gameThread->terminate();
}