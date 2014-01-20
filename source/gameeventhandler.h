// OSG
#include <osgViewer/ViewerEventHandlers>
// troen
#include "troengame.h"
#include "gamelogic.h"
#include "view/shaders.h"

namespace troen
{
	class GameEventHandler : public osgGA::GUIEventHandler {
	public:
		GameEventHandler(TroenGame* game, std::shared_ptr<GameLogic> gameLogic) :
			osgGA::GUIEventHandler(),
			m_troenGame(game),
			m_gameLogic(gameLogic) {}

	protected:
		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*) {
			switch (ea.getEventType()) {
			case osgGA::GUIEventAdapter::KEYDOWN:
				switch (ea.getKey()) {
				case osgGA::GUIEventAdapter::KEY_M:
					m_troenGame->switchSoundVolumeEvent();
					return true;
				case osgGA::GUIEventAdapter::KEY_C:
					m_gameLogic.lock()->removeAllFences();
					return true;
				case osgGA::GUIEventAdapter::KEY_F:
					m_gameLogic.lock()->toggleFencePartsLimit();
					return true;
				case osgGA::GUIEventAdapter::KEY_R:
				{
					std::cout << "Reloading shaders" << std::endl;
					shaders::reloadShaders();
					return true;
				}
				case osgGA::GUIEventAdapter::KEY_Shift_R:
                case osgGA::GUIEventAdapter::KEY_Shift_L:
					m_troenGame->pauseEvent();
					return true;
				default:
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
			if (ea.getEventType() == osgGA::GUIEventAdapter::RESIZE)
			{
				// re setup textures to new size
				m_troenGame->resize(ea.getWindowWidth(), ea.getWindowHeight());
			}

			return false;
		}
	private:
		TroenGame*					m_troenGame;
		std::weak_ptr<GameLogic>	m_gameLogic;
	};
}