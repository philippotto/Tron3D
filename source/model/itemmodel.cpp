#include "itemmodel.h"
// troen
#include "../constants.h"
#include "physicsworld.h"
#include "objectinfo.h"
#include "../controller/fencecontroller.h"

using namespace troen;

ItemModel::ItemModel(btVector3 dimensions, btVector3 position, std::weak_ptr<PhysicsWorld> world, ItemController* itemController)
{
	AbstractModel();
	
	m_world = world;

	// create a trigger volume
	m_pTrigger = new btCollisionObject();

	ObjectInfo* info = new ObjectInfo(itemController, ITEMTYPE);
	m_pTrigger->setUserPointer(info);

	// create a box for the trigger's shape
	m_pTrigger->setCollisionShape(new btBoxShape(dimensions));
	// set the trigger's position
	btTransform triggerTrans;
	triggerTrans.setIdentity();
	triggerTrans.setOrigin(position);
	m_pTrigger->setWorldTransform(triggerTrans);
	// flag the trigger to ignore contact responses
	m_pTrigger->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	
	world.lock()->addCollisionObject(m_pTrigger);
}

void ItemModel::remove()
{
	m_world.lock()->removeCollisionObject(m_pTrigger);
}
