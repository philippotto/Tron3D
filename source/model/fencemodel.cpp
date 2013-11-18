#include "fencemodel.h"

using namespace troen;

FenceModel::FenceModel()
{
	m_rigidBodies = std::make_shared<std::vector<btRigidBody>>();

	for (size_t i = 0; i < 100; i++)
		addFencePart(10 * i);
	
	
}

void FenceModel::addFencePart(int offset)
{
	btBoxShape *boxShape = new btBoxShape(btVector3(1, 10, 10));

	// TODO: convert to shared_ptr
	btDefaultMotionState *fenceMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, offset, 100)));

	// set mass to zero so that we get a static rigidBody (is this enough?)
	btScalar mass = 0;
	btVector3 fenceInertia(0, 0, 0);
	// boxShape->calculateLocalInertia(mass, fenceInertia);

	btRigidBody::btRigidBodyConstructionInfo m_fenceRigidBodyCI(mass, fenceMotionState, boxShape, fenceInertia);
	btRigidBody fenceRigidBody(m_fenceRigidBodyCI);


	m_rigidBodies->push_back(fenceRigidBody);
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