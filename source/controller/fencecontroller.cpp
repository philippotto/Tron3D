#include "fencecontroller.h"
// troen
#include "../constants.h"
#include "bikecontroller.h"
#include "../view/fenceview.h"
#include "../model/fencemodel.h"
#include "../model/physicsworld.h"

#include <osg/Vec3>

using namespace troen;

FenceController::FenceController(
	Player * player,
	btTransform initialTransform) :
AbstractController(),
m_fenceLimitActivated(true)
{
	m_player = player;
	m_model = m_fenceModel = std::make_shared<FenceModel>(this);
	m_view = m_fenceView = std::shared_ptr<FenceView>(new FenceView(this, player->color(), m_model));

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
		m_fenceModel->addFencePart(m_lastPosition, position);
		m_fenceView->addFencePart(osgLastPosition,osgPosition);
		m_lastPosition = position;
	}

	// update fence gap
	m_fenceView->updateFenceGap(osgLastPosition, osgPosition);
}


void FenceController::attachWorld(std::shared_ptr<PhysicsWorld> &world)
{
	m_world = world;
	m_fenceModel->attachWorld(world);
}

void FenceController::removeAllFences()
{
	m_fenceModel->removeAllFences();
	m_fenceView->removeAllFences();
}

void FenceController::removeAllFencesFromModel()
{
	m_fenceModel->removeAllFences();
}

void FenceController::setLimitFence(bool boolean)
{
	m_fenceLimitActivated = boolean;
}

int FenceController::getFenceLimit() {
	if (m_fenceLimitActivated)
		return m_player->points();
	else
		return 0;
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

void FenceController::setLastPosition(const btQuaternion rotation, btVector3 position)
{
	adjustPositionUsingFenceOffset(rotation, position);
	m_lastPosition = position;
}


void FenceController::showFencesInRadarForPlayer(const int id)
{
	m_fenceView->showFencesInRadarForPlayer(id);
}

void FenceController::hideFencesInRadarForPlayer(const int id)
{
	m_fenceView->hideFencesInRadarForPlayer(id);
}

osg::ref_ptr<osg::Group> FenceController::getViewNode()
{
	osg::ref_ptr<osg::Group> group = m_fenceView->getNode();
	// TODO (dw) try not to disable culling, by resizing the childrens bounding boxes
	// group->setCullingActive(false);
	return group;
}

void FenceController::updateFadeOutFactor(float fadeOutFactor)
{
	m_fenceView->updateFadeOutFactor(fadeOutFactor);
}