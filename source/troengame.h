#pragma once

#include <QThread>

#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>

#include <memory>

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

	private:
		bool initialize();
		bool initializeLevel();
		bool initializeViews();
		bool initializeViewer();
		bool initializeModels();
		bool initializeInput();
		bool composeSceneGraph();
		bool initializeTimer();
		bool initializeGameLogic();

		bool shutdown();

		osg::ref_ptr<SampleOSGViewer>       m_sampleOSGViewer;
		osg::ref_ptr<osgViewer::View>		m_gameView;
		// TODO
		// implement some kind of menu to start the game from
		//osg::ref_ptr<osgViewer::View>		m_menuView;

		osg::ref_ptr<osg::Group>            m_rootNode;
		osg::ref_ptr<osg::PositionAttitudeTransform> m_childNode;

		std::shared_ptr<LevelController> m_levelController;

		QThread*	m_gameThread;

		std::shared_ptr<util::ChronoTimer>	m_timer;

		std::shared_ptr<GameLogic> m_gameLogic;
	};
}