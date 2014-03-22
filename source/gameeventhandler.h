#pragma once
// OSG
#include <osgViewer/ViewerEventHandlers>
// troen
#include "forwarddeclarations.h"

namespace troen
{
	class GameEventHandler : public osgGA::GUIEventHandler
	{
	public:
		GameEventHandler(TroenGame * game);
		void attachGameLogic(std::shared_ptr<GameLogic>& gamelogic);

	protected:

		virtual bool handle(const osgGA::GUIEventAdapter& ea,
                            osgGA::GUIActionAdapter& aa,
                            osg::Object*,
                            osg::NodeVisitor*);
	private:
		TroenGame*					m_troenGame;
		std::weak_ptr<GameLogic>	m_gameLogic;
	};
}