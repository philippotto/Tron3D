#include "abstractcontroller.h"
// STD
#include <memory>
// troen
#include "../view/levelview.h"
#include "../model/levelmodel.h"

using namespace troen;

AbstractController::AbstractController()
{
	
}

osg::ref_ptr<osg::Group> AbstractController::getViewNode()
{
	return m_view->getNode();
}

std::shared_ptr<std::vector<btRigidBody>> AbstractController::getRigidBodies()
{
	return m_model->getRigidBodies();
}