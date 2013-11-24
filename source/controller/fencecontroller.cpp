#include "fencecontroller.h"
// STD
#include <memory>
// troen
#include "../view/fenceview.h"
#include "../model/fencemodel.h"
#include "../model/physicsworld.h"

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
	
	const float fenceLength = 50;

	if (m_lastPosition)
	{
		if ((position - m_lastPosition).length() > fenceLength)
		{

			std::shared_ptr<FenceModel> fenceModel = (std::static_pointer_cast<FenceModel>(m_model));

			fenceModel->addFencePart(m_lastPosition, position);
			m_world->addRigidBody(fenceModel->getLastPart());
		}
	}
	m_lastPosition = position;
	
	// TODO
	// std::static_pointer_cast<FenceView>(m_view)->updateFence();
}


void FenceController::attachWorld(std::shared_ptr<PhysicsWorld> world) {
	m_world = world;
}