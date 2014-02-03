#include "itemcontroller.h"
#include "bikecontroller.h"

#include "../constants.h"
#include "../model/objectinfo.h"

#include "../model/abstractmodel.h"
#include "../view/abstractview.h"


#include "../model/itemmodel.h"
#include "../view/itemview.h"

using namespace troen;


ItemController::ItemController(btVector3 position, std::weak_ptr<PhysicsWorld> world, LevelView* levelView)
{
	AbstractController();
	m_type = (ItemController::Type) (randf(0, 10) <= 5 ? 0 : 1);
	m_position = position;

	osg::Vec3 viewDimensions = getDimensions();
	btVector3 modelDimensions(viewDimensions.x(), viewDimensions.y(), 10);

	m_model = std::make_shared<ItemModel>(modelDimensions, position, world, this);
	m_view = std::make_shared<ItemView>(getDimensions(), btToOSGVec3(position), levelView);
}


void ItemController::triggerOn(BikeController* bikeController)
{
	if (m_type == HEALTHUP)
	{
		bikeController->increaseHealth(BIKE_DEFAULT_HEALTH / 2);
	}
	else {
		bikeController->activateTurbo();
	}

	remove();
}

void ItemController::remove()
{
	std::static_pointer_cast<ItemModel>(m_model)->remove();
	std::static_pointer_cast<ItemView>(m_view)->remove();
	delete this;
}

osg::Vec3 ItemController::getDimensions()
{
	if (m_type == troen::ItemController::TURBOSTRIP)
		return osg::Vec3(100, 100, 0.1);
	else
		return osg::Vec3(10, 10, 10);
}
