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

#include "../troengame.h"


using namespace troen;


ItemController::ItemController(btVector3 position, std::weak_ptr<PhysicsWorld> world, TroenGame* troenGame, LevelView* levelView)
{
	AbstractController();

	m_type = (ItemController::Type) (int) floor(randf(0, COUNT));
	m_position = position;
	m_troenGame = troenGame;

	osg::Vec3 viewDimensions = getDimensions();
	btVector3 modelDimensions(viewDimensions.x(), viewDimensions.y(), 10);
	position.setZ(position.z() + viewDimensions.z() / 2.f);

	m_model = m_itemModel = std::make_shared<ItemModel>(modelDimensions, position, world, this);
	m_view = m_itemView = std::make_shared<ItemView>(getDimensions(), btToOSGVec3(position), levelView, m_type);
}


void ItemController::triggerOn(BikeController* bikeController, GameLogic* gamelogic/*= nullptr*/)
{

	switch (m_type)
	{
	case troen::ItemController::TURBOSTRIP:
		bikeController->activateTurbo();
		remove();
		break;
	case troen::ItemController::HEALTHUP:
		bikeController->player()->increaseHealth(BIKE_DEFAULT_HEALTH / 2);
		remove();
		break;
	case troen::ItemController::RADAR:
		if (!gamelogic) return;

		m_id = bikeController->player()->id();
		gamelogic->showFencesInRadarForPlayer(m_id);
		m_gamelogic = gamelogic;
		QTimer::singleShot(5000, this, SLOT(hideFencesInRadarForPlayer()));
		remove();
		return;
		break;
	case troen::ItemController::BENDEDVIEWS:
		m_troenGame->enableBendedViews();
		QTimer::singleShot(TIME_TO_ACTIVATE_BENDED_VIEWS * 3, this, SLOT(deactivateBendedViews()));
		remove();
		return;
		break;
	default:
		remove();
		break;
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
	return osg::Vec3(8, 8, 8);
}

void ItemController::hideFencesInRadarForPlayer()
{
	m_gamelogic->hideFencesInRadarForPlayer(m_id);
	destroy();
}

void ItemController::deactivateBendedViews()
{
	m_troenGame->disableBendedViews();
	destroy();
}
