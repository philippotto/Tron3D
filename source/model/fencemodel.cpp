#include "fencemodel.h"

using namespace troen;

FenceModel::FenceModel()
{
	// TODO: remove m_rigidBodies completely (in favor of m_rigidBodyRefs)
	// m_rigidBodies = std::make_shared<std::vector<btRigidBody>>();	
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

	m_rigidBodyRefs.push_back(fenceRigidBody);
}

btRigidBody* FenceModel::getLastPart() {
	return m_rigidBodyRefs.back().get();
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

	m_rigidBodyRefs.push_back(fenceRigidBody);
}

float FenceModel::getFenceHeight()
{
	return 20;
}

// respectively: updateFence ?
void FenceModel::addFence()
{
	/*
	we want to have a concave shape (which will be updated every frame), so we got the following options:
	- btBvhTriangleMeshShape with btTriangleMesh or btTriangleIndexVertexArray as input (no difference in performance)
	"it is recommend to enable useQuantizedAabbCompression for better memory usage"
	- create independent boxes (respectively glue them together in a compoundShape)


	first approach will be: btBvhTriangleMeshShape with btTriangleMesh
	*/



	btTriangleMesh fenceMesh;
	/*
	the mesh will look this (I should study art)
	1 4                5 ...

	0		             2 3 ...
	*/

	btScalar fenceHeight = 1;
	btScalar fenceStep = 1;

	// TODO change initial value of X
	btScalar currentX = 0;

	// mind the manipulation of i within the loop
	for (int i = 0; i < 10; ++i)
	{

		currentX += fenceStep;

		fenceMesh.addTriangle(
			btVector3(currentX, 0, 0),
			btVector3(currentX, fenceHeight, 0),
			btVector3(currentX + fenceStep, 0, 0),
			false
			);

		i++;

		currentX += fenceStep;

		fenceMesh.addTriangle(
			btVector3(currentX, 0, 0),
			btVector3(currentX - fenceStep, fenceHeight, 0),
			btVector3(currentX, fenceHeight, 0),
			false
			);
	}


	btBvhTriangleMeshShape *fenceShape = new btBvhTriangleMeshShape(&fenceMesh, false);


	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0, 7, 0));
	btDefaultMotionState* motionState = new btDefaultMotionState(trans);



	// we set localInertia to a zero vector
	// maybe, the mass has to be 0 instead of 1 ?
	btRigidBody* fenceBody = new btRigidBody(1, motionState, fenceShape, btVector3(0, 0, 0));


	// do we have to adjust these parameters ?
	fenceBody->setContactProcessingThreshold(BT_LARGE_FLOAT);
	fenceBody->setCcdMotionThreshold(.5);
	fenceBody->setCcdSweptSphereRadius(0);
}