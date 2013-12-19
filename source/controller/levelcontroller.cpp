#include "levelcontroller.h"
// OSG
#include <osg/Group>
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../model/LevelModel.h"
#include "../view/LevelView.h"

using namespace troen;

LevelController::LevelController()
{
	m_model = std::make_shared<LevelModel>(this);
	m_view = std::make_shared<LevelView>(std::static_pointer_cast<LevelModel>(m_model));

	// TODO:
	// (jd) move these magic numbers
	btVector3 bikeDimensions = btVector3(12.5, 25, 12.5);
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(100, 100, bikeDimensions.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(100, -100, bikeDimensions.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-100, -100, bikeDimensions.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-100, 100, bikeDimensions.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(10, 10, bikeDimensions.z() / 2)));
	m_initialBikePositionTransforms.push_back(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-10, -10, bikeDimensions.z() / 2)));

}

btTransform LevelController::initialPositionTransformForBikeWithIndex(int index)
{
	return m_initialBikePositionTransforms[index];
}
