#include "fencecontroller.h"

#include "../view/fenceview.h"
#include "../model/fencemodel.h"

using namespace troen;

FenceController::FenceController()
{
	m_model = std::static_pointer_cast<FenceModel>(std::make_shared<FenceModel>());
	m_view = std::static_pointer_cast<FenceView>(std::make_shared<FenceView>());	
}
