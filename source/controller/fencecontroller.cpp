#include "fencecontroller.h"
// STD
#include <memory>
// troen
#include "../view/fenceview.h"
#include "../model/fencemodel.h"

using namespace troen;

FenceController::FenceController()
{
	m_view = std::static_pointer_cast<FenceView>(std::make_shared<FenceView>());
	m_model = std::static_pointer_cast<FenceModel>(std::make_shared<FenceModel>());

	// TODO
	// just for testing, remove this once the fence is drawn via the update method
	std::static_pointer_cast<FenceView>(m_view)->updateFence();
}

void FenceController::update(btVector3 position)
{
	//std::static_pointer_cast<FenceModel>(m_model)->addFence(position);
	std::static_pointer_cast<FenceView>(m_view)->updateFence();
}
