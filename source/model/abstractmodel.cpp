#include "abstractmodel.h"

using namespace troen;

AbstractModel::AbstractModel()
{
	
}

std::vector<std::shared_ptr<btRigidBody>> AbstractModel::getRigidBodies()
{
	return m_rigidBodies;
}