#include "levelcontroller.h"

// OSG
#include <osg/Group>
// troen
#include "../model/LevelModel.h"
#include "../view/LevelView.h"

using namespace troen;

LevelController::LevelController()
{
	m_view = std::make_shared<LevelView>();
	m_model = std::make_shared<LevelModel>();
}


osg::ref_ptr<osg::Group> LevelController::getViewNode() {
	return m_view->getNode();
}