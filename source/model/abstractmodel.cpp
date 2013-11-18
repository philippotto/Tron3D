#include "abstractmodel.h"

using namespace troen;

AbstractModel::AbstractModel()
{
	
}

std::shared_ptr<std::vector<btRigidBody>> AbstractModel::getRigidBodies()
{
	return m_rigidBodies;
}