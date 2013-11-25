#include "fencecontroller.h"
// STD
#include <memory>
#include <iostream>
// troen
#include "../view/fenceview.h"
#include "../model/fencemodel.h"
#include "../model/physicsworld.h"

using namespace troen;

FenceController::FenceController()
{
	m_view = std::static_pointer_cast<FenceView>(std::make_shared<FenceView>());
	m_model = std::static_pointer_cast<FenceModel>(std::make_shared<FenceModel>());
}

void FenceController::update(btVector3 position)
{
	
	// this determines how accurate the fence will be
	const float fenceLength = 50;

	if (!m_lastPosition)
	{
		m_lastPosition = position;
		return;
	}

	if ((position - m_lastPosition).length() > fenceLength)
	{

		std::shared_ptr<FenceModel> fenceModel = (std::static_pointer_cast<FenceModel>(m_model));

		fenceModel->addFencePart(m_lastPosition, position);
		m_world->addRigidBody(fenceModel->getLastPart());

		std::static_pointer_cast<FenceView>(m_view)->addFencePart(
			osg::Vec3(m_lastPosition.x(), m_lastPosition.y(), m_lastPosition.z()),
			osg::Vec3(position.x(), position.y(), position.z())
		);

		m_lastPosition = position;
	}
}


void FenceController::attachWorld(std::shared_ptr<PhysicsWorld> world) {
	m_world = world;
}