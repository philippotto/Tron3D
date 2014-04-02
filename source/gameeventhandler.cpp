#include "gameeventhandler.h"
// troen
#include "troengame.h"
#include "gamelogic.h"
#include "view/shaders.h"
#include "BendedViews/src/SplineDeformationRendering.h"

using namespace troen;

GameEventHandler::GameEventHandler(TroenGame * game) :
    osgGA::GUIEventHandler(),
    m_troenGame(game)
{}

void GameEventHandler::attachGameLogic(std::shared_ptr<GameLogic>& gamelogic)
{
    m_gameLogic = gamelogic;
}

 bool GameEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*)
 {
	 if (ea.getEventType() == osgGA::GUIEventAdapter::RESIZE)
	 {
		 // re setup textures to new size
		 m_troenGame->resize(ea.getWindowWidth(), ea.getWindowHeight());
		 return false;
	 }


	 if (ea.getEventType() != osgGA::GUIEventAdapter::KEYDOWN) {
		 return false;
	 }
    
	 switch (ea.getKey()) {
	 case osgGA::GUIEventAdapter::KEY_M:
		 m_troenGame->switchSoundVolumeEvent();
		 break;
	 case osgGA::GUIEventAdapter::KEY_C:
		 m_gameLogic.lock()->removeAllFences();
		 break;
	 case osgGA::GUIEventAdapter::KEY_F:
		 m_gameLogic.lock()->toggleFencePartsLimit();
		 break;
	 case osgGA::GUIEventAdapter::KEY_L:
		 m_troenGame->reloadLevel();
		 break;
	 case osgGA::GUIEventAdapter::KEY_R:
		std::cout << "Reloading shaders" << std::endl;
		shaders::reloadShaders();
		m_troenGame->getBendedViews()->reloadShaders();
		break;
	 case osgGA::GUIEventAdapter::KEY_Shift_R:
	 case osgGA::GUIEventAdapter::KEY_Shift_L:
		 m_troenGame->pauseEvent();
		 break;
	 case osgGA::GUIEventAdapter::KEY_Delete:
		 m_gameLogic.lock()->restartLevel();
		 break;
	 case osgGA::GUIEventAdapter::KEY_1:	 
		m_troenGame->getBendedViews()->setPreset(0);
		break;
	 case osgGA::GUIEventAdapter::KEY_2:
		m_troenGame->getBendedViews()->setPreset(1);
		break;
	 case osgGA::GUIEventAdapter::KEY_3: 
		m_troenGame->getBendedViews()->setPreset(2);
		break;
	 case osgGA::GUIEventAdapter::KEY_4: 
		m_troenGame->getBendedViews()->setPreset(3);
		break;
	 case osgGA::GUIEventAdapter::KEY_Tab:
		m_troenGame->toggleHUDVisibility();
		break;	 
	 default: 
	  return false; 
	 }

    return true;
}