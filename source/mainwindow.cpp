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
#include <QLineEdit>
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

	// timeLimit
	{
		QWidget* timeLimitWidget = new QWidget;
		QHBoxLayout* timeLimitLayout = new QHBoxLayout;
		timeLimitWidget->setLayout(timeLimitLayout);

		QLabel* timeLimitLabel = new QLabel("TimeLimit: ");
		timeLimitLayout->addWidget(timeLimitLabel);

		m_timeLimitSpinBox = new QSpinBox;
		m_timeLimitSpinBox->setMinimum(0);
		m_timeLimitSpinBox->setMaximum(10);
		m_timeLimitSpinBox->setValue(0);
		timeLimitLayout->addWidget(m_timeLimitSpinBox);

		vBoxLayout->addWidget(timeLimitWidget);
	}

	QSignalMapper* signalMapper = new QSignalMapper(this);

	// bikeInputs
	for (int i = 0; i < MAX_BIKES; i++)
	{
		QWidget* playerInputWidget = new QWidget;
		QHBoxLayout* playerInputLayout = new QHBoxLayout;
		playerInputWidget->setLayout(playerInputLayout);
		playerInputLayout->setMargin(1);

		QString playerString = QString("Player_") + QString::number(i);
		QLabel* playerInputLabel = new QLabel(playerString);
		playerInputLayout->addWidget(playerInputLabel);

		QLineEdit* playerNameLineEdit = new QLineEdit;
		playerNameLineEdit->setText(playerString);
		playerNameLineEdit->setMaximumWidth(75);
		playerNameLineEdit->setMaxLength(10);
		m_playerNameLineEdits.push_back(playerNameLineEdit);
		playerInputLayout->addWidget(playerNameLineEdit);

		QComboBox* playerComboBox = new QComboBox;
		playerComboBox->addItem("Keyboard WASD");
		playerComboBox->addItem("Keyboard Arrows");
		playerComboBox->addItem("Gamepad XBox");
		playerComboBox->addItem("GAMEPAD PS4");
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

		// own view
		QCheckBox* ownViewCheckBox = new QCheckBox("&Own view");
		bool isFirstPlayer = i == 0;
		ownViewCheckBox->setChecked(isFirstPlayer);
		ownViewCheckBox->setDisabled(isFirstPlayer);
		m_ownViewCheckboxes.push_back(ownViewCheckBox);
		playerInputLayout->addWidget(ownViewCheckBox);

		playerComboBox->setCurrentIndex(i < 2 ? i : 3);	
		vBoxLayout->addWidget(playerInputWidget);
	}
	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(chooseColor(int)));
	updatePlayerInputBoxes();

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
	connect(m_bikeNumberSpinBox, SIGNAL(valueChanged(int)), this, SLOT(bikeNumberChanged(int)));

	connect(this, SIGNAL(startGame(GameConfig)), m_troenGame, SLOT(prepareAndStartGame(const GameConfig&)));
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
	config.numberOfPlayers = m_bikeNumberSpinBox->value();
	config.timeLimit = m_timeLimitSpinBox->value();
	config.playerInputTypes = new int[config.numberOfPlayers];
	config.playerColors = new QColor[config.numberOfPlayers];
	config.playerNames = new QString[config.numberOfPlayers];
	for (int i = 0; i < config.numberOfPlayers; i++)
	{
		config.playerInputTypes[i] = m_playerComboBoxes.at(i)->currentIndex();
		config.playerNames[i] = m_playerNameLineEdits[i]->text();
		config.playerColors[i] = m_playerColors[i];
	}
	config.fullscreen = m_fullscreenCheckBox->isChecked();
	config.usePostProcessing = m_postProcessingCheckBox->isChecked();
	config.useDebugView = m_debugViewCheckBox->isChecked();
	config.testPerformance = m_testPerformanceCheckBox->isChecked();
	config.useReflection = m_reflectionCheckBox->isChecked();

	//TODO: remove ownView checkboxes & replace with spinbox for number of views
	int i = 0;
	for (auto ownViewCheckbox : m_ownViewCheckboxes) {
		config.ownView[i] = ownViewCheckbox->isChecked();
		i++;
	}

	saveSettings();
	emit startGame(config);
}

void MainWindow::bikeNumberChanged(int newBikeNumber)
{
	for (int i = 1; i < MAX_BIKES; i++)
	{
		if (i < newBikeNumber) {
			m_ownViewCheckboxes[i]->setDisabled(false);
		}
		else {
			m_ownViewCheckboxes[i]->setChecked(false);
			m_ownViewCheckboxes[i]->setDisabled(true);
		}
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
	m_timeLimitSpinBox->setValue(settings.value("timeLimit").toInt());
	m_postProcessingCheckBox->setChecked(settings.value("postProcessing").toBool());
	m_testPerformanceCheckBox->setChecked(settings.value("vSyncOff").toBool());
	m_debugViewCheckBox->setChecked(settings.value("debugView").toBool());
	m_reflectionCheckBox->setChecked(settings.value("reflection").toBool());

	for (int i = 0; i < MAX_BIKES; i++)
	{
		//name
		QString playerName = settings.value("player" + QString::number(i) + "name").toString();
		if (playerName.isEmpty()) playerName = QString("Player_") + QString::number(i);
		m_playerNameLineEdits[i]->setText(playerName);
		//input
		int playerInput =
			settings.value("player" + QString::number(i) + "input").toInt();
		m_playerComboBoxes[i]->setCurrentIndex(playerInput);
		//color
		m_playerColors[i] =
			settings.value("player" + QString::number(i) + "color").value<QColor>();
		m_playerColors[i] = m_playerColors[i].value() == 0 ? initialColors[i] : m_playerColors[i];
		QString colorString = QString("background-color: %1").arg(m_playerColors[i].name());
		m_colorButtons[i]->setStyleSheet(colorString);
		//own view
		m_ownViewCheckboxes[i]->setChecked(settings.value("player" + QString::number(i) + "ownView").toBool() || i == 0);
	}

	updatePlayerInputBoxes();
	bikeNumberChanged(m_bikeNumberSpinBox->value());
}

void MainWindow::saveSettings()
{
	QSettings settings(m_settingsFileName, QSettings::IniFormat);

	settings.setValue("bikeNumber", QString::number(m_bikeNumberSpinBox->value()));
	settings.setValue("timeLimit", QString::number(m_timeLimitSpinBox->value()));
	settings.setValue("postProcessing", QString::number(m_postProcessingCheckBox->isChecked()));
	settings.setValue("vSyncOff", QString::number(m_testPerformanceCheckBox->isChecked()));
	settings.setValue("debugView", QString::number(m_debugViewCheckBox->isChecked()));
	settings.setValue("reflection", QString::number(m_reflectionCheckBox->isChecked()));

	for (int i = 0; i < MAX_BIKES; i++)
	{
		settings.setValue("player" + QString::number(i) + "name",
			m_playerNameLineEdits[i]->text());
		settings.setValue("player" + QString::number(i) + "input",
			m_playerComboBoxes[i]->currentIndex());
		settings.setValue("player" + QString::number(i) + "color",
			m_playerColors[i]);
		settings.setValue("player" + QString::number(i) + "ownView",
			m_ownViewCheckboxes[i]->isChecked());
	}

	settings.sync();
}