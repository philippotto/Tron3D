#pragma once
// Qt
#include <QThread>
#include <QMetaType>
#include <QColor>
// OSG
#include <osg/ref_ptr>
#include <osg/ShapeDrawable>
#include <osgGA/GUIEventAdapter>
#include <osg/Group>

// troen
#include "forwarddeclarations.h"
#include "resourcepool.h"

#define MAX_BIKES 6
typedef struct s_GameConfig
{
	int numberOfBikes;
	int timeLimit;
	int* playerInputTypes;
	QColor* playerColors;
	QString* playerNames;
	bool splitscreen;
	bool fullscreen;
	bool usePostProcessing;
	bool useDebugView;
	bool testPerformance;
	bool reflection;
	bool ownView[MAX_BIKES];
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

		void resize(const int width,const int height);

		SplineDeformationRendering* getBendedViews() {
			return m_deformationRendering;
		}

		void setDeformationEnd(double end) {
			m_deformationEnd = end;
		}

		double m_deformationEnd = 10000;


	public slots:
		void prepareAndStartGame(const GameConfig& config);

	private:
		bool initialize();
		bool initializeViews();
		bool initializeViewer();
		bool initializeControllers();
		bool initializeInput();
		bool composeSceneGraph();
		bool initializeTimers();
		bool initializeShaders();
		bool initializeGameLogic();
		bool initializePhysicsWorld();
		bool initializeSound();
		bool initializeSkyDome();
		bool initializeLighting();
		bool initializeReflection();

		bool shutdown();
		void startGameLoop();

		void fixCulling(osg::ref_ptr<osgViewer::View>& view);
		// fullscreen handling
		void setupForFullScreen();
		void returnFromFullScreen();
		uint m_originalWidth;
		uint m_originalHeight;

		// OSG Components
		std::vector<osg::ref_ptr<SampleOSGViewer>>	m_viewers;
		std::vector<osg::ref_ptr<osgViewer::View>>	m_gameViews;

		osg::ref_ptr<GameEventHandler>		m_gameEventHandler;
		osg::ref_ptr<osg::Group>			m_rootNode;
		osg::ref_ptr<SkyDome>               m_skyDome;
		osg::ref_ptr<osgViewer::StatsHandler> m_statsHandler;
		std::shared_ptr<PostProcessing>		m_postProcessing;
		osg::ref_ptr<osg::Group>			m_sceneNode;
		osg::ref_ptr<osg::LightSource>		m_sunLightSource;

		// Controllers
		std::shared_ptr<LevelController>	m_levelController;
		std::vector<std::shared_ptr<BikeController>> m_bikeControllers;
		std::vector<std::shared_ptr<HUDController>>		m_HUDControllers;

		std::vector<osg::ref_ptr<osg::Group>> m_playerNodes;

		QThread*							m_gameThread;
		std::shared_ptr<util::ChronoTimer>	m_gameloopTimer;
		std::shared_ptr<util::ChronoTimer>	m_gameTimer;
		std::shared_ptr<PhysicsWorld>		m_physicsWorld;
		std::shared_ptr<GameLogic>			m_gameLogic;
		std::shared_ptr<sound::AudioManager> m_audioManager;

		std::vector<std::shared_ptr<Reflection>>		m_reflections;


		std::vector<int> m_playerInputTypes;
		std::vector<osg::Vec3> m_playerColors;
		std::vector<QString> m_playerNames;

		ResourcePool m_resourcePool;


		// BendedViews
		SplineDeformationRendering* m_deformationRendering;

		// Startup Options
		int m_numberOfBikes;
		int m_timeLimit;
		bool m_splitscreen;
		bool m_fullscreen;
		bool m_usePostProcessing;
		bool m_useDebugView;
		bool m_testPerformance;
		bool m_useReflection;
		bool m_ownView[MAX_BIKES];
	};
}