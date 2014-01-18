#include "itemcontroller.h"
#include "bikecontroller.h"

using namespace troen;


ItemController::ItemController()
{
	AbstractController();
}

void ItemController::triggerOn(BikeController* bikeController)
{
	bikeController->activateTurbo();
}