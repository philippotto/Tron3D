#include "fencemodel.h"
// troen
#include "../model/physicsworld.h"

using namespace troen;

FenceModel::FenceModel(FenceController* fenceController, int maxFenceParts) : m_maxFenceParts(maxFenceParts)
{
	m_fenceController = fenceController;
	m_rigidBodyDeque = std::deque<std::shared_ptr<btRigidBody>>();
}

void FenceModel::attachWorld(std::weak_ptr<PhysicsWorld>& world)
{
	m_world = world;
}

void FenceModel::addFencePart(btVector3 a, btVector3 b)
{
	btVector3 fenceVector = b - a;
	
	std::shared_ptr<btBoxShape> fenceShape = std::make_shared<btBoxShape>(btVector3(1, fenceVector.length() / 2, getFenceHeight() / 2));
	
	const btVector3 up = btVector3(0, 0, 1);
	const btVector3 forward = btVector3(0, 1, 0);
	const btScalar angle = fenceVector.angle(forward);
	const btScalar inverseAngle = fenceVector.angle(-1 * forward);

	btQuaternion rotationQuat;
	if (angle != 0 && inverseAngle != 0) {
		btVector3 axis = fenceVector.cross(-forward).normalized();
		rotationQuat = btQuaternion(axis, angle);
	}
	else {
		rotationQuat = btQuaternion(0, 0, 0, 1);
	}

	std::shared_ptr<btDefaultMotionState> fenceMotionState = std::make_shared<btDefaultMotionState>(btTransform(rotationQuat, (a + b) / 2 + btVector3(0, 0, getFenceHeight() / 2)));

	const btScalar mass = 0;
	const btVector3 fenceInertia(0, 0, 0);
	
	btRigidBody::btRigidBodyConstructionInfo m_fenceRigidBodyCI(mass, fenceMotionState.get(), fenceShape.get(), fenceInertia);

	std::shared_ptr<btRigidBody> fenceRigidBody = std::make_shared<btRigidBody>(m_fenceRigidBodyCI);
	fenceRigidBody->setUserPointer(m_fenceController);
	fenceRigidBody->setUserIndex(FENCETYPE);

	m_collisionShapeDeque.push_back(fenceShape);
	m_motionStateDeque.push_back(fenceMotionState);
	m_rigidBodyDeque.push_back(fenceRigidBody);

	m_world.lock()->addRigidBody(fenceRigidBody.get());

	enforceFencePartsLimit(m_maxFenceParts);
}

float FenceModel::getFenceHeight()
{
	return 15;
}

void FenceModel::removeAllFences()
{
	for (auto rigidBody : m_rigidBodyDeque)
		m_world.lock()->removeRigidBody(rigidBody.get());
	m_rigidBodyDeque.clear();
	m_motionStateDeque.clear();
	m_collisionShapeDeque.clear();
}

void FenceModel::enforceFencePartsLimit(int maxFenceParts)
{
	if (m_maxFenceParts != maxFenceParts)
		m_maxFenceParts = maxFenceParts;

	int rigidBodyDequeSize = m_rigidBodyDeque.size();
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