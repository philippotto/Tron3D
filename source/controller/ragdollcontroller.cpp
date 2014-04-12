#include "ragdollcontroller.h"
// OSG
#include <osg/Group>
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../constants.h"
#include "../model/physicsworld.h"
#include "../model/ragdollmodel.h"
#include "../view/ragdollview.h"


#include "../troengame.h"

using namespace troen;

RagdollController::RagdollController(Player *player, btTransform startTransform) 
{
	m_startPosition = startTransform.getOrigin();
	m_player = player;

	osg::Vec3 dimensions = osg::Vec3(5.0, 5.0, 5.0);
	osg::Vec3 startPosition_osg = osg::Vec3(0.0, 0.0, 5.0);// osg::Vec3(m_startPosition.x(), m_startPosition.y(), m_startPosition.z());
	m_view = m_ragdollView = std::make_shared<RagdollView>(dimensions, startPosition_osg);
}


void RagdollController::attachWorld(std::shared_ptr<PhysicsWorld> &world)
{
	m_world = world;
	m_model = std::make_shared<RagdollModel>(m_world.lock()->getDiscreteWorld(), this, m_startPosition);
}



void RagdollController::updateView(const btTransform &worldTrans)
{

}