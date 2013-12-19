#include "fencecontroller.h"
// troen
#include "../view/fenceview.h"
#include "../model/fencemodel.h"
#include "../model/physicsworld.h"

#include <osg/Vec3>

using namespace troen;

FenceController::FenceController(osg::Vec3 color, int maxFenceParts) : m_maxFenceParts(maxFenceParts)
{
	m_playerColor = color;
	m_model = std::make_shared<FenceModel>(this, m_maxFenceParts);
	m_view = std::make_shared<FenceView>(m_playerColor, std::dynamic_pointer_cast<FenceModel>(m_model), m_maxFenceParts);
}

void FenceController::update(btVector3 position)
{
	// this determines how accurate the fence will be
	const float fenceLength = 15;

	if (!m_lastPosition)
	{
		m_lastPosition = position;
		return;
	}

	osg::Vec3 osgPosition = osg::Vec3(position.x(), position.y(), position.z());
	osg::Vec3 osgLastPosition = osg::Vec3(m_lastPosition.x(), m_lastPosition.y(), m_lastPosition.z());
	// add new fence part
	if ((position - m_lastPosition).length() > fenceLength)
	{
		std::static_pointer_cast<FenceModel>(m_model)->addFencePart(m_lastPosition, position);
		std::static_pointer_cast<FenceView>(m_view)->addFencePart(osgLastPosition,osgPosition);
		m_lastPosition = position;
	}

	// update fence gap
	std::static_pointer_cast<FenceView>(m_view)->updateFenceGap(osgLastPosition, osgPosition);

}


void FenceController::attachWorld(std::weak_ptr<PhysicsWorld> &world)
{
	m_world = world;
	std::static_pointer_cast<FenceModel>(m_model)->attachWorld(world);
}

void FenceController::removeAllFences()
{
	std::static_pointer_cast<FenceModel>(m_model)->removeAllFences();
	std::static_pointer_cast<FenceView>(m_view)->removeAllFences();
}

void FenceController::enforceFencePartsLimit(int maxFenceParts)
{
	if (m_maxFenceParts == maxFenceParts) return;

	m_maxFenceParts = maxFenceParts;
	std::static_pointer_cast<FenceModel>(m_model)->enforceFencePartsLimit(maxFenceParts);
	std::static_pointer_cast<FenceView>(m_view)->enforceFencePartsLimit(maxFenceParts);


}
