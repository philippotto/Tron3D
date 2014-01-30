#include "mainwindow.h"
// Qt
#include <QDir>
#include <QSettings>
#include <QThread>
#include <QBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QSignalMapper>
// OSG
#include <osg/ref_ptr>
// troen
#include "troengame.h"

using namespace troen;

#define MAX_BIKES 6

MainWindow::MainWindow(QWidget * parent)
{
	QMainWindow();
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
		m_bikeNumberSpinBox->setMinimum(1);
		m_bikeNumberSpinBox->setMaximum(6);
		m_bikeNumberSpinBox->setValue(1);
		bikeNumberLayout->addWidget(m_bikeNumberSpinBox);

		vBoxLayout->addWidget(bikeNumberWidget);
	}

	QSignalMapper* signalMapper = new QSignalMapper(this);

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
		playerComboBox->addItem("GAMEPADPS4");
		playerComboBox->addItem("AI");
		playerInputLayout->addWidget(playerComboBox);
		m_playerComboBoxes.push_back(playerComboBox);

		QPushButton* colorButton = new QPushButton();
		colorButton->setContentsMargins(0, 5, 0, 5);
		signalMapper->setMapping(colorButton, i);
		connect(colorButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
		playerInputLayout->addWidget(colorButton);
		m_playerColors.push_back(QColor());
		m_colorButtons.push_back(colorButton);

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
	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(chooseColor(int)));
	updatePlayerInputBoxes();

	// splitscreenCheckBox
	m_splitscreenCheckBox = new QCheckBox("&Splitscreen");
	m_splitscreenCheckBox->setChecked(false);
	m_splitscreenCheckBox->setDisabled(true);
	vBoxLayout->addWidget(m_splitscreenCheckBox, 0, Qt::AlignLeft);

	//fullscreenCheckBox
	m_fullscreenCheckBox = new QCheckBox("&Fullscreen");
	vBoxLayout->addWidget(m_fullscreenCheckBox, 0, Qt::AlignLeft);

	// postProcessingCheckBox
	m_postProcessingCheckBox = new QCheckBox("&PostProcessing");
	vBoxLayout->addWidget(m_postProcessingCheckBox, 0, Qt::AlignLeft);
	m_postProcessingCheckBox->setChecked(false);

	// postProcessingCheckBox
	m_testPerformanceCheckBox = new QCheckBox("&TestPerformance - vSync OFF");
	vBoxLayout->addWidget(m_testPerformanceCheckBox, 0, Qt::AlignLeft);
	m_testPerformanceCheckBox->setChecked(true);

	// debugViewCheckBox
	m_debugViewCheckBox = new QCheckBox("&DebugView");
	vBoxLayout->addWidget(m_debugViewCheckBox, 0, Qt::AlignLeft);
	m_debugViewCheckBox->setChecked(false);

	//reflectionCheckBox
	m_reflectionCheckBox = new QCheckBox("&Reflection");
	vBoxLayout->addWidget(m_reflectionCheckBox, 0, Qt::AlignLeft);
	m_reflectionCheckBox->setChecked(true);

	// gameStartButton
	m_gameStartButton = new QPushButton(QString("start Game"));
	m_gameStartButton->setContentsMargins(0, 5, 0, 5);
	vBoxLayout->addWidget(m_gameStartButton);

	// statusBar
	m_statusBar = new QStatusBar(this);
	m_statusBar->showMessage("...");
	setStatusBar(m_statusBar);

	// settings
	m_settingsFileName = QDir::currentPath() + "/settings.ini";
	loadSettings();

	// create GameThread and Game
	m_gameThread = new QThread(this);
	m_troenGame = new TroenGame(m_gameThread);

	connect(m_gameStartButton, SIGNAL(clicked()), this, SLOT(prepareGameStart()));
	connect(m_bikeNumberSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePlayerInputBoxes()));
	connect(m_splitscreenCheckBox, SIGNAL(stateChanged(int)), this, SLOT(splitscreenToggled()));
	connect(m_fullscreenCheckBox, SIGNAL(stateChanged(int)), this, SLOT(fullscreenToggled()));
	connect(m_bikeNumberSpinBox, SIGNAL(valueChanged(int)), this, SLOT(bikeNumberChanged(int)));

	connect(this, SIGNAL(startGame(GameConfig)), m_troenGame, SLOT(prepareAndStartGame(GameConfig)));
}

MainWindow::~MainWindow()
{
	m_gameThread->terminate();
}

void MainWindow::chooseColor(int i)
{
	m_playerColors[i] = QColorDialog::getColor(m_playerColors[i]);
	QString colorString = QString("background-color: %1").arg(m_playerColors[i].name());
	m_colorButtons[i]->setStyleSheet(colorString);
}

void MainWindow::updatePlayerInputBoxes()
{
	for (int i = 0; i < MAX_BIKES; i++)
	{
		if (i < m_bikeNumberSpinBox->value())
		{
			m_playerComboBoxes.at(i)->setDisabled(false);
			m_colorButtons.at(i)->setDisabled(false);
		}
		else
		{
			m_playerComboBoxes.at(i)->setDisabled(true);
			m_colorButtons.at(i)->setDisabled(true);
		}
	}
}

