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

#include "itemcontroller.h"

using namespace troen;

LevelController::LevelController()
{
	AbstractController();
	m_model = std::make_shared<LevelModel>(this);
	m_view = std::make_shared<LevelView>(std::static_pointer_cast<LevelModel>(m_model));
	
	m_currentItemCount = 0;

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



osg::ref_ptr<osg::Group>  LevelController::getFloorView()
{
		return std::static_pointer_cast<LevelView>(m_view)->getFloor();

}
void LevelController::attachWorld(std::shared_ptr<PhysicsWorld> &world)
{
	m_world = world;
}

void LevelController::addItemBox()
{
	float x = randf(0, LEVEL_SIZE) - LEVEL_SIZE / 2;
	float y = randf(0, LEVEL_SIZE) - LEVEL_SIZE / 2;
	btVector3 position(x, y, +0.5);

	// the item controller will remove itself
	new ItemController(position, m_world, std::static_pointer_cast<LevelView>(m_view).get());

	m_currentItemCount++;
}

void troen::LevelController::update()
{
	if (m_currentItemCount >= m_targetItemCount) {
		return;
		m_targetItemCount = 0;
	}
		

	// this method is called in each frame, so the amount of items will be refreshed relatively quickly
	// creating all at once would cause a lag

	addItemBox();
}
