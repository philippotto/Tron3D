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
#include <QMessageBox>
// OSG
#include <osg/ref_ptr>
// troen
#include "troengame.h"
#include "network/networkmanager.h"
#include "network/clientmanager.h"

using namespace troen;

#define MAX_BIKES 6

MainWindow::MainWindow(QWidget * parent)
{
	QMainWindow();
    // configure window
    this->setWindowTitle("Troen");

	// create & order widgets
	QTabWidget* tabsContainerWidget = new QTabWidget;
	QWidget* vMainTab = new QWidget;
	QWidget* vNetworkTab = new QWidget;
	QVBoxLayout* vBoxLayout = new QVBoxLayout;
	QVBoxLayout* vBoxLayoutNetwork = new QVBoxLayout;
	vMainTab->setLayout(vBoxLayout);
	vNetworkTab->setLayout(vBoxLayoutNetwork);
	tabsContainerWidget->addTab(vMainTab, "Main");
	tabsContainerWidget->addTab(vNetworkTab, "Network");
	setCentralWidget(tabsContainerWidget);

	////////////////////////////////////////////////////////////////////////////////
	//
	// Main Tab
	//
	////////////////////////////////////////////////////////////////////////////////

	// levelName
	m_levelComboBox = new QComboBox;
	
	// get files from levels folder
	QStringList nameFilter("*.ive");
	QDir directory("data/levels/");
	QStringList levelFiles = directory.entryList(nameFilter);
	foreach(QString currentFile, levelFiles) {
		// remove .ive from string
		currentFile.chop(4);
		m_levelComboBox->addItem(currentFile);
	}

	if (m_levelComboBox->count() == 0){
		QMessageBox::warning(NULL, "Missing levels", "Make sure you have the data/levels folder (see Dropbox).");
	}

	vBoxLayout->addWidget(m_levelComboBox);

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

		QLabel* timeLimitLabel = new QLabel("Time Limit: ");
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

		QString playerString = QString("Player ") + QString::number(i + 1);
		QLabel* playerInputLabel = new QLabel(playerString);
		playerInputLayout->addWidget(playerInputLabel);

		QLineEdit* playerNameLineEdit = new QLineEdit;
		playerNameLineEdit->setText(playerString);
		playerNameLineEdit->setMaximumWidth(75);
		playerNameLineEdit->setMaxLength(20);
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

	// fullscreenCheckBox
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
	m_gameStartButton = new QPushButton(QString("Start Game"));
	m_gameStartButton->setContentsMargins(0, 5, 0, 5);
	vBoxLayout->addWidget(m_gameStartButton);



	// statusBar
	m_statusBar = new QStatusBar(this);
	m_statusBar->showMessage("...");
	setStatusBar(m_statusBar);

	////////////////////////////////////////////////////////////////////////////////
	//
	// Network Tab
	//
	////////////////////////////////////////////////////////////////////////////////

	// server/client parameters
	{
		QWidget* connectionTypeWidget = new QWidget;
		QHBoxLayout* connectionTypeLayout = new QHBoxLayout;
		connectionTypeWidget->setLayout(connectionTypeLayout);
		connectionTypeLayout->setMargin(1);

		m_serverCheckBox = new QCheckBox("Server");
		connectionTypeLayout->addWidget(m_serverCheckBox);

		QLabel* connectToLabel = new QLabel(QString("Connect to:  "));
		connectionTypeLayout->addWidget(connectToLabel);

		m_connectAdressEdit = new QLineEdit;
		m_connectAdressEdit->setText("127.0.0.1");
		m_connectAdressEdit->setMaximumWidth(75);
		m_connectAdressEdit->setMaxLength(50);
		connectionTypeLayout->addWidget(m_connectAdressEdit);

		vBoxLayoutNetwork->addWidget(connectionTypeWidget);
	}

	QWidget* statusWidget = new QWidget;
	statusWidget->setContentsMargins(0, 5, 0, 5);
	QHBoxLayout* statusWidgetLayout = new QHBoxLayout;
	m_statusLabel = new QLabel(QString("No Connection"));
	statusWidgetLayout->addWidget(m_statusLabel);
	statusWidget->setLayout(statusWidgetLayout);
	vBoxLayoutNetwork->addWidget(statusWidget);

	// networkConnectButton
	m_connectNetworkButton = new QPushButton(QString("Connect to Network"));
	m_connectNetworkButton->setContentsMargins(0, 5, 0, 5);
	vBoxLayoutNetwork->addWidget(m_connectNetworkButton);

	m_networkingReady = false;
	m_networkPlayers = 0;

	////////////////////////////////////////////////////////////////////////////////
	//
	// Create Game & Connect Events
	//
	////////////////////////////////////////////////////////////////////////////////

	// settings
	m_settingsFileName = QDir::currentPath() + "/settings.ini";
	loadSettings();

	// create GameThread and Game
	m_gameThread = new QThread(this);
	m_troenGame = new TroenGame(m_gameThread);

	connect(m_connectNetworkButton, SIGNAL(clicked()), this, SLOT(connectNetworking()));
	connect(m_gameStartButton, SIGNAL(clicked()), this, SLOT(prepareGameStart()));
	connect(m_bikeNumberSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePlayerInputBoxes()));
	connect(m_bikeNumberSpinBox, SIGNAL(valueChanged(int)), this, SLOT(bikeNumberChanged(int)));
	connect(m_serverCheckBox, SIGNAL(clicked()), this, SLOT(connectionTypeChanged()));

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

