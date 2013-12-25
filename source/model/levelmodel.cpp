#include "levelmodel.h"
//bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"

using namespace troen;

LevelModel::LevelModel(const LevelController* levelController)
{
	m_levelController = levelController;
	m_rigidBodies = std::vector<std::shared_ptr<btRigidBody>>();

	btScalar levelSize = btScalar(getLevelSize());

	addFloor(levelSize, -10);
	addFloor(levelSize * 2, -100);

	addWalls(levelSize, -10);
	addWalls(levelSize * 2, -100);
}

void LevelModel::addFloor(float size, float yPosition)
{
	m_floors.push_back({
		btVector3(0, 0, yPosition),
		btVector3(size, size, 20)
	});
	
	addBoxes(m_floors);
}

void LevelModel::addWalls(float levelSize, float yPosition)
{

	btScalar wallHeight = 40;

	std::vector<BoxModel> newWalls = {
		{
			btVector3(levelSize / 2, 1, yPosition + wallHeight/2),
			btVector3(2, levelSize, wallHeight)
		},
		{
			btVector3(-levelSize / 2, 0, yPosition + wallHeight / 2),
			btVector3(2, levelSize, wallHeight)
		},
		{
			btVector3(0, levelSize / 2, yPosition + wallHeight / 2),
			btVector3(levelSize, 2, wallHeight)
		},
		{
			btVector3(0, -levelSize / 2, yPosition + wallHeight / 2),
			btVector3(levelSize, 2, wallHeight)
		}
	};

	m_walls.insert(m_walls.end(), newWalls.begin(), newWalls.end());

	addBoxes(m_walls);
}

void LevelModel::addBoxes(std::vector<BoxModel> &boxes)
{
	for (int i = 0; i < boxes.size(); ++i)
	{
		std::shared_ptr<btBoxShape> wallShape = std::make_shared<btBoxShape>(boxes[i].dimensions / 2);
		std::shared_ptr<btDefaultMotionState> wallMotionState
			= std::make_shared<btDefaultMotionState>(btTransform(boxes[i].rotation, boxes[i].center));
		
		btRigidBody::btRigidBodyConstructionInfo
			wallRigidBodyCI(btScalar(0), wallMotionState.get(), wallShape.get(), btVector3(0, 0, 0));
		
		std::shared_ptr<btRigidBody> wallRigidBody = std::make_shared<btRigidBody>(wallRigidBodyCI);
		wallRigidBody->setUserPointer((void *) m_levelController);
		wallRigidBody->setUserIndex(LEVELWALLTYPE);
		
		m_collisionShapes.push_back(wallShape);	
		m_motionStates.push_back(wallMotionState);
		m_rigidBodies.push_back(wallRigidBody);
	}
}

int LevelModel::getLevelSize() {
	return 6000;
}