#include "bikecontroller.h"

#include <math.h>

#include "../input/bikeinputstate.h"


using namespace troen;


BikeController::BikeController()
{
	m_view = std::make_shared<BikeView>();
}