void MainWindow::connectionTypeChanged()
{
	m_connectAdressEdit->setDisabled(m_serverCheckBox->isChecked());
}

void MainWindow::prepareGameStart()
{

	GameConfig config = getGameConfig();
	//TODO: remove ownView checkboxes & replace with spinbox for number of views
	int i = 0;
	for (auto ownViewCheckbox : m_ownViewCheckboxes) {
		config.ownView[i] = ownViewCheckbox->isChecked();
		i++;
	}

	if (m_networkingReady)
		m_troenGame->synchronizeGameStart(config);

	saveSettings();
	emit startGame(config);
}


GameConfig MainWindow::getGameConfig()
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
	config.levelName = m_levelComboBox->currentText().toStdString();
	config.fullscreen = m_fullscreenCheckBox->isChecked();
	config.usePostProcessing = m_postProcessingCheckBox->isChecked();
	config.useDebugView = m_debugViewCheckBox->isChecked();
	config.testPerformance = m_testPerformanceCheckBox->isChecked();
	config.useReflection = m_reflectionCheckBox->isChecked();
	return config;

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

void MainWindow::connectNetworking()
{
	if (m_networkingReady)
	{
		if (!m_serverCheckBox->isChecked())
			m_troenGame->getClientManager()->changeOwnName(m_playerNameLineEdits[0]->text());
		return;
	}

	m_statusLabel->setText(QString("Connecting..."));
	if (m_serverCheckBox->isChecked())
	{
		std::vector<QString> playerNames;
		playerNames.push_back(m_playerNameLineEdits[0]->text());

		// add AIs as seperate players in the server
		for (int i = 0; i < m_playerComboBoxes.size() && i < m_bikeNumberSpinBox->value(); i++)
		{
			if (m_playerComboBoxes[i]->currentText() == "AI")
			{
				m_networkPlayers++;
				playerNames.push_back(m_playerNameLineEdits[i]->text());

			}
		}

		m_troenGame->setupServer(playerNames);
		
		//player_slot = 0;
	}
	else
	{
		m_troenGame->setupClient(m_playerNameLineEdits[0]->text(), m_connectAdressEdit->text().toStdString());
		m_statusLabel->setText(QString("Connected to " + m_connectAdressEdit->text()));
	}

	connect(m_troenGame->getNetworkManager().get(), SIGNAL(remoteStartCall()), this, SLOT(prepareGameStart()));
	connect(m_troenGame->getNetworkManager().get(), SIGNAL(newNetworkPlayer(QString)), this, SLOT(addNetworkPlayer(QString)));
	connect(m_troenGame->getNetworkManager().get(), SIGNAL(playerNameRefused()), this, SLOT(showPlayerNameRefused()));
	connect(m_troenGame->getNetworkManager().get(), SIGNAL(levelChanged(int)), this, SLOT(changeLevel(int)));
	connect(m_levelComboBox, SIGNAL(currentIndexChanged(int)), m_troenGame->getNetworkManager().get(), SLOT(synchronizeLevel(int)));
	
	m_networkingReady = true;
}

void MainWindow::addNetworkPlayer(QString name)
{
	m_networkPlayers++;
	if (m_networkPlayers == 1)
		m_statusLabel->setText(QString(" Connected to ") + name);
	else
		m_statusLabel->setText(m_statusLabel->text() + QString("\n Connected to ") + name);

	m_statusLabel->setStyleSheet("QLabel { background-color : green; color : blue; }");
	m_playerNameLineEdits[m_networkPlayers]->setText(name);
	m_ownViewCheckboxes[m_networkPlayers]->setChecked(false);
	m_bikeNumberSpinBox->setValue(m_networkPlayers+1);
	m_playerComboBoxes[m_networkPlayers]->addItem("Network Player");
	m_playerComboBoxes[m_networkPlayers]->setCurrentText("Network Player");
	bikeNumberChanged(m_networkPlayers+1);
	updatePlayerInputBoxes();

	// send index of level to clients
	if (m_troenGame->getNetworkManager()->isServer())
		m_troenGame->getNetworkManager()->synchronizeLevel(m_levelComboBox->currentIndex());
}

void MainWindow::showPlayerNameRefused()
{
	m_statusLabel->setText(QString("This name is already taken, choose another one."));
	m_statusLabel->setStyleSheet("QLabel { background-color : red; color : blue; }");
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
	m_serverCheckBox->setChecked(settings.value("server").toBool());
	m_levelComboBox->setCurrentIndex(settings.value("level").toInt());

	for (int i = 0; i < MAX_BIKES; i++)
	{
		//name
		QString playerName = settings.value("player" + QString::number(i) + "name").toString();
		if (playerName.isEmpty()) playerName = QString("Player ") + QString::number(i);
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
	settings.setValue("level", QString::number(m_levelComboBox->currentIndex()));

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

void MainWindow::changeLevel(int levelID)
{
	m_levelComboBox->setCurrentIndex(levelID);
}

