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
	btVector3 fenceVectorXY = btVector3(fenceVector.getX(), fenceVector.getY(), 0);
	
	const btVector3 yAxis = btVector3(0, 1, 0);
	const btScalar angle = fenceVectorXY.angle(yAxis);
	const btScalar inverseAngle = fenceVectorXY.angle(-1 * yAxis);

	const btScalar zAngle = fenceVectorXY.angle(fenceVector);

	btQuaternion rotationQuatXY;
	btQuaternion roatationQuat;
	if (angle != 0 && inverseAngle != 0) {
		btVector3 axis = fenceVector.cross(-yAxis).normalized();
		axis.setX(0);
		axis.setY(0);
		axis.normalize();
		rotationQuatXY = btQuaternion(axis, angle);
		//std::cout << axis.x() << " " << axis.y() << " " << axis.z() << std::endl;
	}
	else {
		rotationQuatXY = btQuaternion(0, 0, 0, 1);
		std::cout << "other:" << std::endl;
	}

	//
	// try using euler angles
	//pitch is asin(y), if y is up.Once you have pitch, you have yaw as acos(x / r) where r = cos(pitch).
	//
	//btScalar roll = 0;
	//btScalar pitch = asin(fenceVector.normalized().z());
	//btScalar yaw = acos(fenceVector.normalized().x() / cos(pitch));
	//std::cout << yaw << " " << pitch << " " << roll << std::endl;
	//btQuaternion eulerQuaternion(yaw,pitch,roll);

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