#pragma once
// Qt
#include <QThread>
#include <QMetaType>
#include <QColor>
// OSG
#include <osg/ref_ptr>
#include <osg/ShapeDrawable>
#include <osgGA/GUIEventAdapter>
// troen
#include "forwarddeclarations.h"

#include "resourcepool.h"

#include "OVR.h"

typedef struct s_GameConfig
{
	int numberOfBikes;
	int* playerInputTypes;
	QColor* playerColors;
	bool splitscreen;
	bool fullscreen;
	bool usePostProcessing;
	bool useDebugView;
	bool testPerformance;
} GameConfig;
Q_DECLARE_METATYPE(s_GameConfig)

namespace troen
{
	class TroenGame : public QObject
	{
		Q_OBJECT

	public:
		TroenGame(QThread* thread = nullptr);
		virtual ~TroenGame();
		
		// Events from GameEventHandler
		void switchSoundVolumeEvent();
		void pauseEvent();

		// Logic Events
		void pauseSimulation();
		void unpauseSimulation();

		void resize(int width, int height);
		void setFovy(float newFovy);
		float getFovy();

	public slots:
		void prepareAndStartGame(GameConfig config);

	private:
		bool initialize();

		void initializeOculus();

		bool initializeViews();
		bool initializeViewer();
		bool initializeHud();
		bool initializeControllers();
		bool initializeInput();
		bool composeSceneGraph();
		bool initializeTimer();
		bool initializeShaders();
		bool initializeGameLogic();
		bool initializePhysicsWorld();
		bool initializeSound();
		bool initializeSkyDome();
		bool initializeLighting();

		bool shutdown();
		void startGameLoop();

		void getOcculusOrientation();

		// TODO
		// implement some kind of menu to start the game from
		//osg::ref_ptr<osgViewer::View>		m_menuView;

		// OSG Components
		osg::ref_ptr<SampleOSGViewer>		m_sampleOSGViewer;
		osg::ref_ptr<SampleOSGViewer>		m_sampleOSGViewer2;
		osg::ref_ptr<osgViewer::View>		m_gameView;
		osg::ref_ptr<osgViewer::View>		m_gameView2;

		osg::ref_ptr<GameEventHandler>		m_gameEventHandler;
		osg::ref_ptr<osg::Group>			m_rootNode;
		osg::ref_ptr<SkyDome>               m_skyDome;
		osg::ref_ptr<osgViewer::StatsHandler> m_statsHandler;
		std::shared_ptr<PostProcessing>		m_postProcessing;
		osg::ref_ptr<osg::Group>			m_sceneNode;
		osg::ref_ptr<osg::Switch>			m_hudSwitch;
		osg::ref_ptr<osg::LightSource>		m_sunLightSource;


		// Controllers
		std::shared_ptr<LevelController>	m_levelController;
		std::vector<std::shared_ptr<BikeController>> m_bikeControllers;
		std::shared_ptr<HUDController>		m_HUDController;

		QThread*							m_gameThread;
		std::shared_ptr<util::ChronoTimer>	m_timer;
		std::shared_ptr<PhysicsWorld>		m_physicsWorld;
		std::shared_ptr<GameLogic>			m_gameLogic;
		std::shared_ptr<sound::AudioManager> m_audioManager;

		std::vector<int> m_playerInputTypes;
		std::vector<osg::Vec3> m_playerColors;

		ResourcePool m_resourcePool;

		bool m_simulationPaused;

		// Startup Options
		int m_numberOfBikes;
		bool m_splitscreen;
		bool m_fullscreen;
		bool m_usePostProcessing;
		bool m_useDebugView;
		bool m_testPerformance;



		OVR::SensorFusion *m_SFusion;
		OVR::SensorDevice *m_pSensor;
	};
}