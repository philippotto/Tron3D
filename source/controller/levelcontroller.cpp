#include "levelcontroller.h"

// OSG
#include <osg/Group>
// troen
#include "../model/LevelModel.h"
#include "../view/LevelView.h"

using namespace troen;

LevelController::LevelController()
{
	m_model = (std::shared_ptr<LevelModel>) std::make_shared<LevelModel>(this);
	m_view = (std::shared_ptr<LevelView>) std::make_shared<LevelView>(std::static_pointer_cast<LevelModel>(m_model));
}