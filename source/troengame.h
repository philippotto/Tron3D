#pragma once
// STD
#include <memory>
// Qt
#include <QThread>
// OSG
#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
// troen
#include "forwarddeclarations.h"

namespace troen
{
	class TroenGame : public QObject
	{
		Q_OBJECT

	public:
		TroenGame(QThread* thread = NULL);
		virtual ~TroenGame();

	public slots:
		void startGameLoop();

	signals:
		void newFrame(double currentTime);

	private:
		bool initialize();
		bool initializeViews();
		bool initializeViewer();
		bool initializeControllers();
		bool initializeInput();
		bool composeSceneGraph();
		bool initializeTimer();
		bool initializePhysicsWorld();
		bool initializeSound();

		bool shutdown();
		
		// TODO
		// implement some kind of menu to start the game from
		//osg::ref_ptr<osgViewer::View>		m_menuView;
		osg::ref_ptr<SampleOSGViewer>		m_sampleOSGViewer;
		osg::ref_ptr<osgViewer::View>		m_gameView;
		osg::ref_ptr<osg::Group>			m_rootNode;

		std::shared_ptr<LevelController>	m_levelController;
		std::shared_ptr<BikeController>		m_bikeController;
		std::shared_ptr<HUDController>		m_HUDController;

		QThread*							m_gameThread;
		std::shared_ptr<util::ChronoTimer>	m_timer;
		std::shared_ptr<PhysicsWorld>		m_physicsWorld;
		std::shared_ptr<sound::AudioManager> m_audioManager;
	};
}