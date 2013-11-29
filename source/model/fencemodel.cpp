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
	
	// TODO: convert to shared_ptr
	btBoxShape *boxShape = new btBoxShape(btVector3(1, fenceVector.length() / 2, getFenceHeight() / 2));
	
	const btVector3 up = btVector3(0, 0, 1);
	const btVector3 forward = btVector3(0, 1, 0);
	const btScalar angle = fenceVector.angle(forward);

	btQuaternion rotationQuat;
	if (angle > 0) {
		btVector3 axis = fenceVector.cross(-forward).normalized();
		rotationQuat = btQuaternion(axis, angle);
	}
	btDefaultMotionState *fenceMotionState = new btDefaultMotionState(btTransform(rotationQuat, (a + b) / 2 + btVector3(0, 0, getFenceHeight() / 2)));

	const btScalar mass = 0;
	const btVector3 fenceInertia(0, 0, 0);
	
	btRigidBody::btRigidBodyConstructionInfo m_fenceRigidBodyCI(mass, fenceMotionState, boxShape, fenceInertia);

	std::shared_ptr<btRigidBody> fenceRigidBody = std::make_shared<btRigidBody>(m_fenceRigidBodyCI);
	fenceRigidBody->setUserPointer(m_fenceController);

	m_rigidBodies.push_back(fenceRigidBody);
}

btRigidBody* FenceModel::getLastPart() {
	return m_rigidBodies.back().get();
}

void FenceModel::addFenceMarker(btVector3 a)
{
	// TODO: convert to shared_ptr
	btBoxShape *boxShape = new btBoxShape(btVector3(0.5, 0.5, getFenceHeight() / 2));
	
	btDefaultMotionState *fenceMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), a + btVector3(0, 0, getFenceHeight() / 4)));

	// set mass to zero so that we get a static rigidBody (is this enough?)
	btScalar mass = 0;
	btVector3 fenceInertia(0, 0, 0);
	// boxShape->calculateLocalInertia(mass, fenceInertia);www

	btRigidBody::btRigidBodyConstructionInfo m_fenceRigidBodyCI(mass, fenceMotionState, boxShape, fenceInertia);

	std::shared_ptr<btRigidBody> fenceRigidBody = std::make_shared<btRigidBody>(m_fenceRigidBodyCI);
	fenceRigidBody->setUserPointer(m_fenceController);

	m_rigidBodies.push_back(fenceRigidBody);
}

float FenceModel::getFenceHeight()
{
	return 20;
}