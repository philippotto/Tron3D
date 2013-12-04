// OSG
#include <osgViewer/ViewerEventHandlers>
// troen
#include "TroenGame.h"

namespace troen
{
	class GameEventHandler : public osgGA::GUIEventHandler {
	public:
		GameEventHandler(TroenGame* game) : osgGA::GUIEventHandler(), m_troenGame(game) {}
	protected:
		GameEventHandler() :
			m_troenGame(NULL) {
		}
		virtual ~GameEventHandler() {}

		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*) {
			switch (ea.getEventType()) {
			case osgGA::GUIEventAdapter::KEYDOWN:
				switch (ea.getKey()) {
				case osgGA::GUIEventAdapter::KEY_M:
					m_troenGame->switchSoundVolumeEvent();
					return true;
					break;
				case osgGA::GUIEventAdapter::KEY_C:
					m_troenGame->removeAllFencesEvent();
					return true;
					break;
				case osgGA::GUIEventAdapter::KEY_F:
					m_troenGame->toggleFencePartsLimitEvent();
					return true;
					break;
				}
			//case osgGA::GUIEventAdapter::KEYUP:
			//	switch (ea.getKey()) {
			//	case osgGA::GUIEventAdapter::KEY_H: // toogle Hud Visibility
			//		m_troenGame->toggleHudVisibility();
			//		return true;
			//	}
			//	break;
			}
			return false;
		}
	private:
		TroenGame*   m_troenGame;
	};
}