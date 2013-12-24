#include "fencecontroller.h"
// troen
#include "../constants.h"
#include "../view/fenceview.h"
#include "../model/fencemodel.h"
#include "../model/physicsworld.h"

#include <osg/Vec3>

using namespace troen;

FenceController::FenceController(osg::Vec3 color, btTransform initialTransform, int maxFenceParts) : m_maxFenceParts(maxFenceParts)
{
	m_playerColor = color;
	m_model = std::make_shared<FenceModel>(this, m_maxFenceParts);
    m_view = std::shared_ptr<FenceView>(new FenceView(m_playerColor, m_model,m_maxFenceParts));

	btQuaternion rotation = initialTransform.getRotation();
	btVector3 position = initialTransform.getOrigin();
	adjustPositionUsingFenceOffset(rotation, position);

	m_lastPosition = position;
}

void FenceController::update(btVector3 position, btQuaternion rotation)
{
	adjustPositionUsingFenceOffset(rotation, position);
	osg::Vec3 osgPosition = osg::Vec3(position.x(), position.y(), position.z());
	osg::Vec3 osgLastPosition = osg::Vec3(m_lastPosition.x(), m_lastPosition.y(), m_lastPosition.z());
	// add new fence part
	if ((position - m_lastPosition).length() > FENCE_PART_LENGTH)
	{
		std::static_pointer_cast<FenceModel>(m_model)->addFencePart(m_lastPosition, position);
		std::static_pointer_cast<FenceView>(m_view)->addFencePart(osgLastPosition,osgPosition);
		m_lastPosition = position;
	}

	// update fence gap
	std::static_pointer_cast<FenceView>(m_view)->updateFenceGap(osgLastPosition, osgPosition);
}


void FenceController::attachWorld(std::shared_ptr<PhysicsWorld> &world)
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

void FenceController::adjustPositionUsingFenceOffset(const btQuaternion& rotation, btVector3& position)
{
	btVector3 fenceOffset = btVector3(
		0,
		-BIKE_DIMENSIONS.y() / 2,
		BIKE_DIMENSIONS.z() / 2).rotate(rotation.getAxis(), rotation.getAngle()
		);

	position = position - fenceOffset;
}
