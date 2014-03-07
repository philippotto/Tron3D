#include "itemcontroller.h"
//troen
#include "../player.h"
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

	m_type = (ItemController::Type) (int) floor(randf(0, COUNT));
	m_position = position;

	osg::Vec3 viewDimensions = getDimensions();
	btVector3 modelDimensions(viewDimensions.x(), viewDimensions.y(), 10);
	position.setZ(position.z() + viewDimensions.z() / 2.f);

	m_model = m_itemModel = std::make_shared<ItemModel>(modelDimensions, position, world, this);
	m_view = m_itemView = std::make_shared<ItemView>(getDimensions(), btToOSGVec3(position), levelView, m_type);
}


void ItemController::triggerOn(BikeController* bikeController)
{
	if (m_type == HEALTHUP)
	{
		bikeController->player()->increaseHealth(BIKE_DEFAULT_HEALTH / 2);
	}
	else if (m_type == RADAR)
	{
		int id = bikeController->player()->id();
		bikeController->player()->fenceController()->showFencesInRadarForPlayer(id);
	}
	else {
		bikeController->activateTurbo();
	}

	remove();
}

void ItemController::remove()
{
	m_itemModel->remove();
	m_itemView->remove();

	m_model.reset();
	m_view.reset();

	delete this;
}

osg::Vec3 ItemController::getDimensions()
{
	if (m_type == troen::ItemController::TURBOSTRIP)
		return osg::Vec3(100, 100, 0.1);
	else
		return osg::Vec3(8, 8, 8);
}