void MainWindow::prepareGameStart()
{
	GameConfig config;
	config.numberOfBikes = m_bikeNumberSpinBox->value();
	config.playerInputTypes = new int[config.numberOfBikes];
	config.playerColors = new QColor[config.numberOfBikes];
	for (int i = 0; i < config.numberOfBikes; i++)
	{
		config.playerInputTypes[i] = m_playerComboBoxes.at(i)->currentIndex();
	}
	for (int j = 0; j < config.numberOfBikes; j++)
	{
		config.playerColors[j] = m_playerColors[j];
	}
	config.splitscreen = m_splitscreenCheckBox->isChecked();
	config.fullscreen = m_fullscreenCheckBox->isChecked();
	config.usePostProcessing = m_postProcessingCheckBox->isChecked();
	config.useDebugView = m_debugViewCheckBox->isChecked();
	config.testPerformance = m_testPerformanceCheckBox->isChecked();
	config.reflection = m_reflectionCheckBox->isChecked();

	saveSettings();
	emit startGame(config);
}

void MainWindow::splitscreenToggled()
{
	if (m_splitscreenCheckBox->isChecked())
		m_fullscreenCheckBox->setChecked(false);
}

void MainWindow::fullscreenToggled()
{
	if (m_fullscreenCheckBox->isChecked())
		m_splitscreenCheckBox->setChecked(false);
}

void MainWindow::bikeNumberChanged(int newBikeNumber)
{
	if (newBikeNumber < 2)
	{
		m_splitscreenCheckBox->setChecked(false);
		m_splitscreenCheckBox->setCheckable(false);
		m_splitscreenCheckBox->setDisabled(true);
	}
	else
	{
		m_splitscreenCheckBox->setCheckable(true);
		m_splitscreenCheckBox->setDisabled(false);
	}
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		if (keyEvent->key() == Qt::Key_Return) {
			this->prepareGameStart();
			keyEvent->accept();
			return true;
		}
		else if (keyEvent->key() == Qt::Key_Escape)
		{
			this->close();
			keyEvent->accept();
			return true;
		}
	}
	return QMainWindow::eventFilter(object, event);
}

void MainWindow::childEvent(QChildEvent* e)
{
	if (e->child()->isWidgetType()) {
		if (e->type() == QEvent::ChildAdded) {
			e->child()->installEventFilter(this);
		}
		else if (e->type() == QEvent::ChildRemoved) {
			e->child()->removeEventFilter(this);
		}
	}
	QWidget::childEvent(e);
}

void MainWindow::loadSettings()
{
	QSettings settings(m_settingsFileName, QSettings::IniFormat);

	std::vector<QColor> initialColors{ QColor(255.0, 0.0, 0.0), QColor(0.0, 255.0, 0.0), QColor(0.0, 0.0, 255.0),
		QColor(255.0, 255.0, 0.0), QColor(0.0, 255.0, 255.0), QColor(255.0, 0.0, 255.0) };

	m_bikeNumberSpinBox->setValue(settings.value("bikeNumber").toInt());
	m_splitscreenCheckBox->setChecked(settings.value("splitscreen").toBool());
	m_postProcessingCheckBox->setChecked(settings.value("postProcessing").toBool());
	m_testPerformanceCheckBox->setChecked(settings.value("vSyncOff").toBool());
	m_debugViewCheckBox->setChecked(settings.value("debugView").toBool());
	m_reflectionCheckBox->setChecked(settings.value("reflection").toBool());

	for (int i = 0; i < MAX_BIKES; i++)
	{
		int playerInput =
			settings.value("player" + QString::number(i) + "input").toInt();
		m_playerComboBoxes[i]->setCurrentIndex(playerInput);
		m_playerColors[i] =
			settings.value("player" + QString::number(i) + "color").value<QColor>();
		m_playerColors[i] = m_playerColors[i].value() == 0 ? initialColors[i] : m_playerColors[i];
		QString colorString = QString("background-color: %1").arg(m_playerColors[i].name());
		m_colorButtons[i]->setStyleSheet(colorString);
	}

	updatePlayerInputBoxes();
}

void MainWindow::saveSettings()
{
	QSettings settings(m_settingsFileName, QSettings::IniFormat);

	settings.setValue("bikeNumber", QString::number(m_bikeNumberSpinBox->value()));
	settings.setValue("splitscreen", QString::number(m_splitscreenCheckBox->isChecked()));
	settings.setValue("postProcessing", QString::number(m_postProcessingCheckBox->isChecked()));
	settings.setValue("vSyncOff", QString::number(m_testPerformanceCheckBox->isChecked()));
	settings.setValue("debugView", QString::number(m_debugViewCheckBox->isChecked()));
	settings.setValue("reflection", QString::number(m_reflectionCheckBox->isChecked()));

	for (int i = 0; i < MAX_BIKES; i++)
	{
		settings.setValue("player" + QString::number(i) + "input",
			m_playerComboBoxes[i]->currentIndex());
		settings.setValue("player" + QString::number(i) + "color",
			m_playerColors[i]);
	}

	settings.sync();
}