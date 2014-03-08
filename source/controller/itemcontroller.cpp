#include "itemcontroller.h"
//qt
#include <QTimer>
//troen
#include "../gamelogic.h"
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


void ItemController::triggerOn(BikeController* bikeController, GameLogic* gamelogic/*= nullptr*/)
{
	if (m_type == HEALTHUP)
	{
		bikeController->player()->increaseHealth(BIKE_DEFAULT_HEALTH / 2);
		remove();

	}
	else if (m_type == RADAR)
	{
		if (!gamelogic) return;

		m_id = bikeController->player()->id();
		gamelogic->showFencesInRadarForPlayer(m_id);
		m_gamelogic = gamelogic;
		QTimer::singleShot(5000, this, SLOT(hideFencesInRadarForPlayer()));
		remove();
		return;
	}
	else {
		bikeController->activateTurbo();
		remove();
	}

	destroy();
}

void ItemController::remove()
{
	m_itemModel->remove();
	m_itemView->remove();

	m_model.reset();
	m_view.reset();
}

void ItemController::destroy()
{
	delete this;
}


osg::Vec3 ItemController::getDimensions()
{
	if (m_type == troen::ItemController::TURBOSTRIP)
		return osg::Vec3(100, 100, 0.1);
	else
		return osg::Vec3(8, 8, 8);
}

void ItemController::hideFencesInRadarForPlayer()
{
	m_gamelogic->hideFencesInRadarForPlayer(m_id);
	destroy();
}