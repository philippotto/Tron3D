#pragma once

#include <QThread>

#include <osg/ref_ptr>

#include "forwarddeclarations.h"


namespace troen
{
	class TroenGame : public QObject
	{
		Q_OBJECT

	public:
		TroenGame(osg::GraphicsContext* context, QThread* thread = NULL);
		virtual ~TroenGame();

	private:
		bool initialize();
		bool initializeViewer();
		bool initializeModels();
		bool composeSceneGraph();

		osg::ref_ptr<osg::GraphicsContext>	m_graphicsContext;
		osg::ref_ptr<SampleOSGViewer>       m_sampleOSGViewer;
		osg::ref_ptr<osg::Group>            m_rootNode;
		osg::ref_ptr<osg::Node>				m_childNode;

		QThread*	m_gameThread;

	};


}