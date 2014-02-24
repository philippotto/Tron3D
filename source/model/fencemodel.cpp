#include "fencemodel.h"
// troen
#include "../constants.h"
#include "physicsworld.h"
#include "objectinfo.h"
#include "../controller/fencecontroller.h"

using namespace troen;

FenceModel::FenceModel(FenceController* fenceController)
{
	AbstractModel();
	m_fenceController = fenceController;
	m_rigidBodyDeque = std::deque<std::shared_ptr<btRigidBody>>();
}

void FenceModel::attachWorld(std::shared_ptr<PhysicsWorld>& world)
{
	m_world = world;
}

void FenceModel::addFencePart(btVector3 a, btVector3 b)
{
	btVector3 fenceVector = b - a;

	const btVector3 yAxis = btVector3(0, 1, 0);
	btScalar angle = fenceVector.angle(-yAxis);
	const btScalar inverseAngle = fenceVector.angle(yAxis);

	btQuaternion rotationQuatXY;
	btVector3 axis;
	if (angle != 0 && inverseAngle != 0) {
		// we need to make sure the angle is lower than PI_2
		if (angle < PI_2)
			axis = fenceVector.cross(yAxis).normalized();
		else {
			angle = fenceVector.angle(yAxis);
			axis = fenceVector.cross(-yAxis).normalized();
		}
		rotationQuatXY = btQuaternion(axis, angle);
	}
	else {
		rotationQuatXY = btQuaternion(0, 0, 0, 1);
	}

	std::shared_ptr<btBoxShape> fenceShape = std::make_shared<btBoxShape>(btVector3(FENCE_PART_WIDTH / 2, fenceVector.length() / 2, FENCE_HEIGHT_MODEL / 2));
	std::shared_ptr<btDefaultMotionState> fenceMotionState = std::make_shared<btDefaultMotionState>(btTransform(rotationQuatXY, (a + b) / 2 + btVector3(0, 0, FENCE_HEIGHT_MODEL / 2)));

	const btScalar mass = 0;
	const btVector3 fenceInertia(0, 0, 0);

	btRigidBody::btRigidBodyConstructionInfo m_fenceRigidBodyCI(mass, fenceMotionState.get(), fenceShape.get(), fenceInertia);

	std::shared_ptr<btRigidBody> fenceRigidBody = std::make_shared<btRigidBody>(m_fenceRigidBodyCI);

	ObjectInfo* info = new ObjectInfo(m_fenceController, FENCETYPE);
	fenceRigidBody->setUserPointer(info);

	m_collisionShapeDeque.push_back(fenceShape);
	m_motionStateDeque.push_back(fenceMotionState);
	m_rigidBodyDeque.push_back(fenceRigidBody);

	m_world.lock()->addRigidBody(fenceRigidBody.get(),COLGROUP_FENCE, COLMASK_FENCE);

	enforceFencePartsLimit();
}

void FenceModel::removeAllFences()
{
	for (auto rigidBody : m_rigidBodyDeque)
		m_world.lock()->removeRigidBody(rigidBody.get());
	m_rigidBodyDeque.clear();
	m_motionStateDeque.clear();
	m_collisionShapeDeque.clear();
}

void FenceModel::enforceFencePartsLimit()
{
	int maxFenceParts = m_fenceController->getFenceLimit();

	size_t rigidBodyDequeSize = m_rigidBodyDeque.size();
	if (maxFenceParts != 0 && rigidBodyDequeSize > maxFenceParts)
	{
		for (int i = 0; i < rigidBodyDequeSize - maxFenceParts; i++)
			removeFirstFencePart();
	}
}

void FenceModel::removeFirstFencePart()
{
	m_world.lock()->removeRigidBody(m_rigidBodyDeque.front().get());
	m_rigidBodyDeque.pop_front();
	m_motionStateDeque.pop_front();
	m_collisionShapeDeque.pop_front();
}