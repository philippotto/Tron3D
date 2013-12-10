#include "mainwindow.h"
// Qt
#include <QDir>
#include <QSettings>
#include <QThread>
#include <QBoxLayout>
#include <QLabel>
// OSG
#include <osg/ref_ptr>
// troen
#include "troengame.h"

using namespace troen;

#define MAX_BIKES 6

MainWindow::MainWindow(QWidget * parent)
{
    // configure window
    this->setWindowTitle("Troen");

	// create & order widgets
	QWidget* vBoxWidget = new QWidget;
	QVBoxLayout* vBoxLayout = new QVBoxLayout;
	vBoxWidget->setLayout(vBoxLayout);
	setCentralWidget(vBoxWidget);

	// bikeNumber
	{
		QWidget* bikeNumberWidget = new QWidget;
		QHBoxLayout* bikeNumberLayout = new QHBoxLayout;
		bikeNumberWidget->setLayout(bikeNumberLayout);

		QLabel* bikeNumberLabel = new QLabel("Number of Bikes: ");
		bikeNumberLayout->addWidget(bikeNumberLabel);

		m_bikeNumberSpinBox = new QSpinBox;
		m_bikeNumberSpinBox->setMinimum(2);
		m_bikeNumberSpinBox->setMaximum(6);
		m_bikeNumberSpinBox->setValue(2);
		bikeNumberLayout->addWidget(m_bikeNumberSpinBox);

		vBoxLayout->addWidget(bikeNumberWidget);
	}

	// bikeInputs
	for (int i = 0; i < MAX_BIKES; i++)
	{
		QWidget* playerInputWidget = new QWidget;
		QHBoxLayout* playerInputLayout = new QHBoxLayout;
		playerInputWidget->setLayout(playerInputLayout);
		playerInputLayout->setMargin(1);

		QString playerString = QString("Player ") + QString::number(i);
		QLabel* playerInputLabel = new QLabel(playerString);
		playerInputLayout->addWidget(playerInputLabel);

		QComboBox* playerComboBox = new QComboBox;
		playerComboBox->addItem("KEYBOARD_wasd");
		playerComboBox->addItem("KEYBOARD_arrows");
		playerComboBox->addItem("GAMEPAD");
		playerComboBox->addItem("AI");
		playerInputLayout->addWidget(playerComboBox);
		m_playerComboBoxes.push_back(playerComboBox);

		switch (i)
		{
		case 0:
			playerComboBox->setCurrentIndex(0);
			break;
		case 1:
			playerComboBox->setCurrentIndex(1);
			break;
		default:
			playerComboBox->setCurrentIndex(3);
			break;
		}

		vBoxLayout->addWidget(playerInputWidget);
	}
	updatePlayerInputBoxes();

	// splitscreenCheckBox
	m_splitscreenCheckBox = new QCheckBox("Splitscreen");
	vBoxLayout->addWidget(m_splitscreenCheckBox, 0, Qt::AlignHCenter);

	// splitscreenCheckBox
	m_postProcessingCheckBox = new QCheckBox("PostProcessing");
	vBoxLayout->addWidget(m_postProcessingCheckBox, 0, Qt::AlignHCenter);
	m_postProcessingCheckBox->setChecked(true);

	// gameStartButton
	m_gameStartButton = new QPushButton(QString("start Game"));
	m_gameStartButton->setContentsMargins(0, 5, 0, 5);
	vBoxLayout->addWidget(m_gameStartButton);

	// statusBar
	m_statusBar = new QStatusBar(this);
	m_statusBar->showMessage("...");
	setStatusBar(m_statusBar);

	// settings
	m_settingsFileName = QDir::currentPath() + ":/settings.ini";
	loadSettings();

	// create GameThread and Game
	m_gameThread = new QThread(this);
	m_troenGame = new TroenGame(m_gameThread);

	connect(m_gameStartButton, SIGNAL(clicked()), this, SLOT(prepareGameStart()));
	connect(this, SIGNAL(startGame(GameConfig)), m_troenGame, SLOT(prepareAndStartGame(GameConfig)));
	connect(m_bikeNumberSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePlayerInputBoxes()));
}

MainWindow::~MainWindow()
{
	m_gameThread->terminate();
}

void MainWindow::updatePlayerInputBoxes()
{
	for (int i = 0; i < MAX_BIKES; i++)
	{
		if (i < m_bikeNumberSpinBox->value())
			m_playerComboBoxes.at(i)->setDisabled(false);
		else
			m_playerComboBoxes.at(i)->setDisabled(true);
	}
}

void MainWindow::prepareGameStart()
{
	GameConfig config;
	config.numberOfBikes = m_bikeNumberSpinBox->value();
	config.playerInputTypes = new int[config.numberOfBikes];
	for (int i = 0; i < config.numberOfBikes; i++)
	{
		config.playerInputTypes[i] = m_playerComboBoxes.at(i)->currentIndex();
	}
	config.splitscreen = m_splitscreenCheckBox->isChecked();
	config.usePostProcessing = m_postProcessingCheckBox->isChecked();

	saveSettings();
	emit startGame(config);
}

void MainWindow::loadSettings()
{
	QSettings settings(m_settingsFileName, QSettings::IniFormat);
	
	m_bikeNumberSpinBox->setValue(settings.value("bikeNumber").toInt());
	m_splitscreenCheckBox->setChecked(settings.value("splitscreen").toBool());
	m_postProcessingCheckBox->setChecked(settings.value("postProcessing").toBool());

	for (int i = 0; i < MAX_BIKES; i++)
	{
		int playerInput =
			settings.value("player" + QString::number(i) + "input").toInt();
		m_playerComboBoxes.at(i)->setCurrentIndex(playerInput);
	}

	updatePlayerInputBoxes();
}

void MainWindow::saveSettings()
{
	QSettings settings(m_settingsFileName, QSettings::IniFormat);

	settings.setValue("bikeNumber", QString::number(m_bikeNumberSpinBox->value()));
	settings.setValue("splitscreen", QString::number(m_splitscreenCheckBox->isChecked()));
	settings.setValue("postProcessing", QString::number(m_postProcessingCheckBox->isChecked()));

	for (int i = 0; i < MAX_BIKES; i++)
	{
		settings.setValue("player" + QString::number(i) + "input",
			m_playerComboBoxes.at(i)->currentIndex());
	}

	settings.sync();
}