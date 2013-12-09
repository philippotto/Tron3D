#pragma once
// Qt
#include <QThread>
// OSG
#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osgGA/GUIEventAdapter>
// troen
#include "forwarddeclarations.h"

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
		void removeAllFencesEvent();
		void toggleFencePartsLimitEvent();
		void pauseGameEvent();

		void refreshTextures(const osgGA::GUIEventAdapter&);

	public slots:
		void startGameLoop();

	private:
		bool initialize();
		bool initializeViews();
		bool initializeViewer();
		bool initializeControllers();
		bool initializeInput();
		bool composeSceneGraph();
		bool initializeTimer();
		bool initializeShaders();
		bool initializePhysicsWorld();
		bool initializeSound();
		bool initializeSkyDome();

		bool shutdown();
		
		// TODO
		// implement some kind of menu to start the game from
		//osg::ref_ptr<osgViewer::View>		m_menuView;
		osg::ref_ptr<SampleOSGViewer>		m_sampleOSGViewer;
		osg::ref_ptr<osgViewer::View>		m_gameView;
		osg::ref_ptr<GameEventHandler>		m_gameEventHandler;
		osg::ref_ptr<osg::Group>			m_rootNode;
		osg::ref_ptr<SkyDome>               m_skyDome;
		osg::ref_ptr<osgViewer::StatsHandler> m_statsHandler;
		std::shared_ptr<PostProcessing>		m_postProcessing;
		osg::ref_ptr<osg::Group>			m_sceneNode;

		std::shared_ptr<LevelController>	m_levelController;
		std::shared_ptr<BikeController>		m_bikeController;
		std::shared_ptr<HUDController>		m_HUDController;

		QThread*							m_gameThread;
		std::shared_ptr<util::ChronoTimer>	m_timer;
		std::shared_ptr<PhysicsWorld>		m_physicsWorld;
		std::shared_ptr<sound::AudioManager> m_audioManager;

		int m_maxFenceParts;
		bool m_gamePaused;
	};
}