#include "fencemodel.h"

using namespace troen;

FenceModel::FenceModel(FenceController* fenceController)
{
	m_fenceController = fenceController;
	m_rigidBodies = std::vector<std::shared_ptr<btRigidBody>>();
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

	m_collisionShapes.push_back(fenceShape);
	m_motionStates.push_back(fenceMotionState);
	m_rigidBodies.push_back(fenceRigidBody);
}

btRigidBody* FenceModel::getLastPart() {
	if (m_rigidBodies.empty())
		return NULL;

	return m_rigidBodies.back().get();
}

void FenceModel::addFenceMarker(btVector3 a)
{
	std::shared_ptr<btBoxShape> fenceShape = std::make_shared<btBoxShape>(btVector3(0.5, 0.5, getFenceHeight() / 2));
	
	std::shared_ptr<btDefaultMotionState> fenceMotionState = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), a + btVector3(0, 0, getFenceHeight() / 4)));

	// set mass to zero so that we get a static rigidBody (is this enough?)
	btScalar mass = 0;
	btVector3 fenceInertia(0, 0, 0);
	// boxShape->calculateLocalInertia(mass, fenceInertia);www

	btRigidBody::btRigidBodyConstructionInfo m_fenceRigidBodyCI(mass, fenceMotionState.get(), fenceShape.get(), fenceInertia);

	std::shared_ptr<btRigidBody> fenceRigidBody = std::make_shared<btRigidBody>(m_fenceRigidBodyCI);
	fenceRigidBody->setUserPointer(m_fenceController);

	m_collisionShapes.push_back(fenceShape);
	m_motionStates.push_back(fenceMotionState);
	m_rigidBodies.push_back(fenceRigidBody);
}

float FenceModel::getFenceHeight()
{
	return 15;
}

void FenceModel::removeAllFences()
{
	m_rigidBodies.clear();
	m_motionStates.clear();
	m_collisionShapes.clear();
}