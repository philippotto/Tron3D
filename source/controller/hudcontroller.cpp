#include "hudcontroller.h"
// std
#include <memory>
// troen
#include "../view/hudview.h"
//#include "../model/hudmodel.h"

using namespace troen;

HUDController::HUDController()
{
	m_view = std::static_pointer_cast<HUDView>(std::make_shared<HUDView>());

	// TODO: is a model really necessary?
	// m_model = std::static_pointer_cast<HUDModel>(std::make_shared<HUDModel>());


}