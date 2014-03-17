#include "troengame.h"
// OSG
#include <osg/LineWidth>
#include <osgUtil/Optimizer>
// qt
#include <qcoreapplication>
// troen
#include "troengamebuilder.h"
#include "constants.h"
#include "globals.h"
#include "player.h"
#include "gamelogic.h"
#include "sampleosgviewer.h"

#include "controller/levelcontroller.h"
#include "controller/bikecontroller.h"
#include "controller/hudcontroller.h"

#include "model/physicsworld.h"

#include "view/postprocessing.h"
#include "view/reflection.h"

#include "util/chronotimer.h"
#include "util/gldebugdrawer.h"
#include "sound/audiomanager.h"

#include "network/clientmanager.h"
#include "network/servermanager.h"
#include <thread>

using namespace troen;
extern long double g_currentTime;

TroenGame::TroenGame(QThread* thread /*= nullptr*/) :
QObject(),
m_gameConfig(nullptr),
m_gameThread(thread)
{
	if (m_gameThread == nullptr) {
		m_gameThread = new QThread(this);
	}
	moveToThread(m_gameThread);
	m_gameThread->start(QThread::HighestPriority);
	m_ServerManager = NULL;
	m_ClientManager = NULL;
}




////////////////////////////////////////////////////////////////////////////////
//
// Game Loop
//
////////////////////////////////////////////////////////////////////////////////

void TroenGame::prepareAndStartGame(const GameConfig& gameConfig)
{
	m_gameConfig = std::make_shared<GameConfig>(gameConfig);
	startGameLoop();
}

void TroenGame::startGameLoop()
{
	// adaptive game loop from here:
	// http://entropyinteractive.com/2011/02/game-engine-design-the-game-loop/

	// INITIALIZATION
	TroenGameBuilder builder(this);
	builder.build();

	if (m_gameConfig->useDebugView)
		m_sceneNode->addChild(m_physicsWorld->m_debug->getSceneGraph());

	if (m_gameConfig->fullscreen)
		setupForFullScreen();

	m_gameloopTimer->start();
	m_gameTimer->start();
	m_gameTimer->pause();

	if (isNetworking())
	{
		getNetworkManager()->setLocalGameReady();
		getNetworkManager()->waitOnAllPlayers(); //blocking call
	}

	// GAME LOOP VARIABLES
	long double nextTime = m_gameloopTimer->elapsed();
	const double minMillisecondsBetweenFrames = 16.7; // vSync to 60 fps
	const double maxMillisecondsBetweenFrames = 4 * minMillisecondsBetweenFrames + 1;
	int skippedFrames = 0;
	const int maxSkippedFrames = 4;

	bool nearPlaneAdapted = false;

	// GAME LOOP
	// - AI
	// - network
	// - checkForUserInput and updateModels
	// - physics + updateViews
	// - render;

	// terminates when first viewer is closed
	while (!m_players[0]->viewer()->done())
	{
		g_gameLoopTime = m_gameloopTimer->elapsed();
		g_gameTime = m_gameTimer->elapsed();

		QCoreApplication::processEvents();

		// are we significantly behind? if yes, "resync", force rendering
		if ((g_gameLoopTime - nextTime) > maxMillisecondsBetweenFrames)
			nextTime = g_gameLoopTime;
		// is it time to render the next frame?
		if (m_gameConfig->testPerformance || g_gameLoopTime >= nextTime)
		{
			// assign the time for the next update
			nextTime += minMillisecondsBetweenFrames;

			// LOOP REALLY STARTS HERE:
			m_gameLogic->step(g_gameLoopTime, g_gameTime);
			if (!m_gameTimer->paused())
			{
				for (auto player : m_players)
				{
					player->bikeController()->updateModel(g_gameTime);
				}
				m_physicsWorld->stepSimulation(g_gameTime);
				m_levelController->update();
			}

			if (isNetworking())
				getNetworkManager()->update(g_gameTime);

			m_audioManager->Update(g_gameLoopTime / 1000);
			m_audioManager->setMotorSpeed(m_players[0]->bikeController()->speed());

			if (m_postProcessing) m_postProcessing->setBeat(m_audioManager->getTimeSinceLastBeat());

			// do we have extra time (to draw the frame) or did we skip too many frames already?
			if (g_gameLoopTime < nextTime || (skippedFrames > maxSkippedFrames))
			{
				for (auto player : m_playersWithView)
				{
					player->hudController()->update(
						g_gameLoopTime,
						g_gameTime,
						m_gameConfig->timeLimit,
						m_gameLogic->getGameState(),
						m_players);
				}

				for (auto player : m_playersWithView)
				{
					player->viewer()->frame();
				}
				// TODO: find a way to eleminate this workaround
				// doesn't work if it's executed earlier
				if (!nearPlaneAdapted)
				{
					for (auto player : m_playersWithView)
					{
						fixCulling(player->gameView());
					}
				}
				skippedFrames = 0;
			}
			else
				skippedFrames++;
		}
		else // WAIT
		{
			// calculate the time to sleep
			long double sleepTime = (nextTime - g_gameLoopTime);
			if (sleepTime > 0)	// sanity check, sleep until nextTime
			if (!m_gameConfig->testPerformance) m_gameThread->msleep(sleepTime);
		}
	}

	if (m_gameConfig->fullscreen)
		returnFromFullScreen();

	// SHUTDOWN
	builder.destroy();
}

