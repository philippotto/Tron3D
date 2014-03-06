#include "gameeventhandler.h"
// troen
#include "troengame.h"
#include "gamelogic.h"
#include "view/shaders.h"

using namespace troen;

GameEventHandler::GameEventHandler(TroenGame * game) :
    osgGA::GUIEventHandler(),
    m_troenGame(game)
{}

void GameEventHandler::attachGameLogic(std::shared_ptr<GameLogic>& gamelogic)
{
    m_gameLogic = gamelogic;
}

 bool GameEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*) {
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
        case osgGA::GUIEventAdapter::KEY_Delete:
            m_gameLogic.lock()->restartLevel();
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