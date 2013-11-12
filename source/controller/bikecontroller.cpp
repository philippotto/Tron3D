#include "bikecontroller.h"
//troen
#include "../input/bikeinputstate.h"

using namespace troen;

BikeController::BikeController()
{
	m_view = std::make_shared<BikeView>();
}