void TroenGame::fixCulling(osg::ref_ptr<osgViewer::View> view)
{
	double fovy, aspect, znear, zfar;
	view->getCamera()->getProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
	view->getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	znear = 1.0;
	view->getCamera()->setProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
}

////////////////////////////////////////////////////////////////////////////////
//
// Full Screen Handling
//
////////////////////////////////////////////////////////////////////////////////

void TroenGame::setupForFullScreen()
{
	osg::GraphicsContext::WindowingSystemInterface* wsi =
		osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi)
	{
		std::cout << "[TroenGame::setupForFullScreen] error ..." << std::endl;
		return;
	}
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), m_originalWidth, m_originalHeight);

	//wsi->setScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), 1024, 768);
	this->resize(m_originalWidth, m_originalHeight);
}

void TroenGame::returnFromFullScreen()
{
	osg::GraphicsContext::WindowingSystemInterface* wsi =
		osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi)
	{
		std::cout << "[TroenGame::returnFromFullScreen] error ..." << std::endl;
		return;
	}
	wsi->setScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), m_originalWidth, m_originalHeight);
}



void TroenGame::switchSoundVolumeEvent()
{
	m_audioManager->SetMasterVolume(1 - m_audioManager->GetMasterVolume());
}

void TroenGame::pauseEvent()
{
	if (m_gameLogic->getGameState() == GameLogic::GAMESTATE::GAME_START)
		return;
	else if (!m_gameTimer->paused())
		pauseSimulation();
	else
		unpauseSimulation();
}

void TroenGame::pauseSimulation()
{
	if (!m_gameTimer->paused()) m_gameTimer->pause();
}

void TroenGame::unpauseSimulation()
{
	if (m_gameTimer->paused()) m_gameTimer->start();
}

void TroenGame::resize(int width, int height){
	if (m_postProcessing){
		m_postProcessing->setupTextures(width, height);
	}

	for (auto player : m_playersWithView)
	{
		player->hudController()->resize(width, height);
	}
}


////////////////////////////////////////////////////////////////////////////////
//
// Networking
//
////////////////////////////////////////////////////////////////////////////////

//setup the network connection and return the address of the remote connection
std::string TroenGame::setupNetworking(bool server, std::string connectAddr)
{
	std::cout << "[TroenGame::initialize] networking ..." << std::endl;

	if (server)
	{
		m_ServerManager = std::make_shared<networking::ServerManager>(this);
		m_ServerManager->openServer();
	}
	else
	{
		m_ClientManager = std::make_shared<networking::ClientManager>(this);
		m_ClientManager->openClient(connectAddr);
	}


	// sleep until a valid session is initiated
	while (!getNetworkManager()->isValidSession())
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	if (server)
		return m_ServerManager->getClientAddress();
	else
		return connectAddr;
}


bool TroenGame::synchronizeGameStart(GameConfig config)
{
	getNetworkManager()->synchronizeGameStart(config);
	return true;
}


bool TroenGame::isNetworking()
{
	if (getNetworkManager() != nullptr)
	{
		if (getNetworkManager()->isValidSession())
			return true;
	}
	return false;
}

std::shared_ptr<networking::NetworkManager> TroenGame::getNetworkManager()
{
	if (m_ClientManager != NULL)
		return static_cast<std::shared_ptr<networking::NetworkManager>>(m_ClientManager);
	else if (m_ServerManager != NULL)
		return static_cast<std::shared_ptr<networking::NetworkManager>>(m_ServerManager);
	else
		return NULL;
}
void TroenGame::reloadLevel()
{
	m_levelController->reload();
}
