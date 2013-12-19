#include "levelcontroller.h"
// OSG
#include <osg/Group>
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../constants.h"
#include "../model/LevelModel.h"
#include "../view/LevelView.h"

using namespace troen;

LevelController::LevelController()
{
	m_model = std::make_shared<LevelModel>(this);
	m_view = std::make_shared<LevelView>(std::static_pointer_cast<LevelModel>(m_model));

	initializeinitialBikePositionTransforms();
}

btTransform LevelController::initialPositionTransformForBikeWithIndex(int index)
{
	return m_initialBikePositionTransforms[index];
}

void LevelController::initializeinitialBikePositionTransforms()
{
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(100, 100, DEFAULT_BIKE_DIMENSIONS.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(100, -100, DEFAULT_BIKE_DIMENSIONS.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-100, -100, DEFAULT_BIKE_DIMENSIONS.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-100, 100, DEFAULT_BIKE_DIMENSIONS.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(10, 10, DEFAULT_BIKE_DIMENSIONS.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-10, -10, DEFAULT_BIKE_DIMENSIONS.z() / 2)));
}
