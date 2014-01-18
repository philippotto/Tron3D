#include "levelcontroller.h"
// OSG
#include <osg/Group>
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../constants.h"
#include "../model/LevelModel.h"
#include "../view/LevelView.h"
#include "../model/physicsworld.h"

using namespace troen;

LevelController::LevelController()
{
	AbstractController();
	m_model = std::make_shared<LevelModel>(this);
	m_view = std::make_shared<LevelView>(std::static_pointer_cast<LevelModel>(m_model));

	initializeSpawnPoints();
}

btTransform LevelController::getSpawnPointForBikeWithIndex(int index)
{
	return m_initialBikePositionTransforms[index];
}

btTransform LevelController::getRandomSpawnPoint()
{
	int index = randf(0, m_initialBikePositionTransforms.size() - 1);
	return m_initialBikePositionTransforms[index];
}

void LevelController::initializeSpawnPoints()
{
	btVector3 Z_AXIS(0, 0, 1);
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(Z_AXIS, (float)PI * 3.f / 4.f), btVector3(20, 20, BIKE_DIMENSIONS.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(Z_AXIS, (float)PI * 1.f / 4.f), btVector3(20, -20, BIKE_DIMENSIONS.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(Z_AXIS, (float)-PI * 1.f / 4.f), btVector3(-20, -20, BIKE_DIMENSIONS.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(Z_AXIS, (float)-PI * 3.f / 4.f), btVector3(-20, 20, BIKE_DIMENSIONS.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(Z_AXIS,0), btVector3(100, 100, BIKE_DIMENSIONS.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(Z_AXIS,0), btVector3(-100, -100, BIKE_DIMENSIONS.z() / 2)));
}

void LevelController::attachWorld(std::shared_ptr<PhysicsWorld> &world)
{
	m_world = world;
}

void LevelController::addItemBox(btVector3 &position)
{
	btCollisionObject *item = std::static_pointer_cast<LevelModel>(m_model)->createItemBox(position);
	m_world.lock()->addCollisionObject(item);

	std::static_pointer_cast<LevelView>(m_view)->addItemBox(btToOSGVec3(position));